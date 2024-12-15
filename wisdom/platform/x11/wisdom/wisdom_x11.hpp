#ifndef WISDOM_X11_H
#define WISDOM_X11_H
#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#include <wisdom/vulkan/vk_factory.h>

#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

// couple of undefs to avoid conflicts
#undef Bool
#undef Status
#undef True
#undef False
#undef None
#undef Always

namespace wis {
namespace platform {
class X11Extension;
} // namespace platform

template<>
struct Internal<platform::X11Extension> {
    wis::SharedInstance instance;
    PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR = nullptr;
};

namespace platform {
class X11Extension : public QueryInternalExtension<X11Extension, VKFactoryExtensionImpl<platform::X11Extension>>
{
public:
    static constexpr std::array<const char* const, 1> required_extensions = {
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME
    };

    [[nodiscard]] wis::Result
    Init(const wis::VKFactory& in_instance) noexcept override
    {
        instance = in_instance.GetInternal().factory;
        vkCreateXlibSurfaceKHR = instance.GetInstanceProcAddr<PFN_vkCreateXlibSurfaceKHR>("vkCreateXlibSurfaceKHR");
        vkGetPhysicalDeviceXlibPresentationSupportKHR = instance.GetInstanceProcAddr<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>("vkGetPhysicalDeviceXlibPresentationSupportKHR");
        return {};
    }

    [[nodiscard]] bool
    Supported() const noexcept override
    {
        return vkCreateXlibSurfaceKHR && vkGetPhysicalDeviceXlibPresentationSupportKHR;
    }

public:
    [[nodiscard]] WIS_INLINE wis::VKSwapChain
    CreateSwapchain(wis::Result& result, const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, Display* display, Window window) const noexcept;

    [[nodiscard]] inline wis::ResultValue<wis::VKSwapChain>
        CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, Display* display, Window window) const noexcept
    {
        auto fptr = static_cast<wis::VKSwapChain (X11Extension::*)(wis::Result&, const VKDevice&, VKQueueView, const wis::SwapchainDesc*, Display*, Window) const noexcept>(&X11Extension::CreateSwapchain);
        return wis::ResultValue<wis::VKSwapChain>::from_member_func(fptr, this, device, main_queue, desc, display, window);
    }
};
} // namespace platform
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_x11.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_LINUX
#endif // WISDOM_XCB_H
