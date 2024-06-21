#ifndef WISDOM_EXTENDED_ALLOCATION_H
#define WISDOM_EXTENDED_ALLOCATION_H
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>

namespace wis {
class DX12ExtendedAllocation;

template<>
struct Internal<DX12ExtendedAllocation> {
    bool supports_gpu_upload = false;
};

class DX12ExtendedAllocation : public QueryInternalExtension<DX12ExtendedAllocation, DX12DeviceExtension>
{
protected:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept override
    {
        auto device = instance.GetInternal().device.get();

        D3D12_FEATURE_DATA_D3D12_OPTIONS16 d3d12_options16{};
        auto hr = device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS16, &d3d12_options16, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS16));
        supports_gpu_upload = SUCCEEDED(hr) && d3d12_options16.GPUUploadHeapSupported;
        return {};
    }

public:
    virtual bool Supported() const noexcept override
    {
        return true;
    }

public:
    // may only transition to copy states
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Texture>
    CreateTexture(const wis::DX12ResourceAllocator& allocator,
                  wis::TextureDesc desc,
                  wis::MemoryType memory,
                  wis::MemoryFlags flags) const noexcept;

    [[nodiscard]] WIS_INLINE wis::Result
    WriteMemoryToSubresource(const void* host_data,
                             wis::DX12TextureView dst_texture,
                             wis::TextureState initial_state,
                             wis::TextureRegion region) const noexcept;
};
} // namespace wis

#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_allocator.h>

namespace wis {
class VKExtendedAllocation;

template<>
struct Internal<VKExtendedAllocation> {
    wis::SharedDevice device;
    PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT = nullptr;
    PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT = nullptr;
};

class VKExtendedAllocation : public QueryInternalExtension<VKExtendedAllocation, wis::VKDeviceExtension>
{
protected:
    virtual bool GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash>& available_extensions,
                                  std::unordered_set<std::string_view>& ext_name_set,
                                  std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                  std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        if (available_extensions.contains(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)) {
            ext_name_set.emplace(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME);
            ext_name_set.emplace(VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME); // Required for VK_EXT_HOST_IMAGE_COPY
            ext_name_set.emplace(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME); // Required for VK_EXT_HOST_IMAGE_COPY

            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT] = sizeof(VkPhysicalDeviceHostImageCopyFeaturesEXT);
            property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceHostImageCopyPropertiesEXT);
            return true;
        }
        return false;
    }
    virtual wis::Result Init(const wis::VKDevice& instance,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        if (!structure_map.contains(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT)) {
            return {};
        }

        VkPhysicalDeviceHostImageCopyFeaturesEXT& host_image_copy_features = *reinterpret_cast<VkPhysicalDeviceHostImageCopyFeaturesEXT*>(structure_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT));
        VkPhysicalDeviceHostImageCopyPropertiesEXT& host_image_copy_properties = *reinterpret_cast<VkPhysicalDeviceHostImageCopyPropertiesEXT*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES_EXT));

        auto& gtable = detail::VKFactoryGlobals::Instance().global_table;
        auto& device_i = instance.GetInternal();
        auto& dtable = device_i.device.table();
        auto hdevice = device_i.device.get();

        device = device_i.device;

        vkCopyMemoryToImageEXT = reinterpret_cast<PFN_vkCopyMemoryToImageEXT>(gtable.vkGetDeviceProcAddr(hdevice, "vkCopyMemoryToImageEXT"));
        vkTransitionImageLayoutEXT = reinterpret_cast<PFN_vkTransitionImageLayoutEXT>(gtable.vkGetDeviceProcAddr(hdevice, "vkTransitionImageLayoutEXT"));
        return {};
    }

public:
    virtual bool Supported() const noexcept
    {
        return vkCopyMemoryToImageEXT != nullptr;
    }

public:
    // may only transition to copy states
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKTexture>
    CreateTexture(const wis::VKResourceAllocator& allocator,
                  wis::TextureDesc desc,
                  wis::MemoryType memory,
                  wis::MemoryFlags flags) const noexcept;

    [[nodiscard]] WIS_INLINE wis::Result
    WriteMemoryToSubresource(const void* host_data,
                             wis::VKTextureView dst_texture,
                             wis::TextureState initial_state,
                             const wis::BufferTextureCopyRegion* regions,
                             uint32_t region_count) const noexcept;
};
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using ExtendedAllocation = DX12ExtendedAllocation;
#elif defined(WISDOM_VULKAN)
using ExtendedAllocation = VKExtendedAllocation;
#endif // WISDOM_DX12
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_extended_allocation.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_EXTENDED_ALLOCATION_H
