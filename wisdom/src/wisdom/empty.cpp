//#if WISDOM_VULKAN_FOUND
//#include <wisdom/vulkan/vk_managed_handles.h>
//
//void a()
//{
//    VkInstance b{};
//    wis::shared_handle<VkInstance> a{ b, &vkDestroyInstance };
//    wis::shared_handle<VkDevice> d;
//
//    VkImage i{};
//    wis::shared_handle<VkImage> c{ i, d, &vkDestroyImage, wis::owned_by_swapchain::no };
//    wis::managed_handle<VkImage> e{ i };
//}
//#endif

#include <wisdom/dx12/impl/dx12_info.h>
#include <wisdom/dx12/impl/dx12_device.h>
#include <wisdom/vulkan/impl/vk_factory.h>
#include <wisdom/vulkan/impl/vk_device.h>
