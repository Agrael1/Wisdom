#ifndef WISDOM_WAYLAND_H
#define WISDOM_WAYLAND_H
#if defined(WISDOM_VULKAN)
// TODO: Remove this when the implementation is complete
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#include <wisdom/vulkan/vk_factory.h>

struct wl_display;
struct wl_surface;

#include <vulkan/vulkan_wayland.h>

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

    [[nodiscard]] wis::Result
    Init(const wis::VKFactory& in_instance) noexcept override
    {
        instance = in_instance.GetInternal().factory;
        vkCreateWaylandSurfaceKHR = instance.GetInstanceProcAddr<PFN_vkCreateWaylandSurfaceKHR>("vkCreateWaylandSurfaceKHR");
        vkGetPhysicalDeviceWaylandPresentationSupportKHR = instance.GetInstanceProcAddr<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>("vkGetPhysicalDeviceWaylandPresentationSupportKHR");
        return {};
    }

    [[nodiscard]] bool
    Supported() const noexcept override
    {
        return vkCreateWaylandSurfaceKHR && vkGetPhysicalDeviceWaylandPresentationSupportKHR;
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, wl_display* display, wl_surface* surface) const noexcept;
};
} // namespace platform
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_wayland.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_LINUX
#endif // WISDOM_WAYLAND_H
