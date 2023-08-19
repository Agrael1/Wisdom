#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_barrier.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/global/assertions.h>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
class VKResourceAllocator;

template<>
class Internal<VKResourceAllocator>
{
public:
    [[nodiscard]] VmaAllocator GetAllocator() const noexcept
    {
        return allocator.get();
    }

protected:
    wis::shared_handle<VmaAllocator> allocator;
};

/// @brief Resource allocator for Vulkan
class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    VKResourceAllocator() = default;
    WIS_INLINE VKResourceAllocator(const VKDevice& device)
    {
        auto& i = device.GetInternal();
        static constexpr auto version_mask = 0xFFFU;
        uint32_t version = 0;
        vkEnumerateInstanceVersion(&version);
        version &= ~(version_mask); // unsigned remove patch from instance for compatibility

        VmaAllocatorCreateInfo allocatorInfo{
            VmaAllocatorCreateFlags(0),
            i.adapter,
            i.device.get(),
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
        allocator = wis::shared_handle<VmaAllocator> { al, std::move(device) };
    }

    /// @brief Create a buffer that is persistently mapped to the GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreatePersistentBuffer(size_t size, BufferFlags flags = BufferFlags::None) const
    {
        vk::BufferCreateInfo desc{
            {}, size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits(flags), vk::SharingMode::eExclusive, 0, nullptr, nullptr
        };
        VmaAllocationCreateInfo alloc{
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
        };
        return CreateBuffer(desc, alloc);
    }

    /// @brief Create a buffer that is accessible by the CPU and serves as a staging buffer for GPU uploads
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreateUploadBuffer(size_t size) const
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

    /// @brief Create a buffer that is accessible by the CPU and GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreateHostVisibleBuffer(size_t size, BufferFlags flags = BufferFlags::None) const
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

    /// @brief Create a constant buffer that is accessible by the CPU and GPU
    /// This function is equivalent to CreateHostVisibleBuffer, but ensures that the buffer size is 256 byte aligned in debug mode
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] VKBuffer CreateConstantBuffer(size_t size) const
    {
        wis::assert_debug(size % 256 == 0, wis::format("{} is not 256 byte aligned", size));
        return CreateHostVisibleBuffer(size, BufferFlags::ConstantBuffer);
    }

    [[nodiscard]] VKTexture CreateTexture(const TextureDescriptor& desc) const
    {
        auto format = convert_vk(desc.format);
        vk::ImageCreateInfo img_desc{
            vk::ImageCreateFlagBits::e2DArrayCompatible | vk::ImageCreateFlagBits::e2DViewCompatibleEXT,
            vk::ImageType::e3D, format, vk::Extent3D{ desc.width, desc.height, desc.depth }, desc.mip_levels, desc.array_size
        };
        VmaAllocationCreateInfo alloc{
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        };
        VkImage image;
        VmaAllocation allocation;
        VkResult a = vmaCreateImage(allocator.get(), reinterpret_cast<const VkImageCreateInfo*>(&img_desc), &alloc, &image, &allocation, nullptr);
        return VKTexture{ format, wis::shared_handle<vk::Image>{ image, allocator.getParent() }, wis::shared_handle<VmaAllocation>{ allocation, allocator } };
    }
    [[nodiscard]] VKTexture CreateDepthStencilTexture(DepthDescriptor desc) const
    {
        auto format = convert_vk(desc.format);
        vk::ImageCreateInfo img_desc{
            vk::ImageCreateFlagBits{},
            vk::ImageType::e2D, format, vk::Extent3D{ desc.width, desc.height, 1 }, 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment
        };
        VmaAllocationCreateInfo alloc{
            .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        };
        VkImage image;
        VmaAllocation allocation;
        VkResult a = vmaCreateImage(allocator.get(), reinterpret_cast<const VkImageCreateInfo*>(&img_desc), &alloc, &image, &allocation, nullptr);
        return VKTexture{ format, wis::shared_handle<vk::Image>{ image, allocator.getParent() }, wis::shared_handle<VmaAllocation>{ allocation, allocator } };
    }

private:
    [[nodiscard]] VKBuffer CreateBuffer(const vk::BufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const
    {
        VmaAllocation allocation;
        VkBuffer buffer;
        VkResult a = vmaCreateBuffer(allocator.get(), reinterpret_cast<const VkBufferCreateInfo*>(&desc), &alloc_desc, &buffer, &allocation, nullptr);
        return VKBuffer{ wis::shared_handle<vk::Buffer>{ buffer, allocator.getParent() }, wis::shared_handle<VmaAllocation>{ allocation, allocator }, desc.size };
    }
};
}
