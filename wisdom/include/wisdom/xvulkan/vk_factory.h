#pragma once
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/xvulkan/vk_adapter.h>

#include <mutex>
#include <vector>

namespace wis {
class VKFactory;

template<>
struct Internal<VKFactory> {
    wis::shared_handle<VkInstance> factory;
    h::VkDebugUtilsMessengerEXT messenger;
    uint32_t api_version{};
    std::unique_ptr<wis::VkInstanceTable> instance_table{};

public:
    static inline wis::LibToken lib_token;
    static inline wis::VkGlobalTable global_table{};
    static inline std::once_flag global_flag;
};

WIS_INLINE [[nodiscard]] wis::ResultValue<wis::VKFactory>
VKCreateFactory(bool debug_layer = false, wis::DebugCallback callback = nullptr,
                void* user_data = nullptr) noexcept;

class VKFactory : public QueryInternal<VKFactory>
{
    struct IndexedAdapter {
        uint32_t index_consumption = 0;
        uint32_t index_performance = 0;
        VKAdapter adapter;
    };

    friend wis::ResultValue<wis::VKFactory> VKCreateFactory(bool, wis::DebugCallback,
                                                            void*) noexcept;
    static WIS_INLINE VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackThunk(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) noexcept;

public:
    VKFactory() noexcept = default;
    WIS_INLINE explicit VKFactory(
            wis::shared_handle<VkInstance> instance, uint32_t api_ver, bool debug_layer = false,
            std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback = {}) noexcept;
    WIS_INLINE ~VKFactory() noexcept;

    VKFactory(const VKFactory&) = delete;
    VKFactory(VKFactory&&) noexcept = default;
    VKFactory& operator=(const VKFactory&) = delete;
    VKFactory& operator=(VKFactory&&) noexcept = default;

    operator bool() const noexcept { return bool(factory); }
    operator VKFactoryHandle() const noexcept { return { factory, instance_table.get() }; }

public:
    WIS_INLINE [[nodiscard]] wis::ResultValue<VKAdapter>
    GetAdapter(uint32_t index,
               AdapterPreference preference = AdapterPreference::Performance) const noexcept;

private:
    WIS_INLINE VkResult EnumeratePhysicalDevices() noexcept;

    WIS_INLINE void
    EnableDebugLayer(std::unique_ptr<std::pair<wis::DebugCallback, void*>> xdebug_callback) noexcept;

    static void InitializeGlobalTable() noexcept
    {
        std::call_once(global_flag, []() { global_table.Init(lib_token); });
    }

    static WIS_INLINE std::vector<const char*> FoundExtensions() noexcept;
    static WIS_INLINE std::vector<const char*> FoundLayers() noexcept;

private:
    mutable std::vector<IndexedAdapter> adapters{};
    std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback{};
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_factory.cpp"
#endif // !WISDOM_HEADER_ONLY