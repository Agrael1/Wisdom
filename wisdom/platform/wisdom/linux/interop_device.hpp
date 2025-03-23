#ifndef WISDOM_INTEROP_DEVICE_H
#define WISDOM_INTEROP_DEVICE_H
#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_fence.h>

namespace wis {
namespace platform {
class VKInteropDeviceExtensionLinux;
} // namespace platform

template<>
struct Internal<platform::VKInteropDeviceExtensionLinux> {
    wis::SharedDevice device;
    PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR = nullptr;
    PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR = nullptr;
};

namespace platform {
// Device Extension for memory and semaphore interop
class VKInteropDeviceExtensionLinux : public QueryInternalExtension<VKInteropDeviceExtensionLinux, wis::VKDeviceExtension>
{
protected:
    virtual bool
    GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                     std::unordered_set<std::string_view>& ext_name_set,
                     std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                     std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        if (available_extensions.contains(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME)) {
            ext_name_set.emplace(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME);
        }
        if (available_extensions.contains(VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME)) {
            ext_name_set.emplace(VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME);
        }
        return true;
    }

    virtual wis::Result
    Init(const wis::VKDevice& instance,
         const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
         const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        device = instance.GetInternal().device;
        vkGetSemaphoreFdKHR = device.GetDeviceProcAddr<PFN_vkGetSemaphoreFdKHR>("PFN_vkGetSemaphoreFdKHR");
        vkGetMemoryFdKHR = device.GetDeviceProcAddr<PFN_vkGetMemoryFdKHR>("PFN_vkGetMemoryFdKHR");

        // Tell the device that memory and semaphores should support interop
        const_cast<wis::XInternalFeatures&>(instance.GetInternal().ext1.GetInternal().features).interop_device = Supported();
        return {};
    }

public:
    virtual bool Supported() const noexcept override
    {
        return vkGetSemaphoreFdKHR && vkGetMemoryFdKHR;
    }

public:
    [[nodiscard]] WIS_INLINE int
    GetSemaphoreHandle(wis::Result& result, const wis::VKFence& fence) const noexcept
    {
        int handle;
        VkSemaphoreGetFdInfoKHR handle_info{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR,
            .pNext = nullptr,
            .semaphore = fence.GetInternal().fence.get(),
            .handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT
        };
        auto vr = vkGetSemaphoreFdKHR(device.get(), &handle_info, &handle);
        if (!wis::succeeded(vr)) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to get semaphore handle">(vr);
        }
        return handle;
    }
    [[nodiscard]] WIS_INLINE int
    GetMemoryHandle(wis::Result& result, wis::VKMemoryView memory) const noexcept
    {
        int handle;
        auto allocator = std::get<0>(memory);
        auto allocation = std::get<1>(memory);

        VmaAllocatorInfo al_info;
        vmaGetAllocatorInfo(allocator, &al_info);

        VmaAllocationInfo alloc_info;
        vmaGetAllocationInfo(allocator, allocation, &alloc_info);

        VkMemoryGetFdInfoKHR handle_info{
            .sType = VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR,
            .pNext = nullptr,
            .memory = alloc_info.deviceMemory,
            .handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT
        };

        auto vr = vkGetMemoryFdKHR(al_info.device, &handle_info, &handle);
        if (!wis::succeeded(vr)) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to get memory handle">(vr);
        }
        return handle;
    }
};
} // namespace platform
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis::platform {
#if defined(WISDOM_VULKAN)
using InteropDeviceExtension = platform::VKInteropDeviceExtensionLinux;
#endif // WISDOM_VULKAN
} // namespace wis::platform
#endif // !WISDOM_INTEROP_DEVICE_H
