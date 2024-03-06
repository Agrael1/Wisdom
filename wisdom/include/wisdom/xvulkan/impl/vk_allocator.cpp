#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_allocator.h>
#endif // !WISDOM_HEADER_ONLY

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