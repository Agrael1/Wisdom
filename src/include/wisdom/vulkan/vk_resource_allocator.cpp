#ifndef WIS_VK_RESOURCE_ALLOCATOR_CPP
#define WIS_VK_RESOURCE_ALLOCATOR_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/util/allocation.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self)
{
    auto& impl = *reinterpret_cast<VKResourceAllocatorImpl*>(self);
    if (impl.allocator) {

        // Get device from allocator
        VmaAllocatorInfo allocator_info{};
        vmaGetAllocatorInfo(impl.allocator, &allocator_info);

        detail::release_vk_device(allocator_info.device, impl.device_header);
        impl.allocator = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKResourceAllocatorCreateBuffer(const WisVKResourceAllocator* self,
                                                                     const WisBufferDesc*          desc,
                                                                     WisVKBuffer*                  buffer)
{
    auto& allocator = *reinterpret_cast<const VKResourceAllocatorImpl*>(self);

    VkBufferCreateInfo buffer_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size  = aligned_size(desc->size_bytes, 265u), // align to uniform buffer alignment for safety
        .usage = (VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | convert_vk(desc->usage_flags)),
    };

    VmaAllocationCreateFlags flags = convert_vk(desc->memory_flags);
    if (desc->memory_flags & WisMemoryFlagsMapped) {
        switch (desc->memory_type) {
        case WisMemoryTypeUpload:
        case WisMemoryTypeGPUUpload:
            flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
            break;
        case WisMemoryTypeReadback:
            flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
            break;
        default:
            flags &= ~VMA_ALLOCATION_CREATE_MAPPED_BIT;
            break;
        }
    }

    VmaAllocationCreateInfo alloc_info{
        .flags         = flags,
        .usage         = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = convert_vk(desc->memory_type)
    };
    VkBuffer      buffer_handle     = VK_NULL_HANDLE;
    VmaAllocation allocation_handle = VK_NULL_HANDLE;
    VkResult      vr                = vmaCreateBuffer(
            allocator.allocator,
            &buffer_info,
            &alloc_info,
            &buffer_handle,
            &allocation_handle,
            nullptr);
    if (!succeeded(vr)) {
        return make_result<Func(), "Buffer creation failed">(vr);
    }

    void* mapped_ptr = nullptr;
    if (desc->memory_flags & WisMemoryFlagsMapped) {
        vr = vmaMapMemory(allocator.allocator, allocation_handle, &mapped_ptr);
        if (!succeeded(vr)) {
            vmaDestroyBuffer(allocator.allocator, buffer_handle, allocation_handle);
            return make_result<Func(), "Buffer memory mapping failed">(vr);
        }
    }

    auto& impl         = *new (buffer) VKBufferImpl;
    impl.buffer        = buffer_handle;
    impl.mapped_ptr    = mapped_ptr;
    impl.allocation    = allocation_handle;
    impl.device_header = allocator.device_header;
    impl.device_header->AddRef();

    return vk_success;
}
#endif // WIS_VK_RESOURCE_ALLOCATOR_CPP
