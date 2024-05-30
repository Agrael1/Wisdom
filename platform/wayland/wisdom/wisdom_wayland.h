#pragma once
#if defined(WISDOM_VULKAN)
// TODO: Remove this when the implementation is complete
#define VK_USE_PLATFORM_WAYLAND_KHR
#include <vulkan/vulkan.h>
#undef VK_USE_PLATFORM_WAYLAND_KHR

#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#include <wisdom/vulkan/vk_factory.h>

namespace wis {
namespace platform {
class WaylandExtension;
} // namespace platform

template<>
struct Internal<platform::WaylandExtension> {
    wis::SharedInstance instance;
    PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR = nullptr;
};

namespace platform {
class WaylandExtension : public QueryInternalExtension<WaylandExtension, VKFactoryExtensionImpl<platform::WaylandExtension>>
{
public:
    static constexpr std::array<const char* const, 1> required_extensions = {
        VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
    };

    [[nodiscard]] wis::Result Init(const wis::VKFactory& in_instance) noexcept override
    {
        auto& gt = detail::VKFactoryGlobals::Instance().global_table;
        instance = in_instance.GetInternal().factory;

        vkCreateWaylandSurfaceKHR = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(gt.vkGetInstanceProcAddr(instance.get(), "vkCreateWaylandSurfaceKHR"));
        vkGetPhysicalDeviceWaylandPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(gt.vkGetInstanceProcAddr(instance.get(), "vkGetPhysicalDeviceWaylandPresentationSupportKHR"));

        if (!vkCreateWaylandSurfaceKHR) {
            return wis::make_result<FUNC, "Failed to load Wayland extension functions">(VK_ERROR_FEATURE_NOT_PRESENT);
        }
        return {};
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, wl_display* display, struct wl_surface* surface) const noexcept;
};
} // namespace platform
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_wayland.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_LINUX