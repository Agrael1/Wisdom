#ifndef WISDOM_WINDOWS_H
#define WISDOM_WINDOWS_H
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/dx12_factory_ext.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_device_ext.h>
#include <wisdom/dx12/dx12_device.h>

namespace wis {
namespace platform {
class DX12WindowsExtension;
class DX12InteropDeviceExtension;
} // namespace platform

template<>
struct Internal<platform::DX12WindowsExtension> {
};
template<>
struct Internal<platform::DX12InteropDeviceExtension> {
    wis::com_ptr<ID3D12Device> device;
};

namespace platform {
class DX12WindowsExtension : public QueryInternalExtension<DX12WindowsExtension, DX12FactoryExtension>
{
public:
    [[nodiscard]] WIS_INLINE wis::DX12SwapChain
    CreateSwapchain(wis::Result& result, const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) const noexcept;

    [[nodiscard]] inline wis::ResultValue<DX12SwapChain>
    CreateSwapchain(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) const noexcept
    {
        auto fptr = static_cast<wis::DX12SwapChain (DX12WindowsExtension::*)(wis::Result&, const DX12Device&, DX12QueueView, const wis::SwapchainDesc*, HWND) const noexcept>(&DX12WindowsExtension::CreateSwapchain);
        return wis::ResultValue<DX12SwapChain>::from_member_func(fptr, this, device, main_queue, desc, hwnd);
    }

    [[nodiscard]] WIS_INLINE wis::DX12SwapChain
    CreateSwapchainUWP(wis::Result& result, const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, IUnknown* window) const noexcept;
    [[nodiscard]] inline wis::ResultValue<DX12SwapChain>
    CreateSwapchainUWP(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, IUnknown* window) const noexcept
    {
        auto fptr = static_cast<wis::DX12SwapChain (DX12WindowsExtension::*)(wis::Result&, const DX12Device&, DX12QueueView, const wis::SwapchainDesc*, IUnknown*) const noexcept>(&DX12WindowsExtension::CreateSwapchainUWP);
        return wis::ResultValue<DX12SwapChain>::from_member_func(fptr, this, device, main_queue, desc, window);
    }
};

class DX12InteropDeviceExtension : public QueryInternalExtension<DX12InteropDeviceExtension, DX12DeviceExtension>
{
protected:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept
    {
        device = instance.GetInternal().device;
        return {};
    }

public:
    [[nodiscard]] WIS_INLINE HANDLE
    GetSemaphoreHandle(wis::Result& result, const wis::DX12Fence& fence) const noexcept
    {
        HANDLE handle;
        auto hr = device->CreateSharedHandle(fence.GetInternal().fence.get(), nullptr, GENERIC_ALL, nullptr, &handle);

        if (!wis::succeeded(hr)) {
            result = wis::make_result<FUNC, "Failed to create shared handle for fence">(hr);
        }
        return handle;
    }

    [[nodiscard]] WIS_INLINE HANDLE
    GetMemoryHandle(wis::Result& result, wis::DX12MemoryView memory) const noexcept
    {
        auto allocation = std::get<1>(memory);

        HANDLE handle;
        auto hr = device->CreateSharedHandle(allocation->GetHeap(), nullptr, GENERIC_ALL, nullptr, &handle);

        if (!wis::succeeded(hr)) {
            result = wis::make_result<FUNC, "Failed to create shared handle for memory allocation">(hr);
        }
        return handle;
    }
};
} // namespace platform
} // namespace wis

#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#include <vulkan/vulkan_win32.h>

namespace wis {
namespace platform {
class VKWindowsExtension;
class VKInteropDeviceExtension;
} // namespace platform

template<>
struct Internal<platform::VKWindowsExtension> {
    wis::SharedInstance instance;
    PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
};
template<>
struct Internal<platform::VKInteropDeviceExtension> {
    wis::SharedDevice device;
    PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR = nullptr;
    PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR = nullptr;
    // PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR = nullptr; //later
};

namespace platform {
class VKWindowsExtension : public QueryInternalExtension<VKWindowsExtension, wis::VKFactoryExtensionImpl<VKWindowsExtension>>
{
public:
    static constexpr std::array<const char* const, 1> required_extensions = {
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    [[nodiscard]] wis::Result
    Init(const wis::VKFactory& in_instance) noexcept override
    {
        instance = in_instance.GetInternal().factory;
        vkCreateWin32SurfaceKHR = instance.GetInstanceProcAddr<PFN_vkCreateWin32SurfaceKHR>("vkCreateWin32SurfaceKHR");
        vkGetPhysicalDeviceWin32PresentationSupportKHR = instance.GetInstanceProcAddr<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>("vkGetPhysicalDeviceWin32PresentationSupportKHR");
        return {};
    }

    [[nodiscard]] bool
    Supported() const noexcept override
    {
        return vkCreateWin32SurfaceKHR != nullptr;
    }

public:
    [[nodiscard]] WIS_INLINE wis::VKSwapChain
    CreateSwapchain(wis::Result& result, const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) const noexcept;
    [[nodiscard]] inline wis::ResultValue<wis::VKSwapChain>
    CreateSwapchain(const VKDevice& device, VKQueueView main_queue, const wis::SwapchainDesc* desc, HWND hwnd) const noexcept
    {
        auto fptr = static_cast<wis::VKSwapChain (VKWindowsExtension::*)(wis::Result&, const VKDevice&, VKQueueView, const wis::SwapchainDesc*, HWND) const noexcept>(&VKWindowsExtension::CreateSwapchain);
        return wis::ResultValue<wis::VKSwapChain>::from_member_func(fptr, this, device, main_queue, desc, hwnd);
    }
};

// Device Extension for memory and semaphore interop
class VKInteropDeviceExtension : public QueryInternalExtension<VKInteropDeviceExtension, wis::VKDeviceExtension>
{
protected:
    virtual WIS_INLINE bool
    GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                     std::unordered_set<std::string_view>& ext_name_set,
                     std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                     std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

    virtual WIS_INLINE wis::Result
    Init(const wis::VKDevice& instance,
         const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
         const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

public:
    virtual bool Supported() const noexcept override
    {
        return vkGetSemaphoreWin32HandleKHR && vkGetMemoryWin32HandleKHR;
    }

public:
    [[nodiscard]] WIS_INLINE HANDLE
    GetSemaphoreHandle(wis::Result& result, const wis::VKFence& fence) const noexcept
    {
        HANDLE handle;
        VkSemaphoreGetWin32HandleInfoKHR handle_info{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR,
            .pNext = nullptr,
            .semaphore = fence.GetInternal().fence.get(),
            .handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT
        };
        auto vr = vkGetSemaphoreWin32HandleKHR(device.get(), &handle_info, &handle);
        if (!wis::succeeded(vr)) {
            result = wis::make_result<FUNC, "Failed to get semaphore handle">(vr);
        }
        return handle;
    }
    [[nodiscard]] WIS_INLINE HANDLE
    GetMemoryHandle(wis::Result& result, wis::VKMemoryView memory) const noexcept
    {
        // I know it exists, but platform code is the pain in the ass :(
        extern VMA_CALL_PRE VkResult VMA_CALL_POST vmaGetMemoryWin32Handle(VmaAllocator VMA_NOT_NULL allocator,
                VmaAllocation VMA_NOT_NULL allocation,
                HANDLE hTargetProcess,
                HANDLE * VMA_NOT_NULL pHandle);

        auto allocator = std::get<0>(memory);
        auto allocation = std::get<1>(memory);
        HANDLE handle;
        auto vr = vmaGetMemoryWin32Handle(allocator, allocation, nullptr, &handle);
        if (!wis::succeeded(vr)) {
            result = wis::make_result<FUNC, "Failed to get memory handle">(vr);
        }
        return handle;
    }
};
} // namespace platform
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis::platform {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using WindowsExtension = platform::DX12WindowsExtension;
using InteropDeviceExtension = platform::DX12InteropDeviceExtension;
#elif defined(WISDOM_VULKAN)
using WindowsExtension = platform::VKWindowsExtension;
using InteropDeviceExtension = platform::VKInteropDeviceExtension;
#endif // WISDOM_DX12
} // namespace wis::platform

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_windows.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_WINDOWS_H
