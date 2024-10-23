#ifndef WIS_VK_RESOURCE_H
#define WIS_VK_RESOURCE_H
#include <wisdom/vulkan/vk_memory.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/generated/api/api.h>

namespace wis {
class VKBuffer;
class VKTexture;

template<>
struct Internal<VKBuffer> {
    wis::VKMemory memory;
    h::VkBuffer buffer;

    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator,
             VkBuffer buffer,
             VmaAllocation allocation = nullptr) noexcept
        : memory(std::move(allocator), allocation), buffer(buffer) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        Destroy();
        memory = std::move(o.memory);
        buffer = std::move(o.buffer);
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }

    void Destroy() noexcept
    {
        if (buffer && memory.GetInternal().allocator) {
            auto& device = memory.GetInternal().allocator.header();
            device.table().vkDestroyBuffer(device.get(), buffer, nullptr);
        }
    }
};

class ImplVKBuffer : public QueryInternal<VKBuffer>
{
public:
    ImplVKBuffer() noexcept = default;
    explicit ImplVKBuffer(wis::shared_handle<VmaAllocator> allocator,
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
    template<typename T = void>
    T* Map() const noexcept
    {
        return static_cast<T*>(memory.VKMap());
    }
    void* MapRaw() const noexcept
    {
        return memory.VKMap();
    }
    void Unmap() const noexcept
    {
        memory.VKUnmap();
    }
};

template<>
class Internal<VKTexture>
{
public:
    wis::VKMemory memory;
    h::VkImage buffer;
    VkFormat format{};
    wis::Size2D size{};

    Internal() noexcept = default;
    Internal(VkFormat format, VkImage buffer, wis::shared_handle<VmaAllocator> allocator, VmaAllocation allocation, Size2D size) noexcept
        : memory(std::move(allocator), allocation), buffer(buffer), format(format), size(size)
    {
    }
    Internal(Internal&& other) noexcept = default;
    Internal& operator=(Internal&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        Destroy();
        memory = std::move(other.memory);
        buffer = std::move(other.buffer);
        format = std::move(other.format);
        size = std::move(other.size);
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }

    void Destroy() noexcept
    {
        if (buffer && memory.GetInternal().allocator) {
            auto& device = memory.GetInternal().allocator.header();
            device.table().vkDestroyImage(device.get(), buffer, nullptr);
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

class VKSampler;

template<>
struct Internal<VKSampler> {
    wis::managed_handle_ex<VkSampler> sampler;
};

class VKSampler : public QueryInternal<VKSampler>
{
public:
    VKSampler() noexcept = default;
    explicit VKSampler(wis::managed_handle_ex<VkSampler> sampler) noexcept
        : QueryInternal(std::move(sampler)) { }

    operator bool() const noexcept
    {
        return bool(sampler);
    }
    operator VKSamplerView() const noexcept
    {
        return sampler.get();
    }
};

// =================================================================================================

class VKShaderResource;

template<>
struct Internal<VKShaderResource> {
    wis::managed_handle_ex<VkImageView> view;
};

class VKShaderResource : public QueryInternal<VKShaderResource>
{
public:
    VKShaderResource() noexcept = default;
    explicit VKShaderResource(wis::managed_handle_ex<VkImageView> view) noexcept
        : QueryInternal(std::move(view)) { }

    operator bool() const noexcept
    {
        return bool(view);
    }
    operator VKShaderResourceView() const noexcept
    {
        return view.get();
    }
};

#pragma region VKBuffer
/**
 * @brief Represents buffer object for storing linear data.
 * */
class VKBuffer : public wis::ImplVKBuffer
{
public:
    using wis::ImplVKBuffer::ImplVKBuffer;

public:
    /**
     * @brief Maps the buffer memory to CPU address space.
     * @return The pointer to the mapped memory.
     * */
    inline void* MapRaw() const noexcept
    {
        return wis::ImplVKBuffer::MapRaw();
    }
    /**
     * @brief Unmaps the buffer memory from CPU address space.
     * */
    inline void Unmap() const noexcept
    {
        return wis::ImplVKBuffer::Unmap();
    }
};
#pragma endregion VKBuffer

} // namespace wis

#endif // VK_RESOURCE_H
