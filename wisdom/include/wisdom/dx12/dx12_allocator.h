#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_barrier.h>
#include <wisdom/dx12/dx12_resource.h>
#include <D3D12MemAlloc.h>
#include <wisdom/global/assertions.h>
#include <d3dx12/d3dx12.h>
#endif

WIS_EXPORT namespace wis
{
class DX12ResourceAllocator;

template<>
class Internal<DX12ResourceAllocator>
{
public:
    [[nodiscard]] D3D12MA::Allocator* GetAllocator() const noexcept
    {
        return allocator.get();
    }

protected:
    winrt::com_ptr<D3D12MA::Allocator> allocator;
};

/// @brief Resource allocator for DX12
class DX12ResourceAllocator : public QueryInternal<DX12ResourceAllocator>
{
public:
    DX12ResourceAllocator() = default;
    WIS_INLINE explicit DX12ResourceAllocator(DX12DeviceView device, DX12AdapterView adapter);

    /// @brief Create a buffer that is persistently mapped to the GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] WIS_INLINE DX12Buffer CreatePersistentBuffer(size_t size, [[maybe_unused]] BufferFlags flags = BufferFlags::None);

    /// @brief Create a buffer that is accessible by the CPU and serves as a staging buffer for GPU uploads
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] WIS_INLINE DX12Buffer CreateUploadBuffer(size_t size);

    /// @brief Create a buffer that is accessible by the CPU and GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] WIS_INLINE DX12Buffer CreateHostVisibleBuffer(size_t size, [[maybe_unused]] BufferFlags flags = BufferFlags::None) const;

    /// @brief Create a constant buffer that is accessible by the CPU and GPU
    /// This function is equivalent to CreateHostVisibleBuffer, but ensures that the buffer size is 256 byte aligned in debug mode
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] DX12Buffer CreateConstantBuffer(size_t size) const
    {
        wis::assert_debug(size % 256 == 0, wis::format("{} is nor 256 byte aligned", size));
        return CreateHostVisibleBuffer(size, BufferFlags::ConstantBuffer);
    }
    [[nodiscard]] DX12Texture CreateTexture(const TextureDescriptor& desc) const
    {
        using namespace river::flags;
        winrt::com_ptr<ID3D12Resource> rc;
        winrt::com_ptr<D3D12MA::Allocation> al;

        auto tex_desc = CD3DX12_RESOURCE_DESC1::Tex3D(
                            DXGI_FORMAT(desc.format), desc.width, desc.height, uint16_t(desc.depth), desc.mip_levels);
        D3D12MA::ALLOCATION_DESC all_desc = {};
        all_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

        D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COPY_DEST;

        allocator->CreateResource2(&all_desc, &tex_desc,
                                   state, nullptr,
                                   al.put(), __uuidof(*rc), rc.put_void());

        return DX12Texture{ std::move(rc), std::move(al) };
    }
};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_allocator.inl"
#endif
