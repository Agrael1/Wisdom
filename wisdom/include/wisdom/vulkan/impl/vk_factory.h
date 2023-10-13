#ifndef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/xvk_factory.h>
#endif // !WISDOM_HEADER_ONLY
#include <unordered_map>
#include <vector>
#include <wisdom/util/misc.h>
#include <wisdom/global/definitions.h>

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
        std::vector<VkExtensionProperties> vextensions;
        uint32_t count = 0;
        auto vr = VKGT.vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        do
            vextensions.resize(count);
        while ((vr = VKGT.vkEnumerateInstanceExtensionProperties(nullptr, &count, vextensions.data())) == VK_INCOMPLETE);

        if (!wis::succeeded(vr))
            return;

        for (auto& i : vextensions)
            extensions.emplace(i.extensionName, i);
    }
    void LoadLayers() noexcept
    {
        std::vector<VkLayerProperties> vlayers;
        uint32_t count = 0;
        auto vr = VKGT.vkEnumerateInstanceLayerProperties(&count, nullptr);
        do
            vlayers.resize(count);
        while ((vr = VKGT.vkEnumerateInstanceLayerProperties(&count, vlayers.data())) == VK_INCOMPLETE);

        if (!wis::succeeded(vr))
            return;

        for (auto& i : vlayers)
            layers.emplace(i.layerName, i);
    }

    std::unordered_map<std::string, VkExtensionProperties, wis::string_hash> extensions;
    std::unordered_map<std::string, VkLayerProperties, wis::string_hash> layers;
};

// clang-format off
constexpr inline std::array req_extensions{
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
// clang-format on


std::vector<const char*> wis::VKFactory::FoundExtensions() noexcept
{
    const auto& extensions = FactoryData::GetExtensions();

    if constexpr (wis::debug_mode)
        wis::lib_info(FactoryData::ExtensionsString());

    std::vector<const char*> found_extension;
    for (const auto* extension : req_extensions) {
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
        const auto& layers = FactoryData::GetLayers();
        if constexpr (wis::debug_mode)
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
