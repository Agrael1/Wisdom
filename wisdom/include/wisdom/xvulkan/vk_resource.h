#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xvulkan/vk_views.h>

namespace wis {
class VKBuffer;

template<>
struct Internal<VKBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    VmaAllocation allocation;
    VkBuffer buffer;
    ~Internal()
    {
        if (buffer) {
            vmaDestroyBuffer(allocator.get(), buffer, allocation);
        }
    }
};

class VKBuffer : public QueryInternal<VKBuffer>
{
public:
    VKBuffer() noexcept = default;
    explicit VKBuffer(wis::shared_handle<VmaAllocator> allocator,
                      VkBuffer buffer,
                      VmaAllocation allocation = nullptr) noexcept
        : QueryInternal<VKBuffer>(std::move(allocator), allocation, buffer)
    {
    }
    operator bool() const noexcept
    {
        return bool(buffer);
    }

public:
};
} // namespace wis