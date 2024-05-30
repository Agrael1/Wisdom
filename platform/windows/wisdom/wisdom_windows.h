#pragma once
#if defined(WISDOM_WINDOWS)
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/dx12_factory_ext.h>

namespace wis {
namespace platform {
class DX12WindowsExtension;
} // namespace platform

template<>
struct Internal<platform::DX12WindowsExtension> : public DX12FactoryExtension {
};

namespace platform {
class DX12WindowsExtension : public QueryInternal<DX12WindowsExtension>
{
public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12SwapChain>
    CreateSwapchain(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12SwapChain>
    CreateSwapchainUWP(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, IUnknown* window) noexcept;
};
} // namespace platform
} // namespace wis

#endif // WISDOM_DX12


#if defined(WISDOM_VULKAN)
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#undef VK_USE_PLATFORM_WIN32_KHR

#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_factory_ext.h>

namespace wis {
namespace platform {
class VKWindowsExtension;
} // namespace platform

template<>
struct Internal<platform::VKWindowsExtension> : public VKFactoryExtension {
    wis::SharedInstance instance;
    PFN_vkCreateWin32SurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;

public:
    [[nodiscard]] wis::Result Init(const wis::VKFactory& in_instance) noexcept override
    {
        auto& gt = detail::VKFactoryGlobals::Instance().global_table;
        instance = in_instance.GetInternal().factory;

        vkCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(gt.vkGetInstanceProcAddr(instance.get(), "vkCreateWin32SurfaceKHR"));
        vkGetPhysicalDeviceXcbPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(gt.vkGetInstanceProcAddr(instance.get(), "vkGetPhysicalDeviceWin32PresentationSupportKHR"));

        if (!vkCreateXcbSurfaceKHR) {
            return wis::make_result<FUNC, "Failed to load Win32 extension functions">(VK_ERROR_FEATURE_NOT_PRESENT);
        }
        return {};
    }
};

namespace platform {
class VKWindowsExtension : public QueryInternal<VKWindowsExtension>
{
public:
    static constexpr std::array<const char* const, 1> required_extensions = {
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) noexcept;
};
} // namespace platform
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis::platform {
#if defined(WISDOM_DX12) && !WISDOM_FORCE_VULKAN
using WindowsExtension = platform::DX12WindowsExtension;
#elif defined(WISDOM_VULKAN)
using WindowsExtension = platform::VKWindowsExtension;
#endif // WISDOM_DX12
} // namespace wis::platform

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_windows.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY

#endif // WISDOM_WINDOWS
