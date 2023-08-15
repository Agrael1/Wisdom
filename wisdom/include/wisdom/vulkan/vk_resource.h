#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_buffer_views.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>
#endif

namespace wis {
class VKBuffer;

template<>
struct Internal<VKBuffer> {
    wis::shared_handle<vma::Allocation> allocation; // order mandated
    wis::shared_handle<vk::Buffer> buffer;
    vk::DeviceSize size = 0u;
};

WIS_EXPORT class VKBuffer : public QueryInternal<VKBuffer>
{
public:
    VKBuffer() = default;
    explicit VKBuffer(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation, size_t size) noexcept
        : QueryInternal(std::move(allocation), std::move(buffer), size)
    {
    }
    operator VKBufferView() const noexcept
    {
        return buffer.get();
    }
    operator bool() const noexcept
    {
        return bool(buffer);
    }

public:
    [[nodiscard]] std::span<std::byte> MapMemory() noexcept
    {
        auto vma = allocation.getAllocator().get();
        auto al = allocation.get();
        auto [result, mem] = vma.mapMemory(al);
        return !wis::succeeded(result) ? std::span<std::byte>{} : std::span{ reinterpret_cast<std::byte*>(mem), size };
    }

    bool UpdateSubresource(std::span<const std::byte> data) noexcept
    {
        auto vma = allocation.getAllocator().get();
        auto al = allocation.get();
        auto [result, mem] = vma.mapMemory(al);
        if (!wis::succeeded(result))
            return false;

        auto data_size = data.size();
        std::memcpy(mem, data.data(), data_size > size ? size : data_size);
        vma.unmapMemory(al);
        return true;
    }

    void UnmapMemory() noexcept
    {
        auto vma = allocation.getAllocator().get();
        auto al = allocation.get();
        vma.unmapMemory(al);
    }

    [[nodiscard]] VKVertexBufferView GetVertexBufferView(uint32_t byte_stride) const noexcept
    {
        return VKVertexBufferView{ buffer.get(), size, byte_stride };
    }
};

class VKTexture;

template<>
struct Internal<VKTexture> {
    wis::shared_handle<vma::Allocation> allocation; // order mandated
    wis::shared_handle<vk::Image> image;
    vk::Format format;
};

WIS_EXPORT class VKTexture : public QueryInternal<VKTexture>
{
public:
    VKTexture() = default;
    explicit VKTexture(vk::Format format, wis::shared_handle<vk::Image> image, wis::shared_handle<vma::Allocation> allocation = {}) noexcept
        : QueryInternal(std::move(allocation), std::move(image), format)
    {
    }
    operator VKTextureView() const noexcept
    {
        return { image.get(), format };
    }
    operator bool() const noexcept
    {
        return bool(image);
    }
};
} // namespace wis
