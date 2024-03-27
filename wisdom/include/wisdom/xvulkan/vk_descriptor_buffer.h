#pragma once
#include <wisdom/xvulkan/vk_resource.h>

namespace wis {
class VKDescriptorBuffer;

template<>
struct Internal<VKDescriptorBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;
    h::VkBuffer buffer;
    uint32_t descriptor_size = 0;

    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation, uint32_t descriptor_size) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer), descriptor_size(descriptor_size) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&&) noexcept = default;
    ~Internal() noexcept
    {
        if (buffer && allocation) {
            vmaDestroyBuffer(allocator.get(), buffer, allocation);
        }
    }
};

class VKDescriptorBuffer : public QueryInternal<VKDescriptorBuffer>
{
public:
    VKDescriptorBuffer() noexcept = default;
    explicit VKDescriptorBuffer(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation, uint32_t descriptor_size) noexcept
        : QueryInternal(std::move(allocator), buffer, allocation, descriptor_size) { }
    operator bool() const noexcept { return bool(buffer); }
    operator VKDescriptorBufferView() const noexcept
    {
        return { buffer, descriptor_size };
    }
};
} // namespace wis