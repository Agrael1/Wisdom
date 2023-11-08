#pragma once
#include <wisdom/vulkan/xvk_device.h>
#include <wisdom/vulkan/xvk_swapchain.h>

namespace wis {
[[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKSwapChain>
VKCreateSwapchainWin32(const VKDevice& device, void* hwnd) noexcept;
}

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/impl/vk_surface.h>
#endif // !WISDOM_HEADER_ONLY