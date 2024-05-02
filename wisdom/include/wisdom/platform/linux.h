#pragma once
#if defined(WISDOM_LINUX) && WISDOM_VULKAN
#include <wisdom/vulkan/vk_swapchain.h>

namespace wis {
[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
CreateSwapchainXcb(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, xcb_connection_t* connection, xcb_window_t window) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
CreateSwapchainWayland(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, wl_display* display, wl_surface* surface) noexcept;
}




#ifdef WISDOM_PLATFORM_HEADER_ONLY
#include "impl/linux.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_LINUX
