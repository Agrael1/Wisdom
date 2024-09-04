#pragma once
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
class DX12ResourceAllocator : public QueryInternal<DX12ResourceAllocator>
{
public:
    DX12ResourceAllocator() = default;
    explicit DX12ResourceAllocator(wis::com_ptr<D3D12MA::Allocator> in_allocator, ID3D12Device10* device) noexcept
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
    AllocateImageMemory(uint64_t size, wis::TextureUsage usage,
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
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
