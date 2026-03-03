#ifndef WIS_VK_IMPL_CPP
#define WIS_VK_IMPL_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyBuffer(WisVKBuffer* self)
{
    auto& impl = *reinterpret_cast<VKBufferImpl*>(self);
    if (impl.buffer != VK_NULL_HANDLE) {
        // get allocator
        VmaAllocator allocator = impl.device_header->header.allocator;

        if (impl.mapped_ptr) {
            vmaUnmapMemory(allocator, impl.allocation);
        }
        vmaDestroyBuffer(allocator, impl.buffer, impl.allocation);

        impl.buffer = VK_NULL_HANDLE;

        // Get device from allocator
        VmaAllocatorInfo allocator_info{};
        vmaGetAllocatorInfo(allocator, &allocator_info);

        detail::release_vk_device(allocator_info.device, impl.device_header);
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisVKBufferMap(const WisVKBuffer* self)
{
    auto& impl = *reinterpret_cast<const VKBufferImpl*>(self);
    return impl.mapped_ptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKBufferGetGPUAddress(const WisVKBuffer* self)
{
    auto& impl   = *reinterpret_cast<const VKBufferImpl*>(self);
    auto& header = impl.device_header->header;
    auto& table  = header.device_table;

    VmaAllocator     allocator = impl.device_header->header.allocator;
    VmaAllocatorInfo allocator_info{};
    vmaGetAllocatorInfo(allocator, &allocator_info);

    VkBufferDeviceAddressInfo address_info{
        .sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext  = nullptr,
        .buffer = impl.buffer
    };
    return table.vkGetBufferDeviceAddress(allocator_info.device, &address_info);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyTexture(WisVKTexture* self)
{
    auto& impl = *reinterpret_cast<VKTextureImpl*>(self);
    if (impl.image != VK_NULL_HANDLE) {
        // get allocator
        VmaAllocator allocator = impl.device_header->header.allocator;
        vmaDestroyImage(allocator, impl.image, impl.allocation);

        impl.image = VK_NULL_HANDLE;

        // Get device from allocator
        VmaAllocatorInfo allocator_info{};
        vmaGetAllocatorInfo(allocator, &allocator_info);

        detail::release_vk_device(allocator_info.device, impl.device_header);
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyRootSignature(WisVKRootSignature* self)
{
    auto& impl = *reinterpret_cast<VKRootSignatureImpl*>(self);
    if (impl.root_signature_header != nullptr) {
        delete impl.root_signature_header;
        impl.root_signature_header = nullptr;
    }
}

#endif // WIS_VK_IMPL_CPP
