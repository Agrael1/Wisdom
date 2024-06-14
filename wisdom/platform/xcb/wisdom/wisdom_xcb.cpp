#ifndef WISDOM_XCB_CPP
#define WISDOM_XCB_CPP
#include <wisdom/wisdom_xcb.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/vulkan/vk_device.h>

wis::ResultValue<wis::VKSwapChain>
wis::platform::XCBExtension::CreateSwapchain(const wis::VKDevice& device, wis::VKQueueView main_queue, const wis::SwapchainDesc* desc, xcb_connection_t* connection, xcb_window_t window) const noexcept
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
    const auto& instance_table = instance.table();
    VkSurfaceKHR surface;
    auto result = vkCreateXcbSurfaceKHR(instance.get(), &surface_desc, nullptr, &surface);
    if (!wis::succeeded(result)) {
        return wis::make_result<FUNC, "Failed to create Win32 surface">(result);
    }
    return device.VKCreateSwapChain(wis::SharedSurface{ surface, instance, instance_table.vkDestroySurfaceKHR }, desc, std::get<0>(main_queue));
}

#endif // WISDOM_XCB_CPP
