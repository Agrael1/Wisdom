#ifndef WIS_VK_IMPL_CPP
#define WIS_VK_IMPL_CPP
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyBuffer(WisVKBuffer* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKBufferImpl>(self);
    if (impl.buffer != VK_NULL_HANDLE) {
        // get allocator
        VmaAllocator allocator = impl.device_header->header.allocator;

        if (impl.mapped_ptr) {
            vmaUnmapMemory(allocator, impl.allocation);
        }
        vmaDestroyBuffer(allocator, impl.buffer, impl.allocation);

        impl.buffer = VK_NULL_HANDLE;

        wis::detail::VKReleaseDevice(impl.device_header);
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisVKBufferMap(const WisVKBuffer* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKBufferImpl>(self);
    return impl.mapped_ptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKBufferGetGPUAddress(const WisVKBuffer* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKBufferImpl>(self);
    auto& header = impl.device_header->header;
    auto& table = header.device_table;

    VmaAllocator allocator = impl.device_header->header.allocator;
    VmaAllocatorInfo allocator_info{};
    vmaGetAllocatorInfo(allocator, &allocator_info);

    VkBufferDeviceAddressInfo address_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext = nullptr,
        .buffer = impl.buffer
    };
    return table.vkGetBufferDeviceAddress(allocator_info.device, &address_info);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyTexture(WisVKTexture* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKTextureImpl>(self);
    if (impl.image != VK_NULL_HANDLE) {
        if (impl.owned_by_swapchain) {
            // If the image is owned by the swapchain, we should not destroy it directly, as it will be destroyed when
            // the swapchain is destroyed.
            impl.image = VK_NULL_HANDLE;
            return;
        }

        // get allocator
        VmaAllocator allocator = impl.device_header->header.allocator;
        vmaDestroyImage(allocator, impl.image, impl.allocation);

        impl.image = VK_NULL_HANDLE;

        wis::detail::VKReleaseDevice(impl.device_header);
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisVKTextureWriteSubresource(const WisVKTexture* self, const void* source_data, const WisTextureRegion* target_region)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKTextureImpl>(self);
    auto& header = impl.device_header->header;
    auto device = header.device;
    auto& table = header.device_table;

    static auto plane_to_aspect_mask = [](uint16_t plane) -> VkImageAspectFlags {
        switch (plane) {
        default:
        case 0:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case 1:
            return VK_IMAGE_ASPECT_PLANE_1_BIT;
        case 2:
            return VK_IMAGE_ASPECT_PLANE_2_BIT;
        }
    };

    VkMemoryToImageCopy region{
        .sType = VK_STRUCTURE_TYPE_MEMORY_TO_IMAGE_COPY,
        .pNext = nullptr,
        .pHostPointer = source_data,
        .memoryRowLength = 0,
        .memoryImageHeight = 0,
        .imageSubresource =
            {.aspectMask = plane_to_aspect_mask(target_region->target_subresource.plane_slice),
             .mipLevel = target_region->target_subresource.mip_level,
             .baseArrayLayer = target_region->target_subresource.array_layer,
             .layerCount = 1},
        .imageOffset =
            {static_cast<int32_t>(target_region->box.x),
             static_cast<int32_t>(target_region->box.y),
             static_cast<int32_t>(target_region->box.z)},
        .imageExtent{target_region->box.width, target_region->box.height, target_region->box.depth},
    };

    VkCopyMemoryToImageInfoEXT copy_info{
        .sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_IMAGE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .dstImage = impl.image,
        .dstImageLayout = VK_IMAGE_LAYOUT_GENERAL,
    };
    auto vr = table.vkCopyMemoryToImageEXT(device, &copy_info);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to write texture subresource">(vr);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyRootSignature(WisVKRootSignature* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRootSignatureImpl>(self);
    if (impl.root_signature_header != nullptr) {
        ::operator delete(impl.root_signature_header);
        impl.root_signature_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyShader(WisVKShader* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKShaderImpl>(self);
    if (impl.shader_module != VK_NULL_HANDLE) {
        auto& header = impl.device_header->header;
        auto& table = header.device_table;
        table.vkDestroyShaderModule(header.device, impl.shader_module, nullptr);
        impl.shader_module = VK_NULL_HANDLE;
        wis::detail::VKReleaseDevice(impl.device_header);
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyPipeline(WisVKPipeline* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKPipelineImpl>(self);
    if (impl.pipeline != VK_NULL_HANDLE) {
        auto& header = impl.device_header->header;
        auto& table = header.device_table;
        table.vkDestroyPipeline(header.device, impl.pipeline, nullptr);
        impl.pipeline = VK_NULL_HANDLE;

        wis::detail::VKReleaseDevice(impl.device_header);
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroySurface(WisVKSurface* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKSurfaceImpl>(self);
    if (impl.surface != VK_NULL_HANDLE) {
        wis::detail::VKReleaseSurface(impl.surface_header);
        impl.surface = VK_NULL_HANDLE;
        impl.surface_header = nullptr;
    }
}

#endif // WIS_VK_IMPL_CPP
