#ifndef WISDOM_WAYLAND_CPP
#define WISDOM_WAYLAND_CPP

#ifndef WISDOM_MODULE_DECL
#include <wisdom/wisdom_wayland.hpp>
#include <wisdom/util/log_layer.h>
#include <wisdom/vulkan/vk_device.h>
#endif // !WISDOM_MODULE_DECL


wis::VKSwapChain
wis::platform::WaylandExtension::CreateSwapchain(wis::Result& result, const wis::VKDevice& device, wis::VKQueueView main_queue, const wis::SwapchainDesc& desc, wl_display* display, wl_surface* surface) const noexcept
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
    auto vr = vkCreateWaylandSurfaceKHR(instance.get(), &surface_desc, nullptr, &out_surface);
    if (!wis::succeeded(vr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to create Win32 surface">(vr);
        return {};
    }
    return device.VKCreateSwapChain(result, wis::SharedSurface{ out_surface, instance, instance_table.vkDestroySurfaceKHR }, desc, std::get<0>(main_queue));
}

#endif // WISDOM_WAYLAND_CPP
