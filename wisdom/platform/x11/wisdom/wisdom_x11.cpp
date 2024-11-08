#ifndef WISDOM_X11_CPP
#define WISDOM_X11_CPP
#include <wisdom/wisdom_x11.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/vulkan/vk_device.h>

wis::ResultValue<wis::VKSwapChain>
wis::platform::X11Extension::CreateSwapchain(const wis::VKDevice& device, wis::VKQueueView main_queue, const wis::SwapchainDesc* desc, Display* display, Window window) const noexcept
{
    VkXlibSurfaceCreateInfoKHR surface_desc{
        .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .dpy = display,
        .window = window
    };
    wis::lib_info("Initializing X11 Surface");

    auto& devicei = device.GetInternal();
    const auto& instance_table = instance.table();
    VkSurfaceKHR surface;
    auto result = vkCreateXlibSurfaceKHR(instance.get(), &surface_desc, nullptr, &surface);
    if (!wis::succeeded(result)) {
        return wis::make_result<FUNC, "Failed to create X11 surface">(result);
    }
    return device.VKCreateSwapChain(wis::SharedSurface{ surface, instance, instance_table.vkDestroySurfaceKHR }, desc, std::get<0>(main_queue));
}

#endif // WISDOM_XCB_CPP
