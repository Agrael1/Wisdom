#pragma once
#ifdef WISDOM_BUILD_BINARIES
#include <wisdom/dx12/dx12_allocator.h>
#endif // !WISDOM_HEADER_ONLY

#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_resource_helpers.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/util/misc.h>

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateBuffer(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept
{
    wis::com_ptr<ID3D12Resource> rc;
    wis::com_ptr<D3D12MA::Allocation> al;

    HRESULT hr = allocator->CreateResource2(&all_desc, &res_desc,
                                            state, nullptr,
                                            al.put(), __uuidof(*rc), rc.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer Allocation failed">(hr);

    return DX12Buffer{ std::move(rc), std::move(al), allocator };
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateCommitedBuffer(uint64_t size, BufferFlags flags) const noexcept
{
    uint64_t alignment = flags & BufferFlags::ConstantBuffer ? D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT : 1;
    size = wis::detail::aligned_size(size, alignment);

    return CreateBuffer({ .HeapType = D3D12_HEAP_TYPE_DEFAULT }, CD3DX12_RESOURCE_DESC1::Buffer(size), D3D12_RESOURCE_STATE_COMMON);
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateUploadBuffer(uint64_t size) const noexcept
{
    auto buffer = CreateBuffer({ .HeapType = D3D12_HEAP_TYPE_UPLOAD }, CD3DX12_RESOURCE_DESC1::Buffer(size), D3D12_RESOURCE_STATE_GENERIC_READ);
    return {
        buffer.status, DX12Buffer{ std::move(buffer.value) }
    };
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateReadbackBuffer(uint64_t size) const noexcept
{
    auto buffer = CreateBuffer({ .HeapType = D3D12_HEAP_TYPE_READBACK }, CD3DX12_RESOURCE_DESC1::Buffer(size), D3D12_RESOURCE_STATE_COPY_DEST);
    return {
        buffer.status, DX12Buffer{ std::move(buffer.value) }
    };
}

wis::ResultValue<wis::DX12Texture>
wis::DX12ResourceAllocator::CreateTexture(wis::TextureDesc desc) const noexcept
{
    CD3DX12_RESOURCE_DESC1 tex_desc;

    switch (desc.layout) {
    case wis::TextureLayout::Texture1D:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex1D(
                convert_dx(desc.format),
                desc.size.width, uint16_t(1),
                uint16_t(desc.mip_levels), convert_dx(desc.usage));
        break;
    default:
    case wis::TextureLayout::Texture2D:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(1),
                uint16_t(desc.mip_levels), 1, 0, convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture3D:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex3D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(desc.size.depth_or_layers),
                uint16_t(desc.mip_levels), convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture1DArray:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex1D(
                convert_dx(desc.format),
                desc.size.width, uint16_t(desc.size.depth_or_layers),
                uint16_t(desc.mip_levels), convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture2DArray:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(desc.size.depth_or_layers),
                uint16_t(desc.mip_levels), 1, 0, convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture2DMS:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(1),
                uint16_t(1), convert_dx(desc.sample_count), 4, convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture2DMSArray:
        tex_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(desc.size.depth_or_layers),
                uint16_t(1), convert_dx(desc.sample_count), 4, convert_dx(desc.usage));
        break;
    }

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = D3D12MA::ALLOCATION_FLAG_NONE,
        .HeapType = D3D12_HEAP_TYPE_DEFAULT,
    };

    return CreateBuffer(all_desc, tex_desc, D3D12_RESOURCE_STATE_COMMON);
}
