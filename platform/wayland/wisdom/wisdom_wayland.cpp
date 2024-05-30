#ifdef WISDOM_BUILD_BINARIES /* || defined(__INTELLISENSE__)*/
#include <wisdom/wisdom_wayland.h>
#else
#pragma once
#endif // !WISDOM_PLATFORM_HEADER_ONLY

#include <wisdom/util/log_layer.h>
#include <wisdom/vulkan/vk_device.h>

wis::ResultValue<wis::VKSwapChain>
wis::platform::WaylandExtension::CreateSwapchain(const wis::VKDevice& device, wis::VKQueueView main_queue, const wis::SwapchainDesc* desc, wl_display* display, wl_surface* surface)const noexcept
{
    VkWaylandSurfaceCreateInfoKHR surface_desc{
        .sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .display = display,
        .surface = surface
    };
    wis::lib_info("Initializing Wayland Surface");

    auto& devicei = device.GetInternal();
    const auto& instance_table = instance.table();
    VkSurfaceKHR out_surface;
    auto result = vkCreateWaylandSurfaceKHR(instance.get(), &surface_desc, nullptr, &out_surface);
    if (!wis::succeeded(result)) {
        return wis::make_result<FUNC, "Failed to create Win32 surface">(result);
    }
    wis::SharedSurface surface_handle{ out_surface, instance, instance_table.vkDestroySurfaceKHR };
    return device.VKCreateSwapChain(surface_handle, desc, std::get<0>(main_queue));
}