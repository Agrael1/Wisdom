#ifndef VK_DEBUG_H
#define VK_DEBUG_H
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

    Internal() noexcept = default;
    Internal(wis::SharedInstance instance, VkDebugUtilsMessengerEXT messenger, std::unique_ptr<detail::DebugCallbackData> data) noexcept
        : instance(std::move(instance)), messenger(messenger), data(std::move(data))
    {
    }
    Internal(Internal&& other) noexcept = default;
    Internal& operator=(Internal&& other) noexcept = default;

    ~Internal() noexcept
    {
        if (messenger)
            instance.table().vkDestroyDebugUtilsMessengerEXT(instance.get(), messenger, nullptr);
    }
};

class VKDebugMessenger : public QueryInternal<VKDebugMessenger>
{
public:
    VKDebugMessenger() noexcept = default;
    explicit VKDebugMessenger(wis::SharedInstance instance, VkDebugUtilsMessengerEXT messenger, std::unique_ptr<detail::DebugCallbackData> data) noexcept
        : QueryInternal(std::move(instance), messenger, std::move(data))
    {
    }
    operator bool() const noexcept
    {
        return bool(messenger);
    }
};
} // namespace wis

#endif // !VK_DEBUG_H
