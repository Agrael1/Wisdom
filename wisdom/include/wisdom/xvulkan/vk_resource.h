#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xvulkan/vk_views.h>
#include <wisdom/generated/api/api.h>

namespace wis {
class VKBuffer;

template<>
struct Internal<VKBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    VmaAllocation allocation;
    VkBuffer buffer;
    ~Internal() noexcept
    {
        if (buffer && allocation) {
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

class VKTexture;

template<>
class Internal<VKTexture>
{
public:
    wis::shared_handle<VmaAllocator> allocator;
    VmaAllocation allocation = nullptr;
    VkImage buffer;
    VkFormat format;

    Internal() noexcept = default;
    Internal(VkFormat format, VkImage buffer, wis::shared_handle<VmaAllocator> allocator, VmaAllocation allocation) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer), format(format)
    {
    }
    Internal(Internal&& other) noexcept
        : allocator(std::move(other.allocator))
        , allocation(std::exchange(other.allocation, nullptr))
        , buffer(std::exchange(other.buffer, nullptr))
        , format(other.format)
    {
    }
    Internal& operator=(Internal&& other) noexcept
    {
        allocator = std::move(other.allocator);
        allocation = std::exchange(other.allocation, nullptr);
        buffer = std::exchange(other.buffer, nullptr);
        format = other.format;
        return *this;
    }
    ~Internal() noexcept
    {
        if (buffer && allocation) {
            vmaDestroyImage(allocator.get(), buffer, allocation);
        }
    }
};

class VKTexture : public QueryInternal<VKTexture>
{
public:
    VKTexture() = default;
    explicit VKTexture(VkFormat format, VkImage buffer, wis::shared_handle<VmaAllocator> allocator = {}, VmaAllocation allocation = nullptr) noexcept
        : QueryInternal(format, std::move(buffer), std::move(allocator), allocation)
    {
    }
    operator VKTextureView() const noexcept
    {
        return { buffer };
    }
    operator bool() const noexcept
    {
        return bool(buffer);
    }
};
} // namespace wis