#ifndef WIS_VK_ALLOCATOR_H
#define WIS_VK_ALLOCATOR_H
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_checks.h>

namespace wis {
class VKResourceAllocator;

template<>
struct Internal<VKResourceAllocator> {
    wis::shared_handle<VmaAllocator> allocator;
    wis::shared_handle<VmaAllocator> export_memory_allocator;
};

/// @brief Resource allocator for Vulkan
class ImplVKResourceAllocator : public QueryInternal<VKResourceAllocator>
{
public:
    ImplVKResourceAllocator() = default;
    ImplVKResourceAllocator(wis::shared_handle<VmaAllocator> allocator,
                            wis::shared_handle<VmaAllocator> export_memory_allocator) noexcept
        : QueryInternal(std::move(allocator), std::move(export_memory_allocator))
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
    AllocateTextureMemory(uint64_t size, wis::TextureUsage usage,
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
        return CreateBuffer(size, BufferUsage::CopySrc, MemoryType::Upload, wis::MemoryFlags::Mapped);
    }
    [[nodiscard]] wis::ResultValue<wis::VKBuffer>
    CreateReadbackBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopyDst, MemoryType::Readback, wis::MemoryFlags::Mapped);
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKBuffer>
    VKCreateBuffer(VkBufferCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc, bool interop = false) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKTexture>
    VKCreateTexture(VkImageCreateInfo& desc, const VmaAllocationCreateInfo& alloc_desc, bool interop = false) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKBuffer>
    VKCreateAliasingBuffer(VkBufferCreateInfo& desc, VkDeviceSize offset, VmaAllocation alloc, bool interop = false) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKTexture>
    VKCreateAliasingTexture(VkImageCreateInfo& desc, VkDeviceSize offset, VmaAllocation alloc, bool interop = false) const noexcept;

    WIS_INLINE static void
    VKFillBufferDesc(uint64_t size, BufferUsage flags, VkBufferCreateInfo& info) noexcept;

    WIS_INLINE static void
    VKFillImageDesc(const TextureDesc& desc, VkImageCreateInfo& info) noexcept;

    WIS_INLINE void
    VKFillTextureAllocationInfo(const wis::TextureDesc& desc, VkMemoryRequirements2& out_info) const noexcept;

    WIS_INLINE void
    VKFillBufferAllocationInfo(uint64_t size, wis::BufferUsage flags, VkMemoryRequirements2& out_info) const noexcept;
};

#pragma region VKResourceAllocator
/**
 * @brief Main memory and resource allocator.
 * Uses DXMA and VMA libraries which can allocate memory with O(1) complexity with TLSF algorithm.
 * */
class VKResourceAllocator : public wis::ImplVKResourceAllocator
{
public:
    using wis::ImplVKResourceAllocator::ImplVKResourceAllocator;

public:
    /**
     * @brief Creates a buffer object and allocates memory for it.
     * Equivalent to creating a Buffer, allocating a memory and binding buffer to it.
     * @param size The size of the buffer in bytes.
     * @param usage The usage of the buffer. May affect the alignment of the buffer memory allocation.
     * @param memory The type of the memory to allocate for the buffer.
     * @param mem_flags The flags of the memory to allocate for the buffer.
     * @return wis::VKBuffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKBuffer> CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplVKResourceAllocator::CreateBuffer(size, usage, memory, mem_flags);
    }
    /**
     * @brief Creates a texture object and allocates memory for it.
     * Equivalent to creating a Texture, allocating a memory and binding texture to it.
     * @param desc The description of the texture to create.
     * @param memory The type of the memory to allocate for the texture.
     * @param mem_flags The flags of the memory to allocate for the texture.
     * @return wis::VKTexture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKTexture> CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplVKResourceAllocator::CreateTexture(desc, memory, mem_flags);
    }
    /**
     * @brief Returns the allocation info for the texture.
     * @param desc The description of the texture to get the allocation info for.
     * @return The allocation info for the texture. Contains size and alignment. Useful if allocating memory manually.
     * */
    inline wis::AllocationInfo GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept
    {
        return wis::ImplVKResourceAllocator::GetTextureAllocationInfo(desc);
    }
    /**
     * @brief Returns the allocation info for the buffer.
     * @param size The size of the buffer to get the allocation info for.
     * @param usage The usage of the buffer. May affect the alignment of the buffer memory allocation.
     * @return The allocation info for the buffer. Contains size and alignment. Useful if allocating memory manually.
     * */
    inline wis::AllocationInfo GetBufferAllocationInfo(uint64_t size, wis::BufferUsage usage) const noexcept
    {
        return wis::ImplVKResourceAllocator::GetBufferAllocationInfo(size, usage);
    }
    /**
     * @brief Allocates memory for the image.
     * @param size The size of the memory to allocate.
     * @param usage The usage of the image memory.
     * @param mem_type The type of the memory to allocate for the image.
     * @param mem_flags The flags of the memory to allocate for the image.
     * @return wis::VKMemory on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKMemory> AllocateTextureMemory(uint64_t size, wis::TextureUsage usage, wis::MemoryType mem_type = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplVKResourceAllocator::AllocateTextureMemory(size, usage, mem_type, mem_flags);
    }
    /**
     * @brief Allocates memory for the buffer.
     * @param size The size of the memory to allocate.
     * @param usage The usage of the buffer memory.
     * @param mem_type The type of the memory to allocate for the buffer.
     * @param mem_flags The flags of the memory to allocate for the buffer.
     * @return wis::VKMemory on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKMemory> AllocateBufferMemory(uint64_t size, wis::BufferUsage usage, wis::MemoryType mem_type = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplVKResourceAllocator::AllocateBufferMemory(size, usage, mem_type, mem_flags);
    }
    /**
     * @brief Creates buffer with provided memory.
     * Equivalent to creating aliasing resource.
     * Note, the resulting buffer must be destroyed before Memory backing it up.
     * @param memory The memory to bind the buffer to.
     * @param memory_offset The offset in the memory to bind the buffer to.
     * @param size The size of the buffer to bind.
     * @param usage The usage of the buffer.
     * @return wis::VKBuffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKBuffer> PlaceBuffer(wis::VKMemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept
    {
        return wis::ImplVKResourceAllocator::PlaceBuffer(std::move(memory), memory_offset, size, usage);
    }
    /**
     * @brief Creates texture with provided memory.
     * Equivalent to creating aliasing resource.
     * Note, the resulting Texture must be destroyed before Memory backing it up.
     * @param memory The memory to bind the buffer to.
     * @param memory_offset The offset in the memory to bind the texture to.
     * @param desc The description of the texture to create.
     * @return wis::VKTexture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKTexture> PlaceTexture(wis::VKMemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept
    {
        return wis::ImplVKResourceAllocator::PlaceTexture(std::move(memory), memory_offset, desc);
    }
};
#pragma endregion VKResourceAllocator

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_ALLOCATOR_H
