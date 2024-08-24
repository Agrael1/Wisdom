#ifndef VK_ALLOCATOR_H
#define VK_ALLOCATOR_H
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_checks.h>

namespace wis {
class VKResourceAllocator;

template<>
struct Internal<VKResourceAllocator> {
    wis::shared_handle<VmaAllocator> allocator;
    std::shared_ptr<VmaVulkanFunctions> functions;
    bool interop = false;
};

/// @brief Resource allocator for Vulkan
class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    VKResourceAllocator() = default;
    VKResourceAllocator(wis::shared_handle<VmaAllocator> allocator,
                        std::shared_ptr<VmaVulkanFunctions> functions,
                        bool interop = false) noexcept
        : QueryInternal(std::move(allocator), std::move(functions), interop)
    {
    }
    operator bool() const noexcept
    {
        return bool(allocator);
    }

public:
    // Resource creation functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKBuffer>
    CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory = wis::MemoryType::Default, wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKTexture>
    CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory = wis::MemoryType::Default, wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    // Allocation info functions
    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetBufferAllocationInfo(uint64_t size, wis::BufferUsage usage) const noexcept;

    // Allocation functions
    WIS_INLINE void AllocateImageMemory(uint64_t size, wis::TextureUsage usage,
                                        wis::MemoryType memory = wis::MemoryType::Default,
                                        wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    WIS_INLINE void AllocateBufferMemory(uint64_t size, wis::BufferUsage usage,
                                         wis::MemoryType memory = wis::MemoryType::Default,
                                         wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

public:
    // Convenience functions
    [[nodiscard]] wis::ResultValue<VKBuffer>
    CreateUploadBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopySrc, MemoryType::Upload);
    }
    [[nodiscard]] wis::ResultValue<VKBuffer>
    CreateReadbackBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopyDst, MemoryType::Readback);
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKBuffer>
    VKCreateBuffer(VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKTexture>
    VKCreateTexture(VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    [[nodiscard]] WIS_INLINE static void
    VKFillBufferDesc(uint64_t size, BufferUsage flags, VkBufferCreateInfo& info) noexcept;

    [[nodiscard]] WIS_INLINE static void
    VKFillImageDesc(const TextureDesc& desc, VkImageCreateInfo& info) noexcept;

    [[nodiscard]] WIS_INLINE void
    VKGetTextureAllocationInfo(const wis::TextureDesc& desc, VkMemoryRequirements2& out_info) const noexcept;

    [[nodiscard]] WIS_INLINE void
    VKGetBufferAllocationInfo(uint64_t size, wis::BufferUsage flags, VkMemoryRequirements2& out_info) const noexcept;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_ALLOCATOR_H
