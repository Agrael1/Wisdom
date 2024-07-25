#ifndef VK_FACTORY_H
#define VK_FACTORY_H
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_debug.h>
#include <wisdom/vulkan/vk_factory_ext.h>

#include <mutex>
#include <vector>
#include <unordered_set>

namespace wis {
class VKFactory;

namespace detail {
constexpr inline std::array instance_extensions{
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
};
constexpr inline std::span<const char* const> instance_layers{};

struct VKFactoryGlobals {
    static VKFactoryGlobals& Instance() noexcept
    {
        static VKFactoryGlobals instance;
        return instance;
    }

public:
    WIS_INLINE wis::Result InitializeFactoryGlobals() noexcept;

protected:
    WIS_INLINE wis::Result InitializeGlobalTable() noexcept;
    WIS_INLINE wis::Result InitializeInstanceExtensions() noexcept;
    WIS_INLINE wis::Result InitializeInstanceLayers() noexcept;

public:
    bool initialized = false;
    std::once_flag global_flag;
    wis::VKMainGlobal global_table{};
    wis::LibToken lib_token;

    std::unordered_set<std::string, wis::string_hash, std::equal_to<>> instance_extensions;
    std::unordered_set<std::string, wis::string_hash, std::equal_to<>> instance_layers;
};
} // namespace detail

template<>
struct Internal<VKFactory> {
    wis::SharedInstance factory;
    uint32_t api_version{};
    bool debug_layer = false;
};

class VKFactory : public QueryInternal<VKFactory>
{
    struct IndexedAdapter {
        uint32_t index_consumption = 0;
        uint32_t index_performance = 0;
        VKAdapter adapter;
    };
    static WIS_INLINE VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackThunk(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) noexcept;

public:
    VKFactory() noexcept = default;
    WIS_INLINE explicit VKFactory(
        wis::SharedInstance instance, uint32_t api_ver, bool debug) noexcept;

    VKFactory(const VKFactory&) = delete;
    VKFactory(VKFactory&&) noexcept = default;
    VKFactory& operator=(const VKFactory&) = delete;
    VKFactory& operator=(VKFactory&&) noexcept = default;

    operator bool() const noexcept
    {
        return bool(factory);
    }
    operator VKFactoryHandle() const noexcept
    {
        return { factory };
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKAdapter>
    GetAdapter(uint32_t index,
               AdapterPreference preference = AdapterPreference::Performance) const noexcept;

public:
    WIS_INLINE VkResult VKEnumeratePhysicalDevices() noexcept;

    [[nodiscard]] static WIS_INLINE wis::ResultValue<wis::detail::fixed_allocation<const char*>>
            FoundExtensions(std::span<const char*> in_extensions) noexcept;
    [[nodiscard]] static WIS_INLINE wis::ResultValue<wis::detail::fixed_allocation<const char*>>
            FoundLayers(std::span<const char*> in_layers) noexcept;

private:
    mutable std::vector<IndexedAdapter> adapters{};
};

[[nodiscard]] WIS_INLINE wis::ResultValue<VKFactory>
VKCreateFactory(bool debug_layer = false) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKFactory>
VKCreateFactoryWithExtensions(bool debug_layer, VKFactoryExtension** extensions, size_t extension_count) noexcept;

namespace detail {
[[nodiscard]] WIS_INLINE wis::ResultValue<VKFactory>
VKCreateFactoryEx(VkInstance instance, uint32_t version, bool debug_layer) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKFactory>
VKCreateFactoryWithExtensions(bool debug_layer, const char** exts, size_t extension_count, const char** layers, size_t layer_count) noexcept;
} // namespace detail
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_factory.cpp"
#include "impl/vk_create_factory.cpp"
#endif // !WISDOM_BUILD_BINARIES
#endif // VK_FACTORY_H
