#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_allocator.h>
#endif // !WISDOM_HEADER_ONLY

[[nodiscard]] std::pair<wis::Result, wis::VKBuffer>
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
    return wis::succeeded(result)
            ? std::make_pair(wis::success, VKBuffer{ allocator, buffer, allocation })
            : std::make_pair(wis::make_result<FUNC, "Buffer allocation failed">(result), VKBuffer{});
}
