#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_factory.h>
#include <ranges>
#include <unordered_map>
#include <wisdom/util/misc.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/global/definitions.h>
#endif

namespace {
struct FactoryData {
public:
    static const FactoryData& instance() noexcept
    {
        static FactoryData d;
        return d;
    }
    static auto& GetExtensions() noexcept
    {
        return instance().extensions;
    }
    static auto& GetLayers() noexcept
    {
        return instance().layers;
    }
    [[nodiscard]] static std::string ExtensionsString() noexcept
    {
        std::string debug_str1{ "Available Extensions:\n" };
        for (const auto& i : instance().extensions) {
            wis::format_to(std::back_inserter(debug_str1),
                           "{}\n",
                           i.first);
        }
        return debug_str1;
    }
    [[nodiscard]] static std::string LayersString() noexcept
    {
        std::string debug_str1{ "Available Layers:\n" };
        for (const auto& i : instance().layers) {
            wis::format_to(std::back_inserter(debug_str1),
                           "\t{}\n",
                           i.first);
        }
        return debug_str1;
    }

private:
    FactoryData(bool blayers = true, bool bextensions = true)
    {
        if (bextensions)
            LoadExtensions();
        if (blayers)
            LoadLayers();
    }

    void LoadExtensions() noexcept
    {
        auto vextensions = vk::enumerateInstanceExtensionProperties();
        for (auto& i : vextensions)
            extensions.emplace(i.extensionName, i);
    }
    void LoadLayers() noexcept
    {
        auto vlayers = vk::enumerateInstanceLayerProperties();
        for (auto& i : vlayers)
            layers.emplace(i.layerName, i);
    }

    std::unordered_map<std::string, vk::ExtensionProperties, wis::string_hash> extensions;
    std::unordered_map<std::string, vk::LayerProperties, wis::string_hash> layers;
};

constexpr inline std::array req_extensions
{
    VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_METAL_EXT)
            VK_EXT_METAL_SURFACE_EXTENSION_NAME,
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
            VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
            VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#endif
#if DEBUG_MODE
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

constexpr inline wis::Severity SeverityConvert(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
{
    switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        return wis::Severity::trace;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        return wis::Severity::info;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        return wis::Severity::warn;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        return wis::Severity::error;
    default:
        return wis::Severity::critical;
    }
}

inline constexpr uint32_t order_performance(vk::PhysicalDeviceType t)
{
    using enum vk::PhysicalDeviceType;
    switch (t) {
    default:
    case eOther:
    case eIntegratedGpu:
        return 3;
    case eDiscreteGpu:
        return 4;
    case eVirtualGpu:
        return 2;
    case eCpu:
        return 1;
    }
}
inline constexpr uint32_t order_power(vk::PhysicalDeviceType t)
{
    using enum vk::PhysicalDeviceType;
    switch (t) {
    default:
    case eOther:
    case eIntegratedGpu:
        return 4;
    case eDiscreteGpu:
        return 3;
    case eVirtualGpu:
        return 2;
    case eCpu:
        return 1;
    }
}
} // namespace

VKAPI_ATTR VkBool32 VKAPI_CALL wis::VKFactory::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT /*messageType*/, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* /*pUserData*/)
{
    wis::lib_log(SeverityConvert(messageSeverity),
                 wis::format(
                         "\n[Validation layer]: {}\n [Message]:{}", pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "", pCallbackData->pMessage));
    return false;
}

wis::VKFactory::VKFactory(const ApplicationInfo& app_info, [[maybe_unused]] bool unused)
{
    if (instance_count.fetch_add(1, std::memory_order_relaxed) != 0)
        return;

    wis::lib_info("Initializing Instance");
    uint32_t version = 0;
    vkEnumerateInstanceVersion(&version);

    wis::lib_log(Severity::info, wis::format("Vulkan ver: {}.{}.{}", VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

    static constexpr auto version_mask = 0xFFFU;
    api_version = version &= ~(version_mask); // unsigned remove patch from instance for compatibility

    vk::ApplicationInfo info{
        app_info.application_name,
        VK_MAKE_API_VERSION(0, app_info.app_version.major, app_info.app_version.minor, app_info.app_version.patch),
        app_info.engine_name,
        VK_MAKE_API_VERSION(0, app_info.engine_version.major, app_info.engine_version.minor, app_info.engine_version.patch),
        version
    };

    auto found_extension = FoundExtensions();
    auto found_layers = FoundLayers();

    vk::InstanceCreateInfo create_info;
    create_info.pApplicationInfo = &info;
    create_info.enabledLayerCount = static_cast<uint32_t>(found_layers.size());
    create_info.ppEnabledLayerNames = found_layers.data();
    create_info.enabledExtensionCount = static_cast<uint32_t>(found_extension.size());
    create_info.ppEnabledExtensionNames = found_extension.data();

    constexpr static vk::DebugUtilsMessengerCreateInfoEXT create_instance_debug(
            vk::DebugUtilsMessengerCreateFlagsEXT(0),
            vk::DebugUtilsMessageSeverityFlagsEXT(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT),
            vk::DebugUtilsMessageTypeFlagsEXT(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT),
            VKFactory::debugCallback);

    if constexpr (debug_mode) {
        create_info.pNext = &create_instance_debug;
    }

    factory = wis::shared_handle<vk::Instance>{ vk::createInstance(create_info) };
    DynamicLoader::loader = vk::DispatchLoaderDynamic{ factory.get(), vkGetInstanceProcAddr };
    DynamicLoader::init = true;

    if constexpr (debug_mode) {
        messenger = wis::shared_handle<vk::DebugUtilsMessengerEXT>{ factory->createDebugUtilsMessengerEXT(create_instance_debug, nullptr, DynamicLoader::loader), factory, { nullptr, DynamicLoader::loader } };
    }
}

// NOLINTNEXTLINE
wis::generator<wis::VKAdapter> wis::VKFactory::EnumerateAdapters(AdapterPreference preference) const noexcept
{
    auto adapters = factory->enumeratePhysicalDevices();

    if (adapters.size() > 1) {
        switch (preference) {
        case wis::AdapterPreference::None:
            break;
        case wis::AdapterPreference::MinConsumption:
            std::ranges::sort(adapters, [](vk::PhysicalDevice a, vk::PhysicalDevice b) {
                auto x = a.getProperties();
                auto y = b.getProperties();

                if (order_power(x.deviceType) > order_power(y.deviceType))
                    return true;
                return x.limits.maxMemoryAllocationCount > y.limits.maxMemoryAllocationCount;
            });
            break;
        case wis::AdapterPreference::Performance:
            std::ranges::sort(adapters, [](vk::PhysicalDevice a, vk::PhysicalDevice b) {
                auto x = a.getProperties();
                auto y = b.getProperties();

                if (order_performance(x.deviceType) > order_performance(y.deviceType))
                    return true;
                return x.limits.maxMemoryAllocationCount > y.limits.maxMemoryAllocationCount;
            });
            break;
        default:
            break;
        }
    }

    for (auto& a : adapters) {
        co_yield VKAdapter{ a };
    }
}

std::vector<const char*> wis::VKFactory::FoundExtensions() noexcept
{
    const auto& extensions = FactoryData::GetExtensions();

    if constexpr (debug_mode)
        wis::lib_info(FactoryData::ExtensionsString());

    std::vector<const char*> found_extension;
    for (const auto* extension : req_extensions) {
        if (extensions.contains(extension))
            found_extension.push_back(extension);
    }

    if constexpr (debug_mode) {
        std::string debug_str{ "Used Extensions:\n" };
        for (const auto* i : found_extension)
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        wis::lib_info(std::move(debug_str));
    }

    return found_extension;
}

std::vector<const char*> wis::VKFactory::FoundLayers() noexcept
{
    std::vector<const char*> out;
    if constexpr (debug_mode) {
        const auto& layers = FactoryData::GetLayers();
        if constexpr (debug_mode)
            wis::lib_info(FactoryData::LayersString());

        if (layers.contains("VK_LAYER_KHRONOS_validation"))
            out.push_back("VK_LAYER_KHRONOS_validation");

        std::string debug_str{ "Used Layers:\n" };
        for (const auto* i : out)
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        wis::lib_info(std::move(debug_str));
    }

    return out;
}
