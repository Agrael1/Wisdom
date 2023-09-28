#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/vk_managed_handles.h>

namespace wis {
class VKFactory;

template<>
struct Internal<VKFactory> {
    wis::shared_handle<VkInstance> factory;
    //wis::shared_handle<vk::DebugUtilsMessengerEXT> messenger;
    uint32_t api_version{};
};

WIS_EXPORT class VKFactory : public QueryInternal<VKFactory>
{
    static WIS_INLINE VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

public:
    /// @brief Create a vulkan instance with the given application info
    /// @param app_info Application info
    /// @param unused
    WIS_INLINE VKFactory(const ApplicationInfo& app_info = {}) noexcept;
    operator bool() const noexcept
    {
        return bool(factory);
    }
    operator VKFactoryHandle() const noexcept
    {
        return factory;
    }

    /// @brief Enumerates all adapters on the system
    /// @param preference Preference to use when enumerating adapters, changes the order of the adapters
    /// @return coroutine that yields VKAdapter
    [[nodiscard]] WIS_INLINE wis::generator<VKAdapter>
    EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance) const noexcept;

private:
    static WIS_INLINE std::vector<const char*> FoundExtensions() noexcept;
    static WIS_INLINE std::vector<const char*> FoundLayers() noexcept;
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_factory.inl"
#endif
