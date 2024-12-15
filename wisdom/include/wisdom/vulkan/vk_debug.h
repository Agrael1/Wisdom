#ifndef WIS_VK_DEBUG_H
#define WIS_VK_DEBUG_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_handles.h>

namespace wis {
class VKDebugMessenger;

namespace detail {
struct DebugCallbackData {
    DebugCallback callback;
    void* user_data;
};
} // namespace detail

template<>
struct Internal<VKDebugMessenger> {
    wis::SharedInstance instance;
    h::VkDebugUtilsMessengerEXT messenger;
    std::unique_ptr<detail::DebugCallbackData> data;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;

public:
    Internal() noexcept = default;
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        Destroy();
        instance = std::move(other.instance);
        messenger = std::move(other.messenger);
        data = std::move(other.data);
        vkDestroyDebugUtilsMessengerEXT = other.vkDestroyDebugUtilsMessengerEXT;
        return *this;
    }

    ~Internal() noexcept
    {
        Destroy();
    }

    void Destroy() noexcept
    {
        if (messenger) {
            vkDestroyDebugUtilsMessengerEXT(instance.get(), messenger, nullptr);
        }
        messenger = nullptr;
    }
};

class VKDebugMessenger : public QueryInternal<VKDebugMessenger>
{
public:
    VKDebugMessenger() noexcept = default;
    operator bool() const noexcept
    {
        return bool(messenger);
    }
};
} // namespace wis

#endif // !VK_DEBUG_H
