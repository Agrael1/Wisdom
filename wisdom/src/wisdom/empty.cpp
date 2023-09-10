#include <wisdom/vulkan/vk_managed_handles.h>

void a()
{
    VkInstance b{};
    wis::shared_handle<VkInstance> a{ b, &vkDestroyInstance };
    wis::shared_handle<VkDevice> d;

    VkImage i{};
    wis::shared_handle<VkImage> c{i, d, &vkDestroyImage, wis::owned_by_swapchain::no};
    wis::managed_handle<VkImage> e{i};
}