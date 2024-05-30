#ifndef WISDOM_XCB_H
#define WISDOM_XCB_H
#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#include <wisdom/vulkan/vk_factory.h>

#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

namespace wis {
namespace platform {
class XCBExtension;
} // namespace platform

template<>
struct Internal<platform::XCBExtension> {
    wis::SharedInstance instance;
    PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;
};

namespace platform {
class XCBExtension : public QueryInternalExtension<XCBExtension, VKFactoryExtensionImpl<platform::XCBExtension>>
{
public:
    static constexpr std::array<const char* const, 1> required_extensions = {
        VK_KHR_XCB_SURFACE_EXTENSION_NAME
    };

    [[nodiscard]] wis::Result Init(const wis::VKFactory& in_instance) noexcept override
    {
        auto& gt = detail::VKFactoryGlobals::Instance().global_table;
        instance = in_instance.GetInternal().factory;

        vkCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(gt.vkGetInstanceProcAddr(instance.get(), "vkCreateXcbSurfaceKHR"));
        vkGetPhysicalDeviceXcbPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(gt.vkGetInstanceProcAddr(instance.get(), "vkGetPhysicalDeviceXcbPresentationSupportKHR"));

        if (!vkCreateXcbSurfaceKHR) {
            return wis::make_result<FUNC, "Failed to load XCB extension functions">(VK_ERROR_FEATURE_NOT_PRESENT);
        }
        return {};
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, xcb_connection_t* connection, xcb_window_t window)const noexcept;
};
} // namespace platform
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_xcb.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_LINUX
#endif // WISDOM_XCB_H