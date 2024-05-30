#ifndef VK_CREATE_FACTORY_CPP
#define VK_CREATE_FACTORY_CPP
#include <wisdom/vulkan/vk_factory.h>
#include <unordered_map>
#include <array>
#include <wisdom/util/misc.h>

namespace wis::detail {
// required extensions for the instance
constexpr inline std::array instance_extensions
{
    VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#if defined(VK_USE_PLATFORM_METAL_EXT)
            VK_EXT_METAL_SURFACE_EXTENSION_NAME, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
                                                         VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
            VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#endif
#if DEBUG_MODE
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

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
            wis::format_to(std::back_inserter(debug_str1), "{}\n", i.first);
        }
        return debug_str1;
    }
    [[nodiscard]] static std::string LayersString() noexcept
    {
        std::string debug_str1{ "Available Layers:\n" };
        for (const auto& i : instance().layers) {
            wis::format_to(std::back_inserter(debug_str1), "\t{}\n", i.first);
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
        auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;
        std::vector<VkExtensionProperties> vextensions;
        uint32_t count = 0;
        auto vr = gt.vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        do
            vextensions.resize(count);
        while ((vr = gt.vkEnumerateInstanceExtensionProperties(nullptr, &count,
                                                               vextensions.data())) == VK_INCOMPLETE);

        if (!wis::succeeded(vr))
            return;

        for (auto& i : vextensions)
            extensions.emplace(i.extensionName, i);
    }
    void LoadLayers() noexcept
    {
        auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;
        std::vector<VkLayerProperties> vlayers;
        uint32_t count = 0;
        auto vr = gt.vkEnumerateInstanceLayerProperties(&count, nullptr);
        do
            vlayers.resize(count);
        while ((vr = gt.vkEnumerateInstanceLayerProperties(&count, vlayers.data())) == VK_INCOMPLETE);

        if (!wis::succeeded(vr))
            return;

        for (auto& i : vlayers)
            layers.emplace(i.layerName, i);
    }

    std::unordered_map<std::string_view, VkExtensionProperties, wis::string_hash> extensions;
    std::unordered_map<std::string_view, VkLayerProperties, wis::string_hash> layers;
};

} // namespace wis::detail

std::vector<const char*> wis::VKFactory::FoundExtensions() noexcept
{
    const auto& extensions = wis::detail::FactoryData::GetExtensions();

    if constexpr (wis::debug_mode)
        wis::lib_info(wis::detail::FactoryData::ExtensionsString());

    std::vector<const char*> found_extension;
    for (const auto* extension : wis::detail::instance_extensions) {
        if (extensions.contains(extension))
            found_extension.push_back(extension);
    }

    if constexpr (wis::debug_mode) {
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
    if constexpr (wis::debug_mode) {
        const auto& layers = wis::detail::FactoryData::GetLayers();
        if constexpr (wis::debug_mode)
            wis::lib_info(wis::detail::FactoryData::LayersString());

        if (layers.contains("VK_LAYER_KHRONOS_validation"))
            out.push_back("VK_LAYER_KHRONOS_validation");

        std::string debug_str{ "Used Layers:\n" };
        for (const auto* i : out)
            wis::format_to(std::back_inserter(debug_str), "{},\n", i);
        wis::lib_info(std::move(debug_str));
    }

    return out;
}

wis::ResultValue<wis::VKFactory>
wis::VKCreateFactoryEx(VkInstance instance, uint32_t version, bool debug_layer) noexcept
{
    wis::detail::VKFactoryGlobals::Instance().InitializeGlobalTable();
    auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;

    auto table = wis::detail::make_unique<wis::VkInstanceTable>();
    if (!table)
        return wis::make_result<FUNC, "Failed to create instance table">(VK_ERROR_OUT_OF_HOST_MEMORY);

    table->Init(instance, gt);

    auto factory = wis::VKFactory{ wis::SharedInstance{ instance, gt.vkDestroyInstance, std::move(table) }, version, debug_layer };
    auto result = factory.EnumeratePhysicalDevices();
    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Failed to enumerate physical devices">(result);

    return std::move(factory);
}

wis::ResultValue<wis::VKFactory>
wis::VKCreateFactory(bool debug_layer) noexcept
{
    detail::VKFactoryGlobals::Instance().InitializeGlobalTable();
    auto& gt = detail::VKFactoryGlobals::Instance().global_table;
    VkResult vr{};
    uint32_t version = 0;
    if (gt.vkEnumerateInstanceVersion) {
        vr = gt.vkEnumerateInstanceVersion(&version);
        if (!wis::succeeded(vr))
            return wis::make_result<FUNC, "Failed to enumerate instance version">(vr);
    } else {
        version = VK_API_VERSION_1_0;
    }

    wis::lib_info(wis::format("Vulkan version: {}.{}.{}", VK_API_VERSION_MAJOR(version),
                              VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

    VkApplicationInfo info{
        VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "", VK_MAKE_API_VERSION(0, 1, 0, 0), "",
        VK_MAKE_API_VERSION(0, 1, 0, 0), version
    };

    auto found_extension = VKFactory::FoundExtensions();
    auto found_layers = VKFactory::FoundLayers();

    VkInstanceCreateInfo create_info{ .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                      .pApplicationInfo = &info,
                                      .enabledLayerCount = static_cast<uint32_t>(found_layers.size()),
                                      .ppEnabledLayerNames = found_layers.data(),
                                      .enabledExtensionCount =
                                              static_cast<uint32_t>(found_extension.size()),
                                      .ppEnabledExtensionNames = found_extension.data() };

    wis::managed_handle<VkInstance> instance;
    vr = gt.vkCreateInstance(&create_info, nullptr, instance.put(gt.vkDestroyInstance));
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to create instance">(vr);

    auto table = wis::detail::make_unique<wis::VkInstanceTable>();
    if (!table)
        return wis::make_result<FUNC, "Failed to create instance table">(VK_ERROR_OUT_OF_HOST_MEMORY);

    table->Init(instance.get(), gt);

    auto factory = wis::VKFactory{ wis::SharedInstance{ instance.release(), gt.vkDestroyInstance, std::move(table) }, version, debug_layer };

    vr = factory.EnumeratePhysicalDevices();
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to enumerate physical devices">(vr);

    return std::move(factory);
}
#endif // !VK_CREATE_FACTORY_CPP
