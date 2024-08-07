#pragma once
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_checks.h>

namespace wis {
class DX12ResourceAllocator;
class DX12Device;

template<>
struct Internal<DX12ResourceAllocator> {
    wis::com_ptr<D3D12MA::Allocator> allocator;
};

/// @brief Resource allocator for DX12
class DX12ResourceAllocator : public QueryInternal<DX12ResourceAllocator>
{
public:
    DX12ResourceAllocator() = default;
    explicit DX12ResourceAllocator(wis::com_ptr<D3D12MA::Allocator> in_allocator) noexcept
        : QueryInternal(std::move(in_allocator))
    {
    }
    operator bool() const noexcept
    {
        return bool(allocator);
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Buffer>
    CreateCommitedBuffer(uint64_t size, BufferFlags flags = BufferFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Buffer>
    CreateUploadBuffer(uint64_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Buffer>
    CreateReadbackBuffer(uint64_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Texture>
    CreateTexture(wis::TextureDesc desc) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Buffer>
    DX12CreateResource(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept;

    [[nodiscard]] WIS_INLINE static D3D12_RESOURCE_DESC1
    DX12CreateTextureDesc(const TextureDesc& desc) noexcept;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY
