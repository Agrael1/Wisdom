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
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKBuffer>
    CreateCommitedBuffer(uint64_t size, BufferFlags flags = BufferFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKBuffer>
    CreateUploadBuffer(uint64_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKBuffer>
    CreateReadbackBuffer(uint64_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKTexture>
    CreateTexture(wis::TextureDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetBufferAllocationInfo(uint64_t size, BufferFlags flags = BufferFlags::None) const noexcept;

    // Allocation functions
    WIS_INLINE void AllocateImageMemory(uint64_t size, wis::TextureUsage usage,
                             wis::MemoryType memory) const noexcept;

    WIS_INLINE void AllocateBufferMemory(uint64_t size, wis::BufferFlags usage,
                             wis::MemoryType memory) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKBuffer>
    VKCreateBuffer(VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKTexture>
    VKCreateTexture(VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    [[nodiscard]] WIS_INLINE static VkImageCreateInfo
    VKCreateTextureDesc(const TextureDesc& desc) noexcept;

    [[nodiscard]] WIS_INLINE static void
    VKFillBufferDesc(uint64_t size, BufferFlags flags, VkBufferCreateInfo& info) noexcept;

    [[nodiscard]] WIS_INLINE static void
    VKFillImageDesc(const TextureDesc& desc, VkImageCreateInfo& info) noexcept;

    [[nodiscard]] WIS_INLINE void
    VKGetTextureAllocationInfo(const wis::TextureDesc& desc, VkMemoryRequirements2& out_info) const noexcept;

    [[nodiscard]] WIS_INLINE void
    VKGetBufferAllocationInfo(uint64_t size, wis::BufferFlags flags, VkMemoryRequirements2& out_info) const noexcept;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_ALLOCATOR_H
