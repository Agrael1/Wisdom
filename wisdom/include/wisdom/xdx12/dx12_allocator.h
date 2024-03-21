#pragma once
#include <wisdom/xdx12/dx12_resource.h>
#include <wisdom/xdx12/dx12_checks.h>

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
    CreateCommitedBuffer(size_t size, [[maybe_unused]] BufferFlags flags = BufferFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12UploadBuffer>
    CreateUploadBuffer(size_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Texture>
    CreateTexture(wis::TextureDesc desc) const noexcept;

private:
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12Buffer>
    CreateBuffer(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/dx12_allocator.cpp"
#endif // !WISDOM_HEADER_ONLY