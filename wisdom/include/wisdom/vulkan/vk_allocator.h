#ifndef VK_ALLOCATOR_H
#define VK_ALLOCATOR_H
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_checks.h>

namespace wis {
class VKResourceAllocator;

template<>
struct Internal<VKResourceAllocator> {
    wis::shared_handle<VmaAllocator> allocator;
    bool interop = false;
};

/// @brief Resource allocator for Vulkan
class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    VKResourceAllocator() = default;
    VKResourceAllocator(wis::shared_handle<VmaAllocator> allocator,
                        bool interop = false) noexcept
        : QueryInternal(std::move(allocator), interop)
    {
    }
    operator bool() const noexcept
    {
        return bool(allocator);
    }

public:
    // Resource creation functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKBuffer>
    CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory = wis::MemoryType::Default, wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKTexture>
    CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory = wis::MemoryType::Default, wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    // Allocation info functions
    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetBufferAllocationInfo(uint64_t size, wis::BufferUsage usage) const noexcept;

    // Allocation functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKMemory>
    AllocateImageMemory(uint64_t size, wis::TextureUsage usage,
                        wis::MemoryType memory = wis::MemoryType::Default,
                        wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKMemory>
    AllocateBufferMemory(uint64_t size, wis::BufferUsage usage,
                         wis::MemoryType memory = wis::MemoryType::Default,
                         wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    // Resource placement functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKBuffer>
    PlaceBuffer(wis::VKMemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKTexture>
    PlaceTexture(wis::VKMemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept;

public:
    // Convenience functions
    [[nodiscard]] wis::ResultValue<wis::VKBuffer>
    CreateUploadBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopySrc, MemoryType::Upload);
    }
    [[nodiscard]] wis::ResultValue<wis::VKBuffer>
    CreateReadbackBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopyDst, MemoryType::Readback);
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKBuffer>
    VKCreateBuffer(VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKTexture>
    VKCreateTexture(VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKBuffer>
    VKCreateAliasingBuffer(VkBufferCreateInfo& desc, VkDeviceSize offset, VmaAllocation alloc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKTexture>
    VKCreateAliasingTexture(VkImageCreateInfo& desc, VkDeviceSize offset, VmaAllocation alloc) const noexcept;

    WIS_INLINE static void
    VKFillBufferDesc(uint64_t size, BufferUsage flags, VkBufferCreateInfo& info) noexcept;

    WIS_INLINE static void
    VKFillImageDesc(const TextureDesc& desc, VkImageCreateInfo& info) noexcept;

    WIS_INLINE void
    VKFillTextureAllocationInfo(const wis::TextureDesc& desc, VkMemoryRequirements2& out_info) const noexcept;

    WIS_INLINE void
    VKFillBufferAllocationInfo(uint64_t size, wis::BufferUsage flags, VkMemoryRequirements2& out_info) const noexcept;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_ALLOCATOR_H
