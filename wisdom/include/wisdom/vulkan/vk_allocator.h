#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_barrier.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/global/assertions.h>
#endif // !WISDOM_MODULES

namespace wis {
class VKResourceAllocator;

template<>
class Internal<VKResourceAllocator>
{
public:
    wis::shared_handle<VmaAllocator> allocator;
};

/// @brief Resource allocator for Vulkan
WIS_EXPORT class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    VKResourceAllocator() = default;
    VKResourceAllocator(const VKDevice& device) noexcept
    {
        auto& i = device.GetInternal();
        static constexpr auto version_mask = 0xFFFU;
        uint32_t version = 0;
        vkEnumerateInstanceVersion(&version);
        version &= ~(version_mask); // unsigned remove patch from instance for compatibility

        auto device_handle{ i.device };

        VmaAllocatorCreateInfo allocatorInfo{
            VmaAllocatorCreateFlags(0),
            i.adapter,
            device_handle.get(),
            0,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            i.instance.get(),
            version
        };

        VmaAllocator al;
        vmaCreateAllocator(&allocatorInfo, &al);
        allocator = wis::shared_handle<VmaAllocator>{ al, std::move(device_handle) };
    }
    operator bool() const noexcept
	{
		return bool(allocator);
	}
public:
    /// @brief Create a buffer that is persistently mapped to the GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer
    CreatePersistentBuffer(size_t size, BufferFlags flags = BufferFlags::None) const noexcept
    {
        vk::BufferCreateInfo desc{
            {}, size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits(flags), vk::SharingMode::eExclusive, 0, nullptr, nullptr
        };
        VmaAllocationCreateInfo alloc{
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
        };
        return CreateBuffer(desc, alloc);
    }

    /// @brief Create a buffer that is accessible by the CPU and GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer
    CreateHostVisibleBuffer(size_t size, BufferFlags flags = BufferFlags::None) const noexcept
    {
        vk::BufferCreateInfo desc{
            vk::BufferCreateFlags{}, size, vk::BufferUsageFlagBits(flags),
            vk::SharingMode::eExclusive, 0, nullptr, nullptr
        };
        VmaAllocationCreateInfo alloc{
            .flags = VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
            .requiredFlags = VkMemoryPropertyFlags(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) // ensure mapping does not need to be flushed
        };

        return CreateBuffer(desc, alloc);
    }

    /// @brief Create a buffer that is accessible by the CPU and serves as a staging buffer for GPU uploads
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer
    CreateUploadBuffer(size_t size) const noexcept
    {
        vk::BufferCreateInfo desc{
            vk::BufferCreateFlags{}, size, vk::BufferUsageFlagBits::eTransferSrc,
            vk::SharingMode::eExclusive, 0, nullptr, nullptr
        };
        VmaAllocationCreateInfo alloc{
            .flags = VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
            .requiredFlags = VkMemoryPropertyFlags(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) // ensure mapping does not need to be flushed
        };

        return CreateBuffer(desc, alloc);
    }

    /// @brief Create a constant buffer that is accessible by the CPU and GPU
    /// This function is equivalent to CreateHostVisibleBuffer, but ensures that the buffer size is 256 byte aligned in debug mode
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer
    CreateConstantBuffer(size_t size) const noexcept
    {
        wis::assert_debug(size % 256 == 0, wis::format("{} is not 256 byte aligned", size));
        return CreateHostVisibleBuffer(size, BufferFlags::ConstantBuffer);
    }

    [[nodiscard]] VKTexture
    CreateTexture(const TextureDescriptor& desc, TextureFlags flags = TextureFlags::None) const noexcept
    {
        auto format = convert_vk(desc.format);
        vk::ImageCreateInfo img_desc{};

        switch (desc.type) {
        case TextureType::Texture1D:
            img_desc = vk::ImageCreateInfo{
                {}, vk::ImageType::e1D, format, vk::Extent3D{ desc.width, 1, 1 }, desc.mip_levels, 1
            };
            break;
        case TextureType::Texture2D:
            img_desc = vk::ImageCreateInfo{
                {}, vk::ImageType::e2D, format, vk::Extent3D{ desc.width, desc.height, 1 }, desc.mip_levels, 1
            };
            break;
        case TextureType::Texture1DArray:
            img_desc = vk::ImageCreateInfo{
                {}, vk::ImageType::e1D, format, vk::Extent3D{ desc.width, 1, 1 }, desc.mip_levels, desc.array_size
            };
            break;
        case TextureType::Texture2DArray:
            img_desc = vk::ImageCreateInfo{
                {}, vk::ImageType::e2D, format, vk::Extent3D{ desc.width, desc.height, 1 }, desc.mip_levels, desc.array_size
            };
            break;
        case TextureType::Texture3D:
            img_desc = vk::ImageCreateInfo{
                vk::ImageCreateFlagBits::e2DArrayCompatible | vk::ImageCreateFlagBits::e2DViewCompatibleEXT,
                vk::ImageType::e3D, format, vk::Extent3D{ desc.width, desc.height, desc.depth }, desc.mip_levels, 1
            };
            break;

        default:
            assert(false && "TODO: make multisampled textures");
            break;
        }
        img_desc.usage = vk::ImageUsageFlagBits(flags);

        VmaAllocationCreateInfo alloc{
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        };
        return CreateTexture(img_desc, alloc);
    }

    [[nodiscard]] VKTexture
    CreateDepthStencilTexture(DepthDescriptor desc) const noexcept
    {
        const TextureDescriptor tex_desc{
            .type = TextureType::Texture2D,
            .format = desc.format,
            .width = desc.width,
            .height = desc.height,
            .depth = 1,
            .array_size = 1,
            .mip_levels = 1,
        };
        return CreateTexture(tex_desc, TextureFlags::DepthStencil);
    }

private:
    [[nodiscard]] VKBuffer
    CreateBuffer(const vk::BufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
    {
        VmaAllocation allocation;
        VkBuffer buffer;
        return wis::succeeded(vmaCreateBuffer(
                       allocator.get(),
                       reinterpret_cast<const VkBufferCreateInfo*>(&desc),
                       &alloc_desc,
                       &buffer,
                       &allocation,
                       nullptr))
                ? VKBuffer{
                      wis::shared_handle<vk::Buffer>{ buffer, allocator.getParent() },
                      wis::shared_handle<VmaAllocation>{ allocation, allocator }, desc.size
                  }
                : VKBuffer{};
    }
    [[nodiscard]] VKTexture
    CreateTexture(const vk::ImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept
    {
        VmaAllocation allocation;
        VkImage buffer;
        return wis::succeeded(vmaCreateImage(
                       allocator.get(),
                       reinterpret_cast<const VkImageCreateInfo*>(&desc),
                       &alloc_desc,
                       &buffer,
                       &allocation,
                       nullptr))
                ? VKTexture{
                      desc.format,
                      wis::shared_handle<vk::Image>{ buffer, allocator.getParent() },
                      wis::shared_handle<VmaAllocation>{ allocation, allocator }
                  }
                : VKTexture{};
    }
};
} // namespace wis
