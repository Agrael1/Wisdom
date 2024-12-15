#ifndef WIS_VK_FACTORY_H
#define WIS_VK_FACTORY_H
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
    VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
    VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
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

    static wis::VKMainGlobal& GetGlobalTable() noexcept
    {
        return Instance().global_table;
    }

protected:
    WIS_INLINE wis::Result InitializeGlobalTable() noexcept;
    WIS_INLINE wis::Result InitializeInstanceExtensions() noexcept;
    WIS_INLINE wis::Result InitializeInstanceLayers() noexcept;

private:
    bool initialized = false;
    std::once_flag global_flag;
    wis::VKMainGlobal global_table{};
    wis::LibToken lib_token;

public:
    // Used for checking if extensions are supported
    std::unordered_set<std::string, wis::string_hash, std::equal_to<>> instance_extensions;
    std::unordered_set<std::string, wis::string_hash, std::equal_to<>> instance_layers;
};
} // namespace detail

template<>
struct Internal<VKFactory> {
    wis::SharedInstance factory;
    uint32_t api_version{};
    bool debug_layer = false;

public:
    template<typename PFN>
    [[nodiscard]] PFN GetInstanceProcAddr() const noexcept
    {
        return factory.GetInstanceProcAddr<PFN>();
    }
};

class ImplVKFactory : public QueryInternal<VKFactory>
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
    ImplVKFactory() noexcept = default;
    ImplVKFactory(ImplVKFactory&&) noexcept = default;
    ImplVKFactory& operator=(ImplVKFactory&&) noexcept = default;

    operator bool() const noexcept
    {
        return bool(factory);
    }
    operator VKFactoryHandle() const noexcept
    {
        return { factory };
    }

public:
    [[nodiscard]] WIS_INLINE VKAdapter
    GetAdapter(wis::Result& resutlt, uint32_t index,
               AdapterPreference preference = AdapterPreference::Performance) const noexcept;

public:
    WIS_INLINE VkResult VKEnumeratePhysicalDevices() noexcept;

    [[nodiscard]] static WIS_INLINE wis::detail::fixed_allocation<const char*>
    FoundExtensions(wis::Result& result, std::span<const char*> in_extensions) noexcept;
    [[nodiscard]] static WIS_INLINE wis::detail::fixed_allocation<const char*>
    FoundLayers(wis::Result& result, std::span<const char*> in_layers) noexcept;

private:
    mutable std::vector<IndexedAdapter> adapters{};
};

#pragma region VKFactory
/**
 * @brief Class for creating adapters.
 * Can be safely destroyed after adapter has been chosen.
 * */
class VKFactory : public wis::ImplVKFactory
{
public:
    using wis::ImplVKFactory::ImplVKFactory;

public:
    /**
     * @brief Creates the wis::VKAdapter for the factory with provided index.
     * @param index The index of the adapter to get.
     * @param preference The preference of the adapter to get.
     * Default is wis::AdapterPreference::Performance.
     * @return wis::VKAdapter on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKAdapter GetAdapter(wis::Result& result, uint32_t index, wis::AdapterPreference preference = wis::AdapterPreference::Performance) const noexcept
    {
        return wis::ImplVKFactory::GetAdapter(result, index, preference);
    }
    /**
     * @brief Creates the wis::VKAdapter for the factory with provided index.
     * @param index The index of the adapter to get.
     * @param preference The preference of the adapter to get.
     * Default is wis::AdapterPreference::Performance.
     * @return wis::VKAdapter on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKAdapter> GetAdapter(uint32_t index, wis::AdapterPreference preference = wis::AdapterPreference::Performance) const noexcept
    {
        return wis::ResultValue<wis::VKAdapter>{ &wis::ImplVKFactory::GetAdapter, this, index, preference };
    }
};
#pragma endregion VKFactory

[[nodiscard]] WIS_INLINE wis::VKFactory
ImplVKCreateFactory(wis::Result& result, bool debug_layer, VKFactoryExtension** extensions, size_t extension_count) noexcept;
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_factory.cpp"
#include "impl/vk_create_factory.cpp"
#endif // !WISDOM_BUILD_BINARIES
#endif // VK_FACTORY_H
