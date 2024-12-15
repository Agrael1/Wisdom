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

class ImplDX12ExtendedAllocation : public QueryInternalExtension<DX12ExtendedAllocation, DX12DeviceExtension>
{
protected:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept override
    {
        D3D12_FEATURE_DATA_D3D12_OPTIONS16 d3d12_options16{};
        auto hr = instance.GetInternal().device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS16, &d3d12_options16, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS16));
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

    [[nodiscard]] WIS_INLINE DX12Texture
    CreateGPUUploadTexture(wis::Result& result, const wis::DX12ResourceAllocator& allocator,
                           wis::TextureDesc desc,
                           wis::TextureState initial_state = wis::TextureState::Common,
                           wis::MemoryFlags flags = wis::MemoryFlags::None) const noexcept;

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
#pragma region DX12ExtendedAllocation

class DX12ExtendedAllocation : public wis::ImplDX12ExtendedAllocation
{
public:
    using wis::ImplDX12ExtendedAllocation::ImplDX12ExtendedAllocation;

public:
    /**
     * @brief Creates a texture that is optimized for GPU upload.
     * Subsequently this texture may be directly mapped and does not require copying through copy queue.
     * The memory behaves as GPU local memory. Requires ReBAR enabled in BIOS.
     * @param allocator The allocator to create the texture with.
     * Allocator must be created by the same device, that initialized this extension.
     * @param desc The description of the texture to create.
     * @param initial_state The initial state of the texture. Default is Common.
     * @param flags The flags of the memory to create the texture with. Default is None.
     * @return wis::DX12Texture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12Texture CreateGPUUploadTexture(wis::Result& result, const wis::DX12ResourceAllocator& allocator, const wis::TextureDesc& desc, wis::TextureState initial_state = wis::TextureState::Common, wis::MemoryFlags flags = wis::MemoryFlags::None) const noexcept
    {
        return wis::ImplDX12ExtendedAllocation::CreateGPUUploadTexture(result, allocator, desc, initial_state, flags);
    }
    /**
     * @brief Creates a texture that is optimized for GPU upload.
     * Subsequently this texture may be directly mapped and does not require copying through copy queue.
     * The memory behaves as GPU local memory. Requires ReBAR enabled in BIOS.
     * @param allocator The allocator to create the texture with.
     * Allocator must be created by the same device, that initialized this extension.
     * @param desc The description of the texture to create.
     * @param initial_state The initial state of the texture. Default is Common.
     * @param flags The flags of the memory to create the texture with. Default is None.
     * @return wis::DX12Texture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Texture> CreateGPUUploadTexture(const wis::DX12ResourceAllocator& allocator, const wis::TextureDesc& desc, wis::TextureState initial_state = wis::TextureState::Common, wis::MemoryFlags flags = wis::MemoryFlags::None) const noexcept
    {
        return wis::ResultValue<wis::DX12Texture> { &wis::ImplDX12ExtendedAllocation::CreateGPUUploadTexture, this, allocator, desc, initial_state, flags };
    }
    /**
     * @brief Writes memory directly to the subresource of the texture.
     * Subresource is array slice or mip level.
     * @param host_data The data to write to the texture.
     * @param dst_texture The texture to write the data to.
     * @param initial_state The initial state of the texture.
     * @param region The region to write the data to.
     * */
    [[nodiscard]] inline wis::Result WriteMemoryToSubresourceDirect(const void* host_data, wis::DX12TextureView dst_texture, wis::TextureState initial_state, wis::TextureRegion region) const noexcept
    {
        return wis::ImplDX12ExtendedAllocation::WriteMemoryToSubresourceDirect(host_data, std::move(dst_texture), initial_state, region);
    }
    /**
     * @brief Check if direct GPU upload is supported for the given format.
     * @param format The format to check.
     * @return true if direct GPU upload is supported.
     * */
    inline bool SupportedDirectGPUUpload(wis::DataFormat format) const noexcept
    {
        return wis::ImplDX12ExtendedAllocation::SupportedDirectGPUUpload(format);
    }
};
#pragma endregion DX12ExtendedAllocation

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

class ImplVKExtendedAllocation : public QueryInternalExtension<VKExtendedAllocation, wis::VKDeviceExtension>
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
        return vkCopyMemoryToImageEXT && vkTransitionImageLayoutEXT && vkGetPhysicalDeviceImageFormatProperties2;
    }

public:
    // may only transition to copy states
    [[nodiscard]] WIS_INLINE wis::VKTexture
    CreateGPUUploadTexture(wis::Result& result, const wis::VKResourceAllocator& allocator,
                           wis::TextureDesc desc,
                           wis::TextureState initial_state = wis::TextureState::Common,
                           wis::MemoryFlags flags = wis::MemoryFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::Result
    WriteMemoryToSubresourceDirect(const void* host_data,
                                   wis::VKTextureView dst_texture,
                                   wis::TextureState initial_state,
                                   wis::TextureRegion region) const noexcept;

    [[nodiscard]] WIS_INLINE bool
    SupportedDirectGPUUpload(wis::DataFormat format) const noexcept;
};
#pragma region VKExtendedAllocation

class VKExtendedAllocation : public wis::ImplVKExtendedAllocation
{
public:
    using wis::ImplVKExtendedAllocation::ImplVKExtendedAllocation;

public:
    /**
     * @brief Creates a texture that is optimized for GPU upload.
     * Subsequently this texture may be directly mapped and does not require copying through copy queue.
     * The memory behaves as GPU local memory. Requires ReBAR enabled in BIOS.
     * @param allocator The allocator to create the texture with.
     * Allocator must be created by the same device, that initialized this extension.
     * @param desc The description of the texture to create.
     * @param initial_state The initial state of the texture. Default is Common.
     * @param flags The flags of the memory to create the texture with. Default is None.
     * @return wis::VKTexture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKTexture CreateGPUUploadTexture(wis::Result& result, const wis::VKResourceAllocator& allocator, const wis::TextureDesc& desc, wis::TextureState initial_state = wis::TextureState::Common, wis::MemoryFlags flags = wis::MemoryFlags::None) const noexcept
    {
        return wis::ImplVKExtendedAllocation::CreateGPUUploadTexture(result, allocator, desc, initial_state, flags);
    }
    /**
     * @brief Creates a texture that is optimized for GPU upload.
     * Subsequently this texture may be directly mapped and does not require copying through copy queue.
     * The memory behaves as GPU local memory. Requires ReBAR enabled in BIOS.
     * @param allocator The allocator to create the texture with.
     * Allocator must be created by the same device, that initialized this extension.
     * @param desc The description of the texture to create.
     * @param initial_state The initial state of the texture. Default is Common.
     * @param flags The flags of the memory to create the texture with. Default is None.
     * @return wis::VKTexture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKTexture> CreateGPUUploadTexture(const wis::VKResourceAllocator& allocator, const wis::TextureDesc& desc, wis::TextureState initial_state = wis::TextureState::Common, wis::MemoryFlags flags = wis::MemoryFlags::None) const noexcept
    {
        return wis::ResultValue<wis::VKTexture> { &wis::ImplVKExtendedAllocation::CreateGPUUploadTexture, this, allocator, desc, initial_state, flags };
    }
    /**
     * @brief Writes memory directly to the subresource of the texture.
     * Subresource is array slice or mip level.
     * @param host_data The data to write to the texture.
     * @param dst_texture The texture to write the data to.
     * @param initial_state The initial state of the texture.
     * @param region The region to write the data to.
     * */
    [[nodiscard]] inline wis::Result WriteMemoryToSubresourceDirect(const void* host_data, wis::VKTextureView dst_texture, wis::TextureState initial_state, wis::TextureRegion region) const noexcept
    {
        return wis::ImplVKExtendedAllocation::WriteMemoryToSubresourceDirect(host_data, std::move(dst_texture), initial_state, region);
    }
    /**
     * @brief Check if direct GPU upload is supported for the given format.
     * @param format The format to check.
     * @return true if direct GPU upload is supported.
     * */
    inline bool SupportedDirectGPUUpload(wis::DataFormat format) const noexcept
    {
        return wis::ImplVKExtendedAllocation::SupportedDirectGPUUpload(format);
    }
};
#pragma endregion VKExtendedAllocation

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
