#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_allocator.h>
#endif // !WISDOM_HEADER_ONLY

[[nodiscard]] wis::ResultValue<wis::VKBuffer>
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
