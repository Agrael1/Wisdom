#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_allocator.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/generated/vulkan/vk_structs.hpp>

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::CreateBuffer(const VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
{
    VmaAllocation allocation;
    VkBuffer buffer;
    VkResult result = vmaCreateBuffer(
            allocator.get(),
            &desc,
            &alloc_desc,
            &buffer,
            &allocation,
            nullptr);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Buffer allocation failed">(result);

    return VKBuffer{ allocator, buffer, allocation };
}

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::CreateCommitedBuffer(size_t size, BufferFlags flags) const noexcept
{
    VkBufferCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = VkBufferUsageFlags(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VkBufferUsageFlagBits(flags)),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    VmaAllocationCreateInfo alloc{
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
    };
    return CreateBuffer(desc, alloc);
}

wis::ResultValue<wis::VKUploadBuffer>
wis::VKResourceAllocator::CreateUploadBuffer(size_t size) const noexcept
{
    VkBufferCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    VmaAllocationCreateInfo alloc{
        .flags = VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // ensure mapping does not need to be flushed
    };
    auto result = CreateBuffer(desc, alloc);
    return { result.status, VKUploadBuffer{ std::move(result.value) } };
}


wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::CreateTexture(wis::TextureDesc desc) const noexcept
{
    VkImageCreateInfo img_desc{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .format = convert_vk(desc.format),
        .usage = convert_vk(desc.usage),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = convert_vk(desc.initial_state),
    };

    switch (desc.layout) {
    case wis::TextureLayout::Texture1D:
        img_desc.imageType = VK_IMAGE_TYPE_1D;
        img_desc.extent = { desc.size.width, 1, 1 };
        img_desc.mipLevels = desc.mip_levels;
        img_desc.arrayLayers = 1;
        break;
    case wis::TextureLayout::Texture2D:
        img_desc.imageType = VK_IMAGE_TYPE_2D;
        img_desc.extent = { desc.size.width, desc.size.height, 1 };
        img_desc.mipLevels = desc.mip_levels;
        img_desc.arrayLayers = 1;
        break;
    case wis::TextureLayout::Texture1DArray:
        img_desc.imageType = VK_IMAGE_TYPE_1D;
        img_desc.extent = { desc.size.width, 1, 1 };
        img_desc.mipLevels = desc.mip_levels;
        img_desc.arrayLayers = desc.size.depth_or_layers;
        break;
    default:
    case wis::TextureLayout::Texture2DArray:
        img_desc.imageType = VK_IMAGE_TYPE_2D;
        img_desc.extent = { desc.size.width, desc.size.height, 1 };
        img_desc.mipLevels = desc.mip_levels;
        img_desc.arrayLayers = desc.size.depth_or_layers;
        break;
    case wis::TextureLayout::Texture3D:
        img_desc.imageType = VK_IMAGE_TYPE_3D;
        img_desc.flags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
        img_desc.extent = { desc.size.width, desc.size.height, desc.size.depth_or_layers };
        img_desc.mipLevels = desc.mip_levels;
        img_desc.arrayLayers = 1;
        break;
    case wis::TextureLayout::Texture2DMS:
        img_desc.imageType = VK_IMAGE_TYPE_2D;
        img_desc.extent = { desc.size.width, desc.size.height, 1 };
        img_desc.mipLevels = 1;
        img_desc.arrayLayers = 1;
        img_desc.samples = convert_vk(desc.sample_count);
        break;
    case wis::TextureLayout::Texture2DMSArray:
        img_desc.imageType = VK_IMAGE_TYPE_2D;
        img_desc.extent = { desc.size.width, desc.size.height, 1 };
        img_desc.mipLevels = 1;
        img_desc.arrayLayers = desc.size.depth_or_layers;
        img_desc.samples = convert_vk(desc.sample_count);
        break;
    }

    VmaAllocationCreateInfo alloc{
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
    };
    return CreateTexture(img_desc, alloc);
}

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::CreateTexture(const VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
{
    VmaAllocation allocation;
    VkImage buffer;

    auto result = vmaCreateImage(
            allocator.get(),
            reinterpret_cast<const VkImageCreateInfo*>(&desc),
            &alloc_desc,
            &buffer,
            &allocation,
            nullptr);

    if (!wis::succeeded(result))
        return wis::make_result<FUNC, "Texture allocation failed">(result);

    return VKTexture{ desc.format, buffer, { desc.extent.width, desc.extent.height }, allocator, allocation };
}