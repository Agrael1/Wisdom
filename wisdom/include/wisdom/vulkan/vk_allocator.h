#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_barrier.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/global/assertions.h>
#endif // !WISDOM_MODULES

namespace wis {
class VKResourceAllocator;
class VKDevice;

template<>
struct Internal<VKResourceAllocator> {
    wis::shared_handle<vma::Allocator> allocator;
};

/// @brief Resource allocator for Vulkan
WIS_EXPORT class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    VKResourceAllocator() = default;
    WIS_INLINE VKResourceAllocator(const VKDevice& device) noexcept;
    operator bool() const noexcept { return bool(allocator); }

    /// @brief Create a buffer that is persistently mapped to the GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreatePersistentBuffer(size_t size, BufferFlags flags = BufferFlags::None) const noexcept
    {
        vk::BufferCreateInfo desc{
            .size = size,
            .usage = vk::BufferUsageFlagBits(flags),
        };
        vma::AllocationCreateInfo alloc{
            .usage = vma::MemoryUsage::eAuto
        };
        return CreateBuffer(desc, alloc);
    }

    /// @brief Create a buffer that is accessible by the CPU and GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreateHostVisibleBuffer(size_t size, BufferFlags flags = BufferFlags::None) const noexcept
    {
        vk::BufferCreateInfo desc{
            .size = size,
            .usage = vk::BufferUsageFlagBits(flags),
        };
        vma::AllocationCreateInfo alloc{
            .flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
            .usage = vma::MemoryUsage::eAuto,
            .requiredFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent // ensure mapping does not need to be flushed
        };

        return CreateBuffer(desc, alloc);
    }

    /// @brief Create a buffer that is accessible by the CPU and serves as a staging buffer for GPU uploads
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreateUploadBuffer(size_t size) const noexcept
    {
        return CreateHostVisibleBuffer(size, BufferFlags::CopySource);
    }

    /// @brief Create a constant buffer that is accessible by the CPU and GPU
    /// This function is equivalent to CreateHostVisibleBuffer, but ensures that the buffer size is 256 byte aligned in debug mode
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreateConstantBuffer(size_t size) const noexcept
    {
        wis::assert_debug(size % 256 == 0, wis::format("{} is not 256 byte aligned", size));
        return CreateHostVisibleBuffer(size, BufferFlags::ConstantBuffer);
    }

    /// @brief Create a texture
    /// @param desc Texture descriptor
    /// @param flags Texture usage flags
    /// @return Texture object
    [[nodiscard]] VKTexture CreateTexture(const TextureDescriptor& desc, TextureFlags flags = TextureFlags::None) const noexcept
    {
        vk::ImageCreateInfo img_desc;
        img_desc.imageType = convert_vk(desc.type);
        vk::Extent3D extent;
        if (img_desc.imageType == vk::ImageType::e1D) {
            img_desc.extent = vk::Extent3D{ desc.t1d.width, 1, 1 };
            img_desc.arrayLayers = desc.t1d.array_size;
        } else if (img_desc.imageType == vk::ImageType::e2D) {
            img_desc.extent = vk::Extent3D{ desc.t2d.width, desc.t2d.height, 1 };
            img_desc.arrayLayers = desc.t2d.array_size;
        } else if (img_desc.imageType == vk::ImageType::e3D) {
            img_desc.extent = vk::Extent3D{ desc.t3d.width, desc.t3d.height, desc.t3d.depth };
            img_desc.arrayLayers = 1u;
        }

        img_desc.format = convert_vk(desc.format);
        img_desc.mipLevels = desc.mip_levels;
        img_desc.usage = vk::ImageUsageFlagBits(flags);
        img_desc.samples = vk::SampleCountFlagBits::e1;

        vma::AllocationCreateInfo alloc{
            .usage = vma::MemoryUsage::eAuto,
        };
        return CreateTexture(img_desc, alloc);
    }

    /// @brief Create a depth stencil texture
    /// @param desc Depth descriptor
    /// @param flags Texture usage flags, aside from depth stencil attachment
    /// @return Texture object 
    [[nodiscard]] VKTexture CreateDepthStencilTexture(DepthDescriptor desc, TextureFlags flags = TextureFlags::None) const noexcept
    {
        auto format = convert_vk(desc.format);
        vk::ImageCreateInfo img_desc{
            .imageType = vk::ImageType::e2D,
            .format = format,
            .extent = vk::Extent3D{ desc.width, desc.height, 1 },
            .mipLevels = 1,
            .arrayLayers = 1,
            .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits(flags),
        };
        vma::AllocationCreateInfo alloc{
            .usage = vma::MemoryUsage::eAuto,
        };
        return CreateTexture(img_desc, alloc);
    }

private:
    [[nodiscard]] VKBuffer CreateBuffer(const vk::BufferCreateInfo& desc, const vma::AllocationCreateInfo& alloc_desc) const noexcept
    {
        vk::Buffer buffer;
        vma::Allocation allocation;
        auto result = allocator->createBuffer(&desc, &alloc_desc, &buffer, &allocation, nullptr);
        if (!succeeded(result))
            return {};
        auto shared_allocation = wis::shared_handle<vma::Allocation>{ allocation, allocator };
        auto shared_buffer = wis::shared_handle<vk::Buffer>{ buffer, allocator.getParent() };

        return VKBuffer{ std::move(shared_buffer), std::move(shared_allocation), desc.size };
    }
    [[nodiscard]] VKTexture CreateTexture(const vk::ImageCreateInfo& desc, const vma::AllocationCreateInfo& alloc_desc) const noexcept
    {
        vk::Image image;
        vma::Allocation allocation;
        auto result = allocator->createImage(&desc, &alloc_desc, &image, &allocation, nullptr);
        if (!succeeded(result))
            return {};
        auto shared_allocation = wis::shared_handle<vma::Allocation>{ allocation, allocator };
        auto shared_buffer = wis::shared_handle<vk::Image>{ image, allocator.getParent() };
        auto format = desc.format;
        return VKTexture{ desc.format, std::move(shared_buffer), std::move(shared_allocation) };
    }
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_allocator.inl"
#endif
