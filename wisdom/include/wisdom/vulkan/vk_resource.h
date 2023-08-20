#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_buffer_views.h>
#include <wisdom/vulkan/vk_views.h>
#endif

WIS_EXPORT namespace wis
{
class VKBuffer;

template<>
class Internal<VKBuffer>
{
public:
    wis::shared_handle<VmaAllocation> allocation; // order mandated
    wis::shared_handle<vk::Buffer> buffer;
    vk::DeviceSize size = 0u;
};

class VKBuffer : public QueryInternal<VKBuffer>
{
public:
    VKBuffer() = default;
    explicit VKBuffer(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<VmaAllocation> allocation, size_t size)
        : QueryInternal(std::move(allocation), std::move(buffer), size)
    {
    }
    operator VKBufferView() const noexcept
    {
        return buffer.get();
    }

    bool UpdateSubresource(std::span<const std::byte> data) noexcept
    {
        auto vma = allocation.getAllocator().get();
        auto al = allocation.get();

        void* mem = nullptr;
        vmaMapMemory(vma, al, &mem);
        if (mem == nullptr)
            return false;

        auto data_size = data.size();
        std::memcpy(mem, data.data(), data_size > size ? size : data_size);

        vmaUnmapMemory(vma, al);
        return true;
    }
    std::span<std::byte> MapMemory() noexcept
    {
        auto vma = allocation.getAllocator().get();
        auto al = allocation.get();
        void* mem = nullptr;
        vmaMapMemory(vma, al, &mem);
        if (mem == nullptr)
            return {};
        return { reinterpret_cast<std::byte*>(mem), size };
    }
    void UnmapMemory() noexcept
    {
        auto vma = allocation.getAllocator().get();
        auto al = allocation.get();
        vmaUnmapMemory(vma, al);
    }

    [[nodiscard]] VKVertexBufferView GetVertexBufferView(uint32_t byte_stride) const noexcept
    {
        return VKVertexBufferView{ buffer.get(), size, byte_stride };
    }
};

class VKTexture;

template<>
class Internal<VKTexture>
{
public:
    wis::shared_handle<VmaAllocation> allocation; // order mandated
    wis::shared_handle<vk::Image> buffer;
    vk::Format format;
};

class VKTexture : public QueryInternal<VKTexture>
{
public:
    VKTexture() = default;
    explicit VKTexture(vk::Format format, wis::shared_handle<vk::Image> buffer, wis::shared_handle<VmaAllocation> allocation = {})
        : QueryInternal(std::move(allocation), std::move(buffer), format)
    {
    }
    operator VKTextureView() const noexcept
    {
        return { buffer.get(), format };
    }
};
}
