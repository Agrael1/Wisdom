#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/generated/api/api.h>

namespace wis {
class VKBuffer;

namespace h {
using VmaAllocation = wis::movable_handle<VmaAllocation>;
}

template<>
struct Internal<VKBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;
    h::VkBuffer buffer;

    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&&) noexcept = default;
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
        : QueryInternal<VKBuffer>(std::move(allocator), buffer, allocation)
    {
    }
    operator VKBufferView() const noexcept
    {
        return { buffer };
    }
    operator bool() const noexcept
    {
        return bool(buffer);
    }

public:
};

class VKUploadBuffer : public VKBuffer
{
public:
    VKUploadBuffer() noexcept = default;
    explicit VKUploadBuffer(wis::shared_handle<VmaAllocator> allocator,
                            VkBuffer buffer,
                            VmaAllocation allocation = nullptr) noexcept
        : VKBuffer(std::move(allocator), buffer, allocation)
    {
    }
    explicit VKUploadBuffer(VKBuffer&& buffer) noexcept
        : VKBuffer(std::move(buffer))
    {
    }

public:
    void* Map() const noexcept
    {
        void* data;
        vmaMapMemory(allocator.get(), allocation, &data);
        return data;
    }
    void Unmap() const noexcept
    {
        vmaUnmapMemory(allocator.get(), allocation);
    }

public:
    template<typename T>
    T* Map() const noexcept
    {
        return static_cast<T*>(Map());
    }
};

class VKTexture;

template<>
class Internal<VKTexture>
{
public:
    wis::shared_handle<VmaAllocator> allocator;
    VmaAllocation allocation = nullptr;
    h::VkImage buffer;
    VkFormat format{};
    wis::Size2D size{};

    Internal() noexcept = default;
    Internal(VkFormat format, VkImage buffer, wis::shared_handle<VmaAllocator> allocator, VmaAllocation allocation, Size2D size) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer), format(format), size(size)
    {
    }
    Internal(Internal&& other) noexcept = default;
    Internal& operator=(Internal&& other) noexcept = default;
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
    explicit VKTexture(VkFormat format, VkImage buffer, wis::Size2D size, wis::shared_handle<VmaAllocator> allocator = {}, VmaAllocation allocation = nullptr) noexcept
        : QueryInternal(format, std::move(buffer), std::move(allocator), allocation, size)
    {
    }
    operator VKTextureView() const noexcept
    {
        return { buffer, format, size };
    }
    operator bool() const noexcept
    {
        return bool(buffer);
    }
};

// =================================================================================================
class VKRenderTarget;

template<>
struct Internal<VKRenderTarget> {
    wis::managed_handle_ex<VkImageView> view;
    wis::Size2D size;
};

class VKRenderTarget : public QueryInternal<VKRenderTarget>
{
public:
    VKRenderTarget() = default;
    explicit VKRenderTarget(wis::managed_handle_ex<VkImageView> view, wis::Size2D size) noexcept
        : QueryInternal(std::move(view), size)
    {
    }
    operator bool() const noexcept
    {
        return bool(view);
    }
    operator VKRenderTargetView() const noexcept
    {
        return { view.get(), size };
    }
};

} // namespace wis