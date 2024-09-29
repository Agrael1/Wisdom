#ifndef WIS_DX12_ALLOCATOR_H
#define WIS_DX12_ALLOCATOR_H
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_checks.h>

namespace wis {
class DX12ResourceAllocator;
class DX12Device;

template<>
struct Internal<DX12ResourceAllocator> {
    wis::com_ptr<D3D12MA::Allocator> allocator;
    ID3D12Device10* device; // ownedby allocator, no need to release
};

/// @brief Resource allocator for DX12
class ImplDX12ResourceAllocator : public QueryInternal<DX12ResourceAllocator>
{
public:
    ImplDX12ResourceAllocator() = default;
    explicit ImplDX12ResourceAllocator(wis::com_ptr<D3D12MA::Allocator> in_allocator, ID3D12Device10* device) noexcept
        : QueryInternal(std::move(in_allocator), device)
    {
    }
    operator bool() const noexcept
    {
        return bool(allocator);
    }

public:
    // Resource creation functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Buffer>
    CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory = wis::MemoryType::Default, wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Texture>
    CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory = wis::MemoryType::Default, wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    // Allocation info functions
    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::AllocationInfo
    GetBufferAllocationInfo(uint64_t size, BufferUsage flags = BufferUsage::None) const noexcept;

    // Allocation functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Memory>
    AllocateTextureMemory(uint64_t size, wis::TextureUsage usage,
                          wis::MemoryType memory = wis::MemoryType::Default,
                          wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Memory>
    AllocateBufferMemory(uint64_t size, wis::BufferUsage usage,
                         wis::MemoryType memory = wis::MemoryType::Default,
                         wis::MemoryFlags mem_flags = wis::MemoryFlags::None) const noexcept;

    // Resource placement functions
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Buffer>
    PlaceBuffer(DX12MemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Texture>
    PlaceTexture(DX12MemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept;

public:
    // Convenience functions
    [[nodiscard]] wis::ResultValue<DX12Buffer>
    CreateUploadBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopySrc, MemoryType::Upload, wis::MemoryFlags::Mapped);
    }
    [[nodiscard]] wis::ResultValue<DX12Buffer>
    CreateReadbackBuffer(uint64_t size) const noexcept
    {
        return CreateBuffer(size, BufferUsage::CopyDst, MemoryType::Readback, wis::MemoryFlags::Mapped);
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Buffer>
    DX12CreateResource(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept;

    WIS_INLINE static void
    DX12FillBufferDesc(uint64_t size, BufferUsage flags, D3D12_RESOURCE_DESC1& info) noexcept;

    WIS_INLINE static void
    DX12FillTextureDesc(const TextureDesc& desc, D3D12_RESOURCE_DESC1& info) noexcept;
};

#pragma region DX12ResourceAllocator
/**
 * @brief Main memory and resource allocator.
 * Uses DXMA and VMA libraries which can allocate memory with O(1) complexity with TLSF algorithm.
 * */
struct DX12ResourceAllocator : public wis::ImplDX12ResourceAllocator {
public:
    using wis::ImplDX12ResourceAllocator::ImplDX12ResourceAllocator;

public:
    /**
     * @brief Creates a buffer object and allocates memory for it.
     * Equivalent to creating a Buffer, allocating a memory and binding buffer to it.
     * @param size The size of the buffer in bytes.
     * @param usage The usage of the buffer. May affect the alignment of the buffer memory allocation.
     * @param memory The type of the memory to allocate for the buffer.
     * @param mem_flags The flags of the memory to allocate for the buffer.
     * @return wis::DX12Buffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Buffer> CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::CreateBuffer(size, usage, memory, mem_flags);
    }
    /**
     * @brief Creates a texture object and allocates memory for it.
     * Equivalent to creating a Texture, allocating a memory and binding texture to it.
     * @param desc The description of the texture to create.
     * @param memory The type of the memory to allocate for the texture.
     * @param mem_flags The flags of the memory to allocate for the texture.
     * @return wis::DX12Texture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Texture> CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::CreateTexture(desc, memory, mem_flags);
    }
    /**
     * @brief Returns the allocation info for the texture.
     * @param desc The description of the texture to get the allocation info for.
     * @return The allocation info for the texture. Contains size and alignment. Useful if allocating memory manually.
     * */
    inline wis::AllocationInfo GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::GetTextureAllocationInfo(desc);
    }
    /**
     * @brief Returns the allocation info for the buffer.
     * @param size The size of the buffer to get the allocation info for.
     * @param usage The usage of the buffer. May affect the alignment of the buffer memory allocation.
     * @return The allocation info for the buffer. Contains size and alignment. Useful if allocating memory manually.
     * */
    inline wis::AllocationInfo GetBufferAllocationInfo(uint64_t size, wis::BufferUsage usage) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::GetBufferAllocationInfo(size, usage);
    }
    /**
     * @brief Allocates memory for the image.
     * @param size The size of the memory to allocate.
     * @param usage The usage of the image memory.
     * @param mem_type The type of the memory to allocate for the image.
     * @param mem_flags The flags of the memory to allocate for the image.
     * @return wis::DX12Memory on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Memory> AllocateTextureMemory(uint64_t size, wis::TextureUsage usage, wis::MemoryType mem_type = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::AllocateTextureMemory(size, usage, mem_type, mem_flags);
    }
    /**
     * @brief Allocates memory for the buffer.
     * @param size The size of the memory to allocate.
     * @param usage The usage of the buffer memory.
     * @param mem_type The type of the memory to allocate for the buffer.
     * @param mem_flags The flags of the memory to allocate for the buffer.
     * @return wis::DX12Memory on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Memory> AllocateBufferMemory(uint64_t size, wis::BufferUsage usage, wis::MemoryType mem_type = MemoryType::Default, wis::MemoryFlags mem_flags = MemoryFlags::None) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::AllocateBufferMemory(size, usage, mem_type, mem_flags);
    }
    /**
     * @brief Creates buffer with provided memory.
     * Equivalent to creating aliasing resource.
     * Note, the resulting buffer must be destroyed before Memory backing it up.
     * @param memory The memory to bind the buffer to.
     * @param memory_offset The offset in the memory to bind the buffer to.
     * @param size The size of the buffer to bind.
     * @param usage The usage of the buffer.
     * @return wis::DX12Buffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Buffer> PlaceBuffer(wis::DX12MemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::PlaceBuffer(std::move(memory), memory_offset, size, usage);
    }
    /**
     * @brief Creates texture with provided memory.
     * Equivalent to creating aliasing resource.
     * Note, the resulting Texture must be destroyed before Memory backing it up.
     * @param memory The memory to bind the buffer to.
     * @param memory_offset The offset in the memory to bind the texture to.
     * @param desc The description of the texture to create.
     * @return wis::DX12Texture on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Texture> PlaceTexture(wis::DX12MemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept
    {
        return wis::ImplDX12ResourceAllocator::PlaceTexture(std::move(memory), memory_offset, desc);
    }
};
#pragma endregion DX12ResourceAllocator

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !WIS_DX12_ALLOCATOR_H
