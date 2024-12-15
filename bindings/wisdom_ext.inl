#include "wisdom.h"

#include "wisdom/wisdom_debug.h"
#include "wisdom/wisdom_descriptor_buffer.h"
#include "wisdom/wisdom_extended_allocation.h"

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_dx12.hpp>

template<wis::FactoryExtID>
struct DX12FactoryExtensionMap {
    using Type = wis::DX12FactoryExtension;
};
template<>
struct DX12FactoryExtensionMap<wis::FactoryExtID::DebugExtension> {
    using Type = wis::DX12DebugExtension;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) DX12FactoryExtensionBridge(wis::FactoryExtID id, Args&&... args)
{
    switch (id) {
    case wis::FactoryExtID::DebugExtension:
        return Executor<typename DX12FactoryExtensionMap<wis::FactoryExtID::DebugExtension>::Type>{}(std::forward<Args>(args)...);
    default:
        return Executor<wis::DX12FactoryExtension>{}(std::forward<Args>(args)...);
    }
}

//-------------------------------------------------------------------------

template<wis::DeviceExtID>
struct DX12DeviceExtensionMap {
    using Type = wis::DX12DeviceExtension;
};
template<>
struct DX12DeviceExtensionMap<wis::DeviceExtID::DescriptorBufferExtension> {
    using Type = wis::DX12DescriptorBufferExtension;
};
template<>
struct DX12DeviceExtensionMap<wis::DeviceExtID::ExtendedAllocation> {
    using Type = wis::DX12ExtendedAllocation;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) DX12DeviceExtensionBridge(wis::DeviceExtID id, Args&&... args)
{
    switch (id) {
    case wis::DeviceExtID::DescriptorBufferExtension:
        return Executor<typename DX12DeviceExtensionMap<wis::DeviceExtID::DescriptorBufferExtension>::Type>{}(std::forward<Args>(args)...);
    case wis::DeviceExtID::ExtendedAllocation:
        return Executor<typename DX12DeviceExtensionMap<wis::DeviceExtID::ExtendedAllocation>::Type>{}(std::forward<Args>(args)...);
    default:
        return Executor<wis::DX12DeviceExtension>{}(std::forward<Args>(args)...);
    }
}
// DX12DebugExtension methods --
extern "C" WisResult DX12DebugExtensionCreateDebugMessenger(DX12DebugExtension self, DebugCallback callback, void* user_data, DX12DebugMessenger* messenger)
{
    auto* xself = reinterpret_cast<wis::DX12DebugExtension*>(self);
    auto&& [res, value] = xself->CreateDebugMessenger(reinterpret_cast<wis::DebugCallback>(callback), user_data);

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *messenger = reinterpret_cast<DX12DebugMessenger>(new (std::nothrow) wis::DX12DebugMessenger(std::move(value)));
    if (!*messenger) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12DebugMessenger." };
    }
    return reinterpret_cast<WisResult&>(res);
}
// DX12DescriptorBufferExtension methods --
extern "C" WisResult DX12DescriptorBufferExtensionCreateRootSignature(DX12DescriptorBufferExtension self, const WisPushConstant* root_constants, uint32_t constants_count, const WisPushDescriptor* root_descriptors, uint32_t descriptors_count, const WisDescriptorTable* tables, uint32_t tables_count, DX12RootSignature* signature)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBufferExtension*>(self);
    auto&& [res, value] = xself->CreateRootSignature(reinterpret_cast<const wis::PushConstant*&>(root_constants), constants_count, reinterpret_cast<const wis::PushDescriptor*&>(root_descriptors), descriptors_count, reinterpret_cast<const wis::DescriptorTable*&>(tables), tables_count);

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *signature = reinterpret_cast<DX12RootSignature>(new (std::nothrow) wis::DX12RootSignature(std::move(value)));
    if (!*signature) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12RootSignature." };
    }
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12DescriptorBufferExtensionCreateDescriptorBuffer(DX12DescriptorBufferExtension self, WisDescriptorHeapType type, WisDescriptorMemory memory, uint64_t memory_bytes, DX12DescriptorBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBufferExtension*>(self);
    auto&& [res, value] = xself->CreateDescriptorBuffer(static_cast<wis::DescriptorHeapType>(type), static_cast<wis::DescriptorMemory>(memory), memory_bytes);

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *buffer = reinterpret_cast<DX12DescriptorBuffer>(new (std::nothrow) wis::DX12DescriptorBuffer(std::move(value)));
    if (!*buffer) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12DescriptorBuffer." };
    }
    return reinterpret_cast<WisResult&>(res);
}
extern "C" uint64_t DX12DescriptorBufferExtensionGetDescriptorTableAlignment(DX12DescriptorBufferExtension self, WisDescriptorHeapType type)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBufferExtension*>(self);
    auto res = xself->GetDescriptorTableAlignment(static_cast<wis::DescriptorHeapType>(type));
    ;
    return res;
}
extern "C" uint64_t DX12DescriptorBufferExtensionGetDescriptorSize(DX12DescriptorBufferExtension self, WisDescriptorHeapType type)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBufferExtension*>(self);
    auto res = xself->GetDescriptorSize(static_cast<wis::DescriptorHeapType>(type));
    ;
    return res;
}
extern "C" void DX12DescriptorBufferExtensionSetDescriptorBuffers(DX12DescriptorBufferExtension self, DX12CommandList list, DX12DescriptorBuffer resource_buffer, DX12DescriptorBuffer sampler_buffer)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBufferExtension*>(self);
    xself->SetDescriptorBuffers(*reinterpret_cast<wis::DX12CommandList*>(list), *reinterpret_cast<wis::DX12DescriptorBuffer*>(resource_buffer), *reinterpret_cast<wis::DX12DescriptorBuffer*>(sampler_buffer));
}
extern "C" void DX12DescriptorBufferExtensionSetDescriptorTableOffset(DX12DescriptorBufferExtension self, DX12CommandList list, DX12RootSignature root_signature, uint32_t table_index, DX12DescriptorBuffer buffer, uint32_t table_aligned_byte_offset)
{
    auto* xself = reinterpret_cast<wis::DX12DescriptorBufferExtension*>(self);
    xself->SetDescriptorTableOffset(*reinterpret_cast<wis::DX12CommandList*>(list), *reinterpret_cast<wis::DX12RootSignature*>(root_signature), table_index, *reinterpret_cast<wis::DX12DescriptorBuffer*>(buffer), table_aligned_byte_offset);
}
// DX12ExtendedAllocation methods --
extern "C" WisResult DX12ExtendedAllocationCreateGPUUploadTexture(DX12ExtendedAllocation self, const DX12ResourceAllocator* allocator, const WisTextureDesc* desc, WisTextureState initial_state, WisMemoryFlags flags, DX12Texture* texture)
{
    auto* xself = reinterpret_cast<wis::DX12ExtendedAllocation*>(self);
    auto&& [res, value] = xself->CreateGPUUploadTexture(*reinterpret_cast<const wis::DX12ResourceAllocator*>(allocator), *reinterpret_cast<const wis::TextureDesc*>(desc), static_cast<wis::TextureState>(initial_state), static_cast<wis::MemoryFlags>(flags));

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *texture = reinterpret_cast<DX12Texture>(new (std::nothrow) wis::DX12Texture(std::move(value)));
    if (!*texture) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12Texture." };
    }
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult DX12ExtendedAllocationWriteMemoryToSubresourceDirect(DX12ExtendedAllocation self, const void* host_data, DX12Texture dst_texture, WisTextureState initial_state, WisTextureRegion region)
{
    auto* xself = reinterpret_cast<wis::DX12ExtendedAllocation*>(self);
    auto res = xself->WriteMemoryToSubresourceDirect(host_data, *reinterpret_cast<wis::DX12Texture*>(dst_texture), static_cast<wis::TextureState>(initial_state), reinterpret_cast<wis::TextureRegion&>(region));
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" bool DX12ExtendedAllocationSupportedDirectGPUUpload(DX12ExtendedAllocation self, WisDataFormat format)
{
    auto* xself = reinterpret_cast<wis::DX12ExtendedAllocation*>(self);
    auto res = xself->SupportedDirectGPUUpload(static_cast<wis::DataFormat>(format));
    ;
    return res;
}
#endif

#if defined(WISDOM_VULKAN)
#include <wisdom/wisdom_vk.hpp>

template<wis::FactoryExtID>
struct VKFactoryExtensionMap {
    using Type = wis::VKFactoryExtension;
};
template<>
struct VKFactoryExtensionMap<wis::FactoryExtID::DebugExtension> {
    using Type = wis::VKDebugExtension;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) VKFactoryExtensionBridge(wis::FactoryExtID id, Args&&... args)
{
    switch (id) {
    case wis::FactoryExtID::DebugExtension:
        return Executor<typename VKFactoryExtensionMap<wis::FactoryExtID::DebugExtension>::Type>{}(std::forward<Args>(args)...);
    default:
        return Executor<wis::VKFactoryExtension>{}(std::forward<Args>(args)...);
    }
}

//-------------------------------------------------------------------------

template<wis::DeviceExtID>
struct VKDeviceExtensionMap {
    using Type = wis::VKDeviceExtension;
};
template<>
struct VKDeviceExtensionMap<wis::DeviceExtID::DescriptorBufferExtension> {
    using Type = wis::VKDescriptorBufferExtension;
};
template<>
struct VKDeviceExtensionMap<wis::DeviceExtID::ExtendedAllocation> {
    using Type = wis::VKExtendedAllocation;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) VKDeviceExtensionBridge(wis::DeviceExtID id, Args&&... args)
{
    switch (id) {
    case wis::DeviceExtID::DescriptorBufferExtension:
        return Executor<typename VKDeviceExtensionMap<wis::DeviceExtID::DescriptorBufferExtension>::Type>{}(std::forward<Args>(args)...);
    case wis::DeviceExtID::ExtendedAllocation:
        return Executor<typename VKDeviceExtensionMap<wis::DeviceExtID::ExtendedAllocation>::Type>{}(std::forward<Args>(args)...);
    default:
        return Executor<wis::VKDeviceExtension>{}(std::forward<Args>(args)...);
    }
}
// VKDebugExtension methods --
extern "C" WisResult VKDebugExtensionCreateDebugMessenger(VKDebugExtension self, DebugCallback callback, void* user_data, VKDebugMessenger* messenger)
{
    auto* xself = reinterpret_cast<wis::VKDebugExtension*>(self);
    auto&& [res, value] = xself->CreateDebugMessenger(reinterpret_cast<wis::DebugCallback>(callback), user_data);

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *messenger = reinterpret_cast<VKDebugMessenger>(new (std::nothrow) wis::VKDebugMessenger(std::move(value)));
    if (!*messenger) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKDebugMessenger." };
    }
    return reinterpret_cast<WisResult&>(res);
}
// VKDescriptorBufferExtension methods --
extern "C" WisResult VKDescriptorBufferExtensionCreateRootSignature(VKDescriptorBufferExtension self, const WisPushConstant* root_constants, uint32_t constants_count, const WisPushDescriptor* root_descriptors, uint32_t descriptors_count, const WisDescriptorTable* tables, uint32_t tables_count, VKRootSignature* signature)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBufferExtension*>(self);
    auto&& [res, value] = xself->CreateRootSignature(reinterpret_cast<const wis::PushConstant*&>(root_constants), constants_count, reinterpret_cast<const wis::PushDescriptor*&>(root_descriptors), descriptors_count, reinterpret_cast<const wis::DescriptorTable*&>(tables), tables_count);

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *signature = reinterpret_cast<VKRootSignature>(new (std::nothrow) wis::VKRootSignature(std::move(value)));
    if (!*signature) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKRootSignature." };
    }
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKDescriptorBufferExtensionCreateDescriptorBuffer(VKDescriptorBufferExtension self, WisDescriptorHeapType type, WisDescriptorMemory memory, uint64_t memory_bytes, VKDescriptorBuffer* buffer)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBufferExtension*>(self);
    auto&& [res, value] = xself->CreateDescriptorBuffer(static_cast<wis::DescriptorHeapType>(type), static_cast<wis::DescriptorMemory>(memory), memory_bytes);

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *buffer = reinterpret_cast<VKDescriptorBuffer>(new (std::nothrow) wis::VKDescriptorBuffer(std::move(value)));
    if (!*buffer) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKDescriptorBuffer." };
    }
    return reinterpret_cast<WisResult&>(res);
}
extern "C" uint64_t VKDescriptorBufferExtensionGetDescriptorTableAlignment(VKDescriptorBufferExtension self, WisDescriptorHeapType type)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBufferExtension*>(self);
    auto res = xself->GetDescriptorTableAlignment(static_cast<wis::DescriptorHeapType>(type));
    ;
    return res;
}
extern "C" uint64_t VKDescriptorBufferExtensionGetDescriptorSize(VKDescriptorBufferExtension self, WisDescriptorHeapType type)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBufferExtension*>(self);
    auto res = xself->GetDescriptorSize(static_cast<wis::DescriptorHeapType>(type));
    ;
    return res;
}
extern "C" void VKDescriptorBufferExtensionSetDescriptorBuffers(VKDescriptorBufferExtension self, VKCommandList list, VKDescriptorBuffer resource_buffer, VKDescriptorBuffer sampler_buffer)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBufferExtension*>(self);
    xself->SetDescriptorBuffers(*reinterpret_cast<wis::VKCommandList*>(list), *reinterpret_cast<wis::VKDescriptorBuffer*>(resource_buffer), *reinterpret_cast<wis::VKDescriptorBuffer*>(sampler_buffer));
}
extern "C" void VKDescriptorBufferExtensionSetDescriptorTableOffset(VKDescriptorBufferExtension self, VKCommandList list, VKRootSignature root_signature, uint32_t table_index, VKDescriptorBuffer buffer, uint32_t table_aligned_byte_offset)
{
    auto* xself = reinterpret_cast<wis::VKDescriptorBufferExtension*>(self);
    xself->SetDescriptorTableOffset(*reinterpret_cast<wis::VKCommandList*>(list), *reinterpret_cast<wis::VKRootSignature*>(root_signature), table_index, *reinterpret_cast<wis::VKDescriptorBuffer*>(buffer), table_aligned_byte_offset);
}
// VKExtendedAllocation methods --
extern "C" WisResult VKExtendedAllocationCreateGPUUploadTexture(VKExtendedAllocation self, const VKResourceAllocator* allocator, const WisTextureDesc* desc, WisTextureState initial_state, WisMemoryFlags flags, VKTexture* texture)
{
    auto* xself = reinterpret_cast<wis::VKExtendedAllocation*>(self);
    auto&& [res, value] = xself->CreateGPUUploadTexture(*reinterpret_cast<const wis::VKResourceAllocator*>(allocator), *reinterpret_cast<const wis::TextureDesc*>(desc), static_cast<wis::TextureState>(initial_state), static_cast<wis::MemoryFlags>(flags));

    if (res.status != wis::Status::Ok) {
        return reinterpret_cast<WisResult&>(res);
    }

    *texture = reinterpret_cast<VKTexture>(new (std::nothrow) wis::VKTexture(std::move(value)));
    if (!*texture) {
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKTexture." };
    }
    return reinterpret_cast<WisResult&>(res);
}
extern "C" WisResult VKExtendedAllocationWriteMemoryToSubresourceDirect(VKExtendedAllocation self, const void* host_data, VKTexture dst_texture, WisTextureState initial_state, WisTextureRegion region)
{
    auto* xself = reinterpret_cast<wis::VKExtendedAllocation*>(self);
    auto res = xself->WriteMemoryToSubresourceDirect(host_data, *reinterpret_cast<wis::VKTexture*>(dst_texture), static_cast<wis::TextureState>(initial_state), reinterpret_cast<wis::TextureRegion&>(region));
    ;
    return reinterpret_cast<WisResult&>(res);
}
extern "C" bool VKExtendedAllocationSupportedDirectGPUUpload(VKExtendedAllocation self, WisDataFormat format)
{
    auto* xself = reinterpret_cast<wis::VKExtendedAllocation*>(self);
    auto res = xself->SupportedDirectGPUUpload(static_cast<wis::DataFormat>(format));
    ;
    return res;
}
#endif
