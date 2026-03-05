#ifndef WIS_VK_DESCRIPTOR_HEAP_CPP
#define WIS_VK_DESCRIPTOR_HEAP_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <bit>

using namespace wis;



//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyDescriptorHeap(WisVKDescriptorHeap* self)
{
    auto& impl = *reinterpret_cast<wis::impl::VKDescriptorHeapImpl*>(self);
    if (impl.buffer) {
        // Destroy buffer
        if (impl.gpu_address == 0) {
            std::free(impl.buffer);
        } else {
            vmaUnmapMemory(impl.device_header->header.allocator, impl.allocation);
            vmaDestroyBuffer(impl.device_header->header.allocator, impl.buffer, impl.allocation);
        }

        detail::release_vk_device(impl.device, impl.device_header);
        impl.buffer = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisVKDescriptorHeapGetCPUHandle(const WisVKDescriptorHeap* self)
{
    auto& heap = *reinterpret_cast<const wis::impl::VKDescriptorHeapImpl*>(self);
    return heap.mapped_ptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteConstantBuffer(const WisVKDescriptorHeap*      self,
                                                                         const WisConstantBufferBinding* data,
                                                                         uint32_t                        index)
{
    auto& heap  = *reinterpret_cast<const wis::impl::VKDescriptorHeapImpl*>(self);
    auto& table = heap.device_header->header.device_table;

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size    = heap.descriptor_size,
    };
    VkDeviceAddressRangeEXT address_range{
        .address = data->buffer_address,
        .size    = data->size_bytes,
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .data  = { .pAddressRange = &address_range }
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to write resource descriptor for constant buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteStructuredBuffer(const WisVKDescriptorHeap* self,
                                                                           WisVKBufferView            buffer,
                                                                           const WisBufferBinding*    data,
                                                                           uint32_t                   index)
{
    auto& heap      = *reinterpret_cast<const wis::impl::VKDescriptorHeapImpl*>(self);
    auto  vk_buffer = std::bit_cast<VkBuffer>(buffer);
    auto& table     = heap.device_header->header.device_table;

    // Get device address of the buffer view
    VkBufferDeviceAddressInfo address_info{
        .sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext  = nullptr,
        .buffer = vk_buffer
    };
    auto address = table.vkGetBufferDeviceAddress(heap.device, &address_info);

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size    = heap.descriptor_size,
    };
    VkDeviceAddressRangeEXT address_range{
        .address = address + data->array_offset * data->stride_bytes,
        .size    = static_cast<VkDeviceSize>(data->stride_bytes) * data->structure_count,
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .data  = { .pAddressRange = &address_range }
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to write resource descriptor for storage buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteRWStructuredBuffer(const WisVKDescriptorHeap* self,
                                                                             WisVKBufferView            buffer,
                                                                             const WisBufferBinding*    data,
                                                                             uint32_t                   index)
{
    return wisVKDescriptorHeapWriteStructuredBuffer(self, buffer, data, index); // For Vulkan, RWStructuredBuffer is the same as StructuredBuffer with storage buffer descriptor type
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteSampler(const WisVKDescriptorHeap* self,
                                                                  const WisSamplerDesc*      sampler,
                                                                  uint32_t                   index)
{
    auto& heap  = *reinterpret_cast<const wis::impl::VKDescriptorHeapImpl*>(self);
    auto& table = heap.device_header->header.device_table;

    float normalized_anisotropy = std::max(float(sampler->max_anisotropy - 1) / 16.f, 0.0f);

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size    = heap.descriptor_size,
    };
    VkSamplerCreateInfo sampler_info{
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                   = nullptr, // no support for custom border color for now
        .flags                   = 0,
        .magFilter               = wis::detail::convert_vk(sampler->mag_filter),
        .minFilter               = wis::detail::convert_vk(sampler->min_filter),
        .mipmapMode              = VkSamplerMipmapMode(sampler->mip_filter),
        .addressModeU            = wis::detail::convert_vk(sampler->address_u),
        .addressModeV            = wis::detail::convert_vk(sampler->address_v),
        .addressModeW            = wis::detail::convert_vk(sampler->address_w),
        .mipLodBias              = sampler->mip_lod_bias,
        .anisotropyEnable        = sampler->is_anisotropic,
        .maxAnisotropy           = normalized_anisotropy,
        .compareEnable           = sampler->comparison_op != WisCompareOperationNever,
        .compareOp               = wis::detail::convert_vk(sampler->comparison_op),
        .minLod                  = sampler->min_lod,
        .maxLod                  = sampler->max_lod,
        .borderColor             = wis::detail::convert_vk(sampler->static_border_color),
        .unnormalizedCoordinates = sampler->flags & WisSamplerFlagsNonNormalizedCoordinates ? VK_TRUE : VK_FALSE,
    };
    VkResult result = table.vkWriteSamplerDescriptorsEXT(heap.device, 1, &sampler_info, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to write resource descriptor for storage buffer view">(result);
    }
    return wis::detail::vk_success;
}

#endif // WIS_VK_DESCRIPTOR_HEAP_CPP
