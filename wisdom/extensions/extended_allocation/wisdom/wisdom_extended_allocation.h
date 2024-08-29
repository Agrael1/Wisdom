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
    wis::com_ptr<ID3D12Device10> device;
};

class DX12ExtendedAllocation : public QueryInternalExtension<DX12ExtendedAllocation, DX12DeviceExtension>
{
protected:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept override
    {
        device = instance.GetInternal().device.get();
        auto spAdapter = instance.GetInternal().adapter.as<IDXGIAdapter3>();

        D3D12_FEATURE_DATA_D3D12_OPTIONS16 d3d12_options16{};
        auto hr = device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS16, &d3d12_options16, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS16));
        supports_gpu_upload = wis::succeeded(hr) && d3d12_options16.GPUUploadHeapSupported;
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
    CreateGPUUploadTexture(const wis::DX12ResourceAllocator& allocator,
                           wis::TextureDesc desc,
                           wis::MemoryFlags flags) const noexcept;

    [[nodiscard]] WIS_INLINE wis::Result
    WriteMemoryToSubresourceDirect(const void* host_data,
                                   wis::DX12TextureView dst_texture,
                                   wis::TextureState initial_state,
                                   wis::TextureRegion region) const noexcept;

    [[nodiscard]] bool SupportedDirectGPUUpload(wis::DataFormat) const noexcept
    {
        return supports_gpu_upload;
    }
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
    h::VkPhysicalDevice adapter;
    PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT = nullptr;
    PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT = nullptr;
    PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2 = nullptr;
};

class VKExtendedAllocation : public QueryInternalExtension<VKExtendedAllocation, wis::VKDeviceExtension>
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
        return vkCopyMemoryToImageEXT;
    }

public:
    // may only transition to copy states
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKTexture>
    CreateGPUUploadTexture(const wis::VKResourceAllocator& allocator,
                           wis::TextureDesc desc,
                           wis::MemoryFlags flags) const noexcept;

    [[nodiscard]] WIS_INLINE wis::Result
    WriteMemoryToSubresourceDirect(const void* host_data,
                                   wis::VKTextureView dst_texture,
                                   wis::TextureState initial_state,
                                   wis::TextureRegion region) const noexcept;

    [[nodiscard]] WIS_INLINE bool
    SupportedDirectGPUUpload(wis::DataFormat format) const noexcept;
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
#include "impl/impl.dx.cpp"
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_EXTENDED_ALLOCATION_H
