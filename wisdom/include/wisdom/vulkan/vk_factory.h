#pragma once
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_debug.h>

#include <mutex>
#include <vector>

namespace wis {
class VKFactory;

namespace detail {
struct VKFactoryGlobals {
    static VKFactoryGlobals& Instance() noexcept
    {
        static VKFactoryGlobals instance;
        return instance;
    }
    void InitializeGlobalTable() noexcept
    {
        std::call_once(global_flag, [this]() {
            global_table.Init(lib_token);
        });
    }

    std::once_flag global_flag;
    wis::VkGlobalTable global_table{};
    wis::LibToken lib_token;
};
} // namespace detail

template<>
struct Internal<VKFactory> {
    wis::SharedInstance factory;
    uint32_t api_version{};
    bool debug_layer = false;
};

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKFactory>
VKCreateFactory(bool debug_layer = false) noexcept;

class VKFactory : public QueryInternal<VKFactory>
{
    struct IndexedAdapter {
        uint32_t index_consumption = 0;
        uint32_t index_performance = 0;
        VKAdapter adapter;
    };

    friend wis::ResultValue<wis::VKFactory> VKCreateFactory(bool) noexcept;
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

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKDebugMessenger>
    CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept;

private:
    WIS_INLINE VkResult EnumeratePhysicalDevices() noexcept;

    static WIS_INLINE std::vector<const char*> FoundExtensions() noexcept;
    static WIS_INLINE std::vector<const char*> FoundLayers() noexcept;

private:
    mutable std::vector<IndexedAdapter> adapters{};
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_factory.cpp"
#endif // !WISDOM_HEADER_ONLY