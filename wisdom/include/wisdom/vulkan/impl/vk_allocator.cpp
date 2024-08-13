#ifndef VK_ALLOCATOR_CPP
#define VK_ALLOCATOR_CPP
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::VKCreateBuffer(const VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
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
wis::VKResourceAllocator::CreateCommitedBuffer(uint64_t size, BufferFlags flags) const noexcept
{
    VkBufferCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = VkBufferUsageFlags(VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VkBufferUsageFlagBits(flags)),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    VmaAllocationCreateInfo alloc{
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
    };
    return VKCreateBuffer(desc, alloc);
}

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::CreateUploadBuffer(uint64_t size) const noexcept
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
    auto result = VKCreateBuffer(desc, alloc);
    return { result.status, VKBuffer{ std::move(result.value) } };
}

wis::ResultValue<wis::VKBuffer>
wis::VKResourceAllocator::CreateReadbackBuffer(uint64_t size) const noexcept
{
    VkBufferCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    VmaAllocationCreateInfo alloc{
        .flags = VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // ensure mapping does not need to be flushed
    };
    auto result = VKCreateBuffer(desc, alloc);
    return { result.status, VKBuffer{ std::move(result.value) } };
}

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::CreateTexture(wis::TextureDesc desc) const noexcept
{
    VkImageCreateInfo img_desc = VKCreateTextureDesc(desc);

    VmaAllocationCreateInfo alloc{
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
    };
    return VKCreateTexture(img_desc, alloc);
}

wis::AllocationInfo
wis::VKResourceAllocator::GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept
{
    VkImageCreateInfo imageInfo = wis::VKResourceAllocator::VKCreateTextureDesc(desc);

    VkDeviceImageMemoryRequirementsKHR devImgMemReq{
        .sType = VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS_KHR,
        .pCreateInfo = &imageInfo
    };
    VkMemoryRequirements2 memReq{
        .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2
    };
    functions->vkGetDeviceImageMemoryRequirements(
            allocator.header().get(), &devImgMemReq, &memReq);

    return {
        .size_bytes = memReq.memoryRequirements.size,
        .alignment_bytes = memReq.memoryRequirements.alignment,
    };
}

wis::AllocationInfo
wis::VKResourceAllocator::GetBufferAllocationInfo(uint64_t size, BufferFlags flags) const noexcept
{
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VkBufferUsageFlags(VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VkBufferUsageFlagBits(flags)),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkDeviceBufferMemoryRequirementsKHR devBufferMemReq{
        .sType = VK_STRUCTURE_TYPE_DEVICE_BUFFER_MEMORY_REQUIREMENTS_KHR,
        .pCreateInfo = &bufferInfo
    };
    VkMemoryRequirements2 memReq{
        .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2
    };
    functions->vkGetDeviceBufferMemoryRequirements(
            allocator.header().get(), &devBufferMemReq, &memReq);

    return {
        .size_bytes = memReq.memoryRequirements.size,
        .alignment_bytes = memReq.memoryRequirements.alignment,
    };
}

wis::ResultValue<wis::VKTexture>
wis::VKResourceAllocator::VKCreateTexture(const VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
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

VkImageCreateInfo
wis::VKResourceAllocator::VKCreateTextureDesc(const TextureDesc& desc) noexcept
{
    VkImageCreateInfo img_desc{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .format = convert_vk(desc.format),
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .usage = convert_vk(desc.usage),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
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
    return img_desc;
}
#endif // !VK_ALLOCATOR_CPP
