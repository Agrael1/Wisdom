#pragma once
#ifndef WISDOM_PLATFORM_HEADER_ONLY
#include <wisdom/platform/linux.h>
#endif // !WISDOM_PLATFORM_HEADER_ONLY

#include <wisdom/util/log_layer.h>
#include <wisdom/vulkan/vk_device.h>

wis::ResultValue<wis::VKSwapChain>
wis::CreateSwapchainXcb(const wis::VKDevice& device, wis::VKQueueView main_queue, const wis::SwapchainDesc* desc, xcb_connection_t* connection, xcb_window_t window) noexcept
{
    VkXcbSurfaceCreateInfoKHR surface_desc{
        .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .connection = connection,
        .window = window
    };
    wis::lib_info("Initializing Xcb Surface");

    auto& devicei = device.GetInternal();
    const auto& instance_table = devicei.GetInstanceTable();
    const auto& instance = devicei.adapter.GetInternal().instance;
    VkSurfaceKHR surface;
    auto result = instance_table.vkCreateXcbSurfaceKHR(instance.get(), &surface_desc, nullptr, &surface);
    if (!wis::succeeded(result)) {
        return wis::make_result<FUNC, "Failed to create Win32 surface">(result);
    }
    wis::SharedSurface surface_handle{ surface, instance, instance_table.vkDestroySurfaceKHR };
    return device.VKCreateSwapChain(surface_handle, desc, std::get<0>(main_queue));
}

wis::ResultValue<wis::VKSwapChain>
wis::CreateSwapchainWayland(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, wl_display* display, wl_surface* surface) noexcept
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
    const auto& instance_table = devicei.GetInstanceTable();
    const auto& instance = devicei.adapter.GetInternal().instance;
    VkSurfaceKHR out_surface;
    auto result = instance_table.vkCreateWaylandSurfaceKHR(instance.get(), &surface_desc, nullptr, &out_surface);
    if (!wis::succeeded(result)) {
        return wis::make_result<FUNC, "Failed to create Win32 surface">(result);
    }
    wis::SharedSurface surface_handle{ out_surface, instance, instance_table.vkDestroySurfaceKHR };
    return device.VKCreateSwapChain(surface_handle, desc, std::get<0>(main_queue));
}

