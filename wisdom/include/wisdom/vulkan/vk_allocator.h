#pragma once
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_checks.h>

namespace wis {
class VKResourceAllocator;

template<>
struct Internal<VKResourceAllocator> {
    wis::shared_handle<VmaAllocator> allocator;
    std::shared_ptr<VmaVulkanFunctions> functions;
};

/// @brief Resource allocator for Vulkan
class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    VKResourceAllocator() = default;
    VKResourceAllocator(wis::shared_handle<VmaAllocator> allocator,
                        std::shared_ptr<VmaVulkanFunctions> functions) noexcept
        : QueryInternal(std::move(allocator), std::move(functions))
    {
    }
    operator bool() const noexcept
    {
        return bool(allocator);
    }

public:
    WIS_INLINE [[nodiscard]] wis::ResultValue<VKBuffer>
    CreateCommitedBuffer(uint64_t size, BufferFlags flags = BufferFlags::None) const noexcept;

    WIS_INLINE [[nodiscard]] wis::ResultValue<VKBuffer>
    CreateUploadBuffer(uint64_t size) const noexcept;

    WIS_INLINE [[nodiscard]] wis::ResultValue<VKBuffer>
    CreateReadbackBuffer(uint64_t size) const noexcept;

    WIS_INLINE [[nodiscard]] wis::ResultValue<VKTexture>
    CreateTexture(wis::TextureDesc desc) const noexcept;

private:
    WIS_INLINE [[nodiscard]] wis::ResultValue<VKBuffer>
    CreateBuffer(const VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;

    WIS_INLINE [[nodiscard]] wis::ResultValue<VKTexture>
    CreateTexture(const VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc) const noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
