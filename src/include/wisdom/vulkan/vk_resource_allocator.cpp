#ifndef WIS_VK_RESOURCE_ALLOCATOR_CPP
#define WIS_VK_RESOURCE_ALLOCATOR_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/util/allocation.hpp>

namespace wis::detail {
inline VkImageCreateInfo VKFillImageDesc(const WisTextureDesc& desc) noexcept
{
    VkImageCreateInfo info{
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = 0,
        .format        = wis::detail::convert_vk(desc.format),
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .usage         = wis::detail::convert_vk(desc.usage_flags),
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    switch (desc.layout) {
    case WisTextureLayoutTexture1D:
        info.imageType   = VK_IMAGE_TYPE_1D;
        info.extent      = { desc.width, 1, 1 };
        info.mipLevels   = desc.mip_levels;
        info.arrayLayers = 1;
        break;
    case WisTextureLayoutTexture2D:
        info.imageType   = VK_IMAGE_TYPE_2D;
        info.extent      = { desc.width, desc.height, 1 };
        info.mipLevels   = desc.mip_levels;
        info.arrayLayers = 1;
        break;
    case WisTextureLayoutTexture1DArray:
        info.imageType   = VK_IMAGE_TYPE_1D;
        info.extent      = { desc.width, 1, 1 };
        info.mipLevels   = desc.mip_levels;
        info.arrayLayers = desc.depth_or_array_size;
        break;
    default:
    case WisTextureLayoutTexture2DArray:
        info.imageType   = VK_IMAGE_TYPE_2D;
        info.extent      = { desc.width, desc.height, 1 };
        info.mipLevels   = desc.mip_levels;
        info.arrayLayers = desc.depth_or_array_size;
        break;
    case WisTextureLayoutTexture3D:
        info.imageType   = VK_IMAGE_TYPE_3D;
        info.flags       = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT | VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT;
        info.extent      = { desc.width, desc.height, desc.depth_or_array_size };
        info.mipLevels   = desc.mip_levels;
        info.arrayLayers = 1;
        break;
    case WisTextureLayoutTexture2DMS:
        info.imageType   = VK_IMAGE_TYPE_2D;
        info.extent      = { desc.width, desc.height, 1 };
        info.mipLevels   = 1;
        info.arrayLayers = 1;
        info.samples     = wis::detail::convert_vk(desc.sample_count);
        break;
    case WisTextureLayoutTexture2DMSArray:
        info.imageType   = VK_IMAGE_TYPE_2D;
        info.extent      = { desc.width, desc.height, 1 };
        info.mipLevels   = 1;
        info.arrayLayers = desc.depth_or_array_size;
        info.samples     = wis::detail::convert_vk(desc.sample_count);
        break;
    case WisTextureLayoutTextureCube:
        info.imageType   = VK_IMAGE_TYPE_2D;
        info.flags       = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        info.extent      = { desc.width, desc.height, 1 };
        info.mipLevels   = 1;
        info.arrayLayers = 6;
        break;
    case WisTextureLayoutTextureCubeArray:
        info.imageType   = VK_IMAGE_TYPE_2D;
        info.flags       = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        info.extent      = { desc.width, desc.height, 1 };
        info.mipLevels   = 1;
        info.arrayLayers = desc.depth_or_array_size / 6 + (desc.depth_or_array_size % 6 != 0);
        break;
    }
    return info;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKResourceAllocatorImpl>(self);
    if (impl.allocator) {
        wis::detail::release_vk_device(impl.device_header);
        impl.allocator = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKResourceAllocatorCreateBuffer(const WisVKResourceAllocator* self,
                                                                     const WisBufferDesc*          desc,
                                                                     WisVKBuffer*                  buffer)
{
    auto& allocator = wis::from_handle_ref<const wis::impl::VKResourceAllocatorImpl>(self);

    VkBufferCreateInfo buffer_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size  = wis::aligned_size(desc->size_bytes, 265u), // align to uniform buffer alignment for safety
        .usage = (VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | wis::detail::convert_vk(desc->usage_flags)),
    };

    VmaAllocationCreateFlags flags = wis::detail::convert_vk(desc->memory_flags);
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
        .requiredFlags = wis::detail::convert_vk(desc->memory_type)
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
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Buffer creation failed">(vr);
    }

    void* mapped_ptr = nullptr;
    if (desc->memory_flags & WisMemoryFlagsMapped) {
        vr = vmaMapMemory(allocator.allocator, allocation_handle, &mapped_ptr);
        if (!wis::detail::succeeded(vr)) {
            vmaDestroyBuffer(allocator.allocator, buffer_handle, allocation_handle);
            return wis::detail::make_result<wis::detail::Func(), "Buffer memory mapping failed">(vr);
        }
    }

    auto& impl = *new (buffer) wis::impl::VKBufferImpl{
        .buffer        = buffer_handle,
        .allocation    = allocation_handle,
        .mapped_ptr    = mapped_ptr,
        .device_header = allocator.device_header,
    };

    impl.device_header->AddRef();

    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKResourceAllocatorCreateTexture(const WisVKResourceAllocator* self,
                                                                      const WisTextureDesc*         desc,
                                                                      WisTextureState               initial_state,
                                                                      WisVKTexture*                 buffer)
{
    auto& allocator = wis::from_handle_ref<const wis::impl::VKResourceAllocatorImpl>(self);
    // Check memory type, you can't create a texture with upload or readback memory types
    if (desc->memory_type == WisMemoryTypeUpload || desc->memory_type == WisMemoryTypeReadback) {
        return wis::detail::make_result<wis::detail::Func(), "Invalid memory type for texture creation">(VK_ERROR_UNKNOWN);
    }

    VkImageCreateInfo image_info = wis::detail::VKFillImageDesc(*desc);

    VmaAllocationCreateFlags flags = wis::detail::convert_vk(desc->memory_flags) & ~VMA_ALLOCATION_CREATE_MAPPED_BIT;
    VmaAllocationCreateInfo  alloc_info{
         .flags         = flags,
         .usage         = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
         .requiredFlags = wis::detail::convert_vk(desc->memory_type)
    };
    VkImage       image_handle      = VK_NULL_HANDLE;
    VmaAllocation allocation_handle = VK_NULL_HANDLE;
    VkResult      vr                = vmaCreateImage(
            allocator.allocator,
            &image_info,
            &alloc_info,
            &image_handle,
            &allocation_handle,
            nullptr);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Buffer creation failed">(vr);
    }

    // Perform transition to initial state if it's not undefined, since the image will be created in undefined layout
    if (initial_state != WisTextureStateUndefined) {
        auto& header   = allocator.device_header->header;
        auto& features = header.features;
        if (!(features.supported_image_layout_transitions & (1u << static_cast<uint32_t>(initial_state)))) {
            vmaDestroyImage(allocator.allocator, image_handle, allocation_handle);
            return wis::detail::make_result<wis::detail::Func(), "The Vulkan device does not support the requested initial texture state">(VK_ERROR_FEATURE_NOT_PRESENT);
        }

        // perform transition
        auto& table = header.device_table;

        VkHostImageLayoutTransitionInfoEXT transition_info{
            .sType            = VK_STRUCTURE_TYPE_HOST_IMAGE_LAYOUT_TRANSITION_INFO_EXT,
            .pNext            = nullptr,
            .image            = image_handle,
            .oldLayout        = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout        = wis::detail::convert_vk(initial_state),
            .subresourceRange = {
                                 .aspectMask     = wis::detail::VKAspectFlags(image_info.format),
                                 .baseMipLevel   = 0,
                                 .levelCount     = image_info.mipLevels,
                                 .baseArrayLayer = 0,
                                 .layerCount     = image_info.arrayLayers,
                                 },
        };

        vr = table.vkTransitionImageLayoutEXT(header.device, 1, &transition_info);
        if (!wis::detail::succeeded(vr)) {
            vmaDestroyImage(allocator.allocator, image_handle, allocation_handle);
            return wis::detail::make_result<wis::detail::Func(), "Failed to transition image to initial layout">(vr);
        }
    }

    auto& impl = *new (buffer) wis::impl::VKTextureImpl{
        .image         = image_handle,
        .allocation    = allocation_handle,
        .device_header = allocator.device_header,
    };
    impl.device_header->AddRef();

    return wis::detail::vk_success;
}

#endif // WIS_VK_RESOURCE_ALLOCATOR_CPP
