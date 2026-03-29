#ifndef WIS_VK_DESCRIPTOR_HEAP_CPP
#define WIS_VK_DESCRIPTOR_HEAP_CPP
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>

#include <bit>

namespace wis::detail
{
inline VkImageAspectFlags VKGetAspectFlags(const WisTextureBinding& binding) noexcept
{
    if ((binding.flags & WisTextureBindingFlagsStencilView) &&
        (binding.format == WisDataFormatD24UnormS8Uint || binding.format == WisDataFormatD32FloatS8Uint))
    {
        return VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    if ((binding.flags & WisTextureBindingFlagsDepthView) &&
            (binding.format == WisDataFormatD32FloatS8Uint || binding.format == WisDataFormatD24UnormS8Uint) ||
        (binding.format == WisDataFormatD16Unorm || binding.format == WisDataFormatD32Float))
    {
        return VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    if (binding.range.plane_slice) {
        return VK_IMAGE_ASPECT_PLANE_0_BIT << (binding.range.plane_slice - 1);
    }
    return VK_IMAGE_ASPECT_COLOR_BIT;
}

inline VkImageViewCreateInfo VKGetSRVDesc(const WisTextureBinding& binding) noexcept
{
    VkImageViewCreateInfo srv_desc{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .format = wis::detail::VKConvert(binding.format),
        .components = {
            .r = wis::detail::VKConvert(binding.component_mapping.r),
            .g = wis::detail::VKConvert(binding.component_mapping.g),
            .b = wis::detail::VKConvert(binding.component_mapping.b),
            .a = wis::detail::VKConvert(binding.component_mapping.a),
        },
    };
    auto aspect_flags = VKGetAspectFlags(binding);

    switch (binding.layout) {
    case WisTextureLayoutTexture1D:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_1D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = 0,
            .layerCount = 1
        };
        break;
    case WisTextureLayoutTexture1DArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    case WisTextureLayoutTexture2D:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = 0,
            .layerCount = 1
        };
        break;
    default:
    case WisTextureLayoutTexture2DArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    case WisTextureLayoutTexture3D:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_3D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = 0,
            .layerCount = 1
        };
        break;
    case WisTextureLayoutTexture2DMS:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srv_desc.subresourceRange =
            {.aspectMask = aspect_flags, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1};
        break;
    case WisTextureLayoutTexture2DMSArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    case WisTextureLayoutTextureCube:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = 0,
            .layerCount = 6
        };
        break;
    case WisTextureLayoutTextureCubeArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = binding.range.mip_level_count,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    }
    return srv_desc;
}

inline VkImageViewCreateInfo VKGetUAVDesc(const WisTextureBinding& binding) noexcept
{
    VkImageViewCreateInfo srv_desc{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .format = wis::detail::VKConvert(binding.format),
        .components = {},
    };
    auto aspect_flags = VKGetAspectFlags(binding);

    switch (binding.layout) {
    case WisTextureLayoutTexture1D:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_1D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };
        break;
    case WisTextureLayoutTexture1DArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = 1,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    case WisTextureLayoutTexture2D:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };
        break;
    default:
    case WisTextureLayoutTexture2DArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = 1,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    case WisTextureLayoutTexture3D:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_3D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = binding.range.base_mip_level,
            .levelCount = 1,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    case WisTextureLayoutTexture2DMS:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };
        break;
    case WisTextureLayoutTexture2DMSArray:
        srv_desc.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        srv_desc.subresourceRange = {
            .aspectMask = aspect_flags,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = binding.range.base_array_layer,
            .layerCount = binding.range.array_layer_count
        };
        break;
    }
    return srv_desc;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyDescriptorHeap(WisVKDescriptorHeap* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKDescriptorHeapImpl>(self);
    if (impl.buffer) {
        // Destroy buffer
        if (impl.gpu_address == 0) {
            std::free(impl.buffer);
        } else {
            vmaDestroyBuffer(impl.device_header->header.allocator, impl.buffer, impl.allocation);
        }

        wis::detail::VKReleaseDevice(impl.device_header);
        impl.buffer = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyViewHeap(WisVKViewHeap* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKViewHeapImpl>(self);
    if (impl.view_heap) {
        for (uint32_t i = 0; i < impl.capacity; ++i) {
            if (impl.view_heap[i].view != VK_NULL_HANDLE) {
                impl.device_header->header.device_table
                    .vkDestroyImageView(impl.device_header->header.device, impl.view_heap[i].view, nullptr);
            }
        }

        delete[] impl.view_heap;
        impl.view_heap = nullptr;

        wis::detail::VKReleaseDevice(impl.device_header);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisVKDescriptorHeapGetCPUHandle(const WisVKDescriptorHeap* self)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    return heap.mapped_ptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteConstantBuffer(
    const WisVKDescriptorHeap* self,
    const WisConstantBufferBinding* data,
    uint32_t index
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    auto& table = heap.device_header->header.device_table;

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size = heap.descriptor_size,
    };
    VkDeviceAddressRangeEXT address_range{
        .address = data->buffer_address,
        .size = data->size_bytes,
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .data = {.pAddressRange = &address_range}
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::
            make_result<wis::detail::Func(), "Failed to write resource descriptor for constant buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteStructuredBuffer(
    const WisVKDescriptorHeap* self,
    WisVKBufferView buffer,
    const WisBufferBinding* data,
    uint32_t index
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    auto vk_buffer = std::bit_cast<VkBuffer>(buffer);
    auto& table = heap.device_header->header.device_table;

    // Get device address of the buffer view
    VkBufferDeviceAddressInfo address_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext = nullptr,
        .buffer = vk_buffer
    };
    auto address = table.vkGetBufferDeviceAddress(heap.device, &address_info);

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size = heap.descriptor_size,
    };
    VkDeviceAddressRangeEXT address_range{
        .address = address + data->array_offset * data->stride_bytes,
        .size = static_cast<VkDeviceSize>(data->stride_bytes) * data->structure_count,
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .data = {.pAddressRange = &address_range}
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::
            make_result<wis::detail::Func(), "Failed to write resource descriptor for storage buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDescriptorHeapWriteRWStructuredBuffer(
    const WisVKDescriptorHeap* self,
    WisVKBufferView buffer,
    const WisBufferBinding* data,
    uint32_t index
)
{
    // For Vulkan, RWStructuredBuffer is the same as StructuredBuffer with storage buffer descriptor type
    return wisVKDescriptorHeapWriteStructuredBuffer(self, buffer, data, index);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisVKDescriptorHeapWriteSampler(const WisVKDescriptorHeap* self, const WisSamplerDesc* sampler, uint32_t index)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    auto& table = heap.device_header->header.device_table;

    float normalized_anisotropy = std::max(float(sampler->max_anisotropy - 1) / 16.f, 0.0f);

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size = heap.descriptor_size,
    };
    VkSamplerReductionModeCreateInfo reduction_mode_info{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO,
        .pNext = nullptr, // Custom border?
        .reductionMode = sampler->comparison_op != WisCompareOpNever ? VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE
                                                                     : wis::detail::VKConvert(sampler->reduction_mode)
    };
    VkSamplerCreateInfo sampler_info{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = &reduction_mode_info,
        .flags = 0,
        .magFilter = wis::detail::VKConvert(sampler->mag_filter),
        .minFilter = wis::detail::VKConvert(sampler->min_filter),
        .mipmapMode = VkSamplerMipmapMode(sampler->mip_filter),
        .addressModeU = wis::detail::VKConvert(sampler->address_u),
        .addressModeV = wis::detail::VKConvert(sampler->address_v),
        .addressModeW = wis::detail::VKConvert(sampler->address_w),
        .mipLodBias = sampler->mip_lod_bias,
        .anisotropyEnable = sampler->is_anisotropic,
        .maxAnisotropy = normalized_anisotropy,
        .compareEnable = sampler->comparison_op != WisCompareOpNever,
        .compareOp = wis::detail::VKConvert(sampler->comparison_op),
        .minLod = sampler->min_lod,
        .maxLod = sampler->max_lod,
        .borderColor = wis::detail::VKConvert(sampler->static_border_color),
        .unnormalizedCoordinates = sampler->flags & WisSamplerFlagsNonNormalizedCoordinates ? VK_TRUE : VK_FALSE,
    };
    VkResult result = table.vkWriteSamplerDescriptorsEXT(heap.device, 1, &sampler_info, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::
            make_result<wis::detail::Func(), "Failed to write resource descriptor for storage buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WISDOM_API WisResult wisVKDescriptorHeapWriteTexture(
    const WisVKDescriptorHeap* self,
    WisVKTextureView view,
    const WisTextureBinding* data,
    uint32_t index
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    auto& table = heap.device_header->header.device_table;

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size = heap.descriptor_size,
    };

    VkImageViewCreateInfo view_create_info = wis::detail::VKGetSRVDesc(*data);
    view_create_info.image = std::bit_cast<VkImage>(view);

    VkImageDescriptorInfoEXT image_desc{
        .sType = VK_STRUCTURE_TYPE_IMAGE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .pView = &view_create_info,
        .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, // TODO: Granular layout for depth stencil
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .data = {.pImage = &image_desc}
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::
            make_result<wis::detail::Func(), "Failed to write resource descriptor for constant buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WISDOM_API WisResult wisVKDescriptorHeapWriteRWTexture(
    const WisVKDescriptorHeap* self,
    WisVKTextureView view,
    const WisTextureBinding* data,
    uint32_t index
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    auto& table = heap.device_header->header.device_table;

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size = heap.descriptor_size,
    };

    VkImageViewCreateInfo view_create_info = wis::detail::VKGetUAVDesc(*data);
    view_create_info.image = std::bit_cast<VkImage>(view);

    VkImageDescriptorInfoEXT image_desc{
        .sType = VK_STRUCTURE_TYPE_IMAGE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .pView = &view_create_info,
        .layout = VK_IMAGE_LAYOUT_GENERAL, // TODO: Granular layout for depth stencil
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        .data = {.pImage = &image_desc}
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::
            make_result<wis::detail::Func(), "Failed to write resource descriptor for constant buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisVKDescriptorHeapWriteAccelerationStructure(const WisVKDescriptorHeap* self, uint64_t address, uint32_t index)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    auto& table = heap.device_header->header.device_table;

    VkHostAddressRangeEXT host_range{
        .address = static_cast<uint8_t*>(heap.mapped_ptr) + index * heap.descriptor_size,
        .size = heap.descriptor_size,
    };
    VkDeviceAddressRangeEXT address_range{
        .address = address,
        .size = 0,
    };
    VkResourceDescriptorInfoEXT resource_desc{
        .sType = VK_STRUCTURE_TYPE_RESOURCE_DESCRIPTOR_INFO_EXT,
        .pNext = nullptr,
        .type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
        .data = {.pAddressRange = &address_range}
    };
    VkResult result = table.vkWriteResourceDescriptorsEXT(heap.device, 1, &resource_desc, &host_range);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::
            make_result<wis::detail::Func(), "Failed to write resource descriptor for constant buffer view">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDescriptorHeapCopyDescriptors(
    const WisVKDescriptorHeap* self,
    uint32_t dst_index,
    const void* src_ptr,
    uint32_t src_index,
    uint32_t count
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(self);
    std::memcpy(
        static_cast<uint8_t*>(heap.mapped_ptr) + static_cast<size_t>(dst_index) * heap.descriptor_size,
        static_cast<const uint8_t*>(src_ptr) + static_cast<size_t>(src_index) * heap.descriptor_size,
        static_cast<size_t>(count) * heap.descriptor_size
    );
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKViewHeapWriteRenderTarget(
    const WisVKViewHeap* self,
    const WisVKTexture* texture,
    const WisRenderTargetDesc* render_target,
    uint32_t index
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKViewHeapImpl>(self);
    auto& tex = wis::from_handle_ref<const wis::impl::VKTextureImpl>(texture);
    auto& header = heap.device_header->header;

    // simply create image view
    auto vk_format = wis::detail::VKConvert(render_target->format);
    VkImageViewCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .image = tex.image,
        .format = vk_format,
    };
    info.subresourceRange.aspectMask = wis::detail::VKAspectFlags(vk_format);

    switch (render_target->layout) {
    case WisTextureLayoutTexture1D:
        info.viewType = VK_IMAGE_VIEW_TYPE_1D;
        info.subresourceRange.baseMipLevel = render_target->mip_level;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0, info.subresourceRange.layerCount = 1;
        break;
    case WisTextureLayoutTexture2D:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.subresourceRange.baseMipLevel = render_target->mip_level;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0, info.subresourceRange.layerCount = 1;
        break;
    case WisTextureLayoutTexture3D:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        info.subresourceRange.baseMipLevel = render_target->mip_level;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = render_target->base_array_layer;
        info.subresourceRange.layerCount = render_target->array_layer_count;
        break;
    case WisTextureLayoutTexture1DArray:
        info.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        info.subresourceRange.baseMipLevel = render_target->mip_level;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = render_target->base_array_layer;
        info.subresourceRange.layerCount = render_target->array_layer_count;
        break;
    case WisTextureLayoutTexture2DArray:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        info.subresourceRange.baseMipLevel = render_target->mip_level;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = render_target->base_array_layer;
        info.subresourceRange.layerCount = render_target->array_layer_count;
        break;
    case WisTextureLayoutTexture2DMS:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        break;
    case WisTextureLayoutTexture2DMSArray:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = render_target->base_array_layer;
        info.subresourceRange.layerCount = render_target->array_layer_count;
        break;
    default:
        break;
    }

    // Get at index position in the view heap
    wis::detail::VKRenderTargetView& out_render_target = heap.view_heap[index];
    if (out_render_target.view != VK_NULL_HANDLE) {
        header.device_table.vkDestroyImageView(header.device, out_render_target.view, nullptr);
    }

    VkImageView view = VK_NULL_HANDLE;
    auto vr = header.device_table.vkCreateImageView(header.device, &info, nullptr, &view);
    if (!wis::detail::succeeded(vr)) {
        return 0; // Failed to create image view, return 0 as an invalid handle
    }

    out_render_target = {
        .view = view,
        .width = tex.width,
        .height = tex.height,
        .array_layer_count = tex.depth_or_array_size,
    };

    return std::bit_cast<uint64_t>(&out_render_target);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKViewHeapWriteDepthStencil(
    const WisVKViewHeap* self,
    const WisVKTexture* texture,
    const WisRenderTargetDesc* render_target,
    uint32_t index
)
{
    // For Vulkan, depth stencil view is essentially the same as render target view with different aspect mask, so we
    // can reuse the same function
    return wisVKViewHeapWriteRenderTarget(self, texture, render_target, index);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKViewHeapGetViewAddress(const WisVKViewHeap* self, uint32_t index)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKViewHeapImpl>(self);
    if (index >= heap.capacity) {
        return 0; // Invalid index, return 0 as an invalid handle
    }
    return std::bit_cast<uint64_t>(&heap.view_heap[index]);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKViewHeapCopyViews(
    const WisVKViewHeap* self,
    uint32_t dst_index,
    uint64_t src_ptr,
    uint32_t src_index,
    uint32_t count
)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKViewHeapImpl>(self);
    if (dst_index + count > heap.capacity) {
        return; // Invalid range, do nothing
    }
    auto* src_views = reinterpret_cast<const wis::detail::VKRenderTargetView*>(std::bit_cast<const void*>(src_ptr)) +
                      src_index;
    auto* dst_views = heap.view_heap + dst_index;
    for (uint32_t i = 0; i < count; ++i) {
        // Destroy existing view at destination if it's not null
        if (dst_views[i].view != VK_NULL_HANDLE) {
            heap.device_header->header.device_table
                .vkDestroyImageView(heap.device_header->header.device, dst_views[i].view, nullptr);
        }
        dst_views[i] = src_views[i];
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKViewHeapGetCPUHandle(const WisVKViewHeap* self)
{
    auto& heap = wis::from_handle_ref<const wis::impl::VKViewHeapImpl>(self);
    return std::bit_cast<uint64_t>(heap.view_heap);
}

#endif // WIS_VK_DESCRIPTOR_HEAP_CPP
