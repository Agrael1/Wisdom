#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_barrier.h>
#include <wisdom/dx12/dx12_resource.h>
#include <D3D12MemAlloc.h>
#include <wisdom/global/assertions.h>
#include <wisdom/dx12/dx12_format.h>
#include <d3dx12/d3dx12.h>
#endif

namespace wis {
class DX12ResourceAllocator;
class DX12Device;

template<>
class Internal<DX12ResourceAllocator>
{
public:
    winrt::com_ptr<D3D12MA::Allocator> allocator;
};

/// @brief Resource allocator for DX12
WIS_EXPORT class DX12ResourceAllocator : public QueryInternal<DX12ResourceAllocator>
{
public:
    DX12ResourceAllocator() = default;
    WIS_INLINE explicit DX12ResourceAllocator(const wis::DX12Device& device) noexcept;
    operator bool() const noexcept
    {
        return bool(allocator);
    }

public:
    /// @brief Create a buffer that is persistently mapped to the GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] WIS_INLINE DX12Buffer
    CreatePersistentBuffer(size_t size, [[maybe_unused]] BufferFlags flags = BufferFlags::None) const noexcept;

    /// @brief Create a buffer that is accessible by the CPU and serves as a staging buffer for GPU uploads
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] WIS_INLINE DX12Buffer
    CreateUploadBuffer(size_t size) const noexcept;

    /// @brief Create a buffer that is accessible by the CPU and GPU
    /// @param size Size of the buffer
    /// @param flags Type of buffer
    /// @return Buffer object
    [[nodiscard]] DX12Buffer
    CreateHostVisibleBuffer(size_t size, [[maybe_unused]] BufferFlags flags = BufferFlags::None) const noexcept
    {
        return CreateUploadBuffer(size);
    }

    /// @brief Create a constant buffer that is accessible by the CPU and GPU
    /// This function is equivalent to CreateHostVisibleBuffer, but ensures that the buffer size is 256 byte aligned in debug mode
    /// @param size Size of the buffer
    /// @return Buffer object
    [[nodiscard]] DX12Buffer
    CreateConstantBuffer(size_t size) const noexcept
    {
        wis::assert_debug(size % 256 == 0, wis::format("{} is not 256 byte aligned", size));
        return CreateHostVisibleBuffer(size, BufferFlags::ConstantBuffer);
    }

    [[nodiscard]] DX12Texture
    CreateTexture(const TextureDescriptor& desc, TextureFlags flags = TextureFlags::None) const noexcept
    {
        using namespace wis::flags;
        winrt::com_ptr<ID3D12Resource> rc;
        winrt::com_ptr<D3D12MA::Allocation> al;

        CD3DX12_RESOURCE_DESC1 tex_desc{};
        switch (desc.type) {
        case TextureType::Texture1D:
            tex_desc = CD3DX12_RESOURCE_DESC1::Tex1D(
                    DXGI_FORMAT(desc.format),
                    desc.width, 1,
                    uint16_t(desc.mip_levels));
            break;
        case TextureType::Texture2D:
            tex_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
                    DXGI_FORMAT(desc.format),
                    desc.width,
                    desc.height,
                    uint16_t(1),
                    uint16_t(desc.mip_levels));
        case TextureType::Texture3D:
            tex_desc = CD3DX12_RESOURCE_DESC1::Tex3D(
                    DXGI_FORMAT(desc.format),
                    desc.width,
                    desc.height,
                    uint16_t(desc.depth),
                    uint16_t(desc.mip_levels));
            break;
        case TextureType::Texture1DArray:
            tex_desc = CD3DX12_RESOURCE_DESC1::Tex1D(
                    DXGI_FORMAT(desc.format),
                    desc.width, uint16_t(desc.array_size),
                    uint16_t(desc.mip_levels));
            break;
        case TextureType::Texture2DArray:
            tex_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
                    DXGI_FORMAT(desc.format),
                    desc.width,
                    desc.height,
                    uint16_t(desc.array_size),
                    uint16_t(desc.mip_levels));
            break;
        default:
            assert(false && "TODO: make multisampled textures");
            break;
        }
        tex_desc.Flags = convert_dx(flags);

        D3D12MA::ALLOCATION_DESC all_desc = {};
        all_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

        D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COPY_DEST;

        return wis::succeeded(allocator->CreateResource2(&all_desc, &tex_desc,
                                                         state, nullptr,
                                                         al.put(), __uuidof(*rc), rc.put_void()))
                ? DX12Texture{ std::move(rc), std::move(al) }
                : DX12Texture{};
    }

    [[nodiscard]] DX12Texture
    CreateDepthStencilTexture(DepthDescriptor desc) const noexcept
    {
        const TextureDescriptor t_desc{
            .type = TextureType::Texture2D,
            .format = desc.format,
            .width = desc.width,
            .height = desc.height,
            .depth = 1,
            .array_size = 1,
            .mip_levels = 1,
        };
        return CreateTexture(t_desc, TextureFlags::DepthStencil);
    }
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_allocator.inl"
#endif
