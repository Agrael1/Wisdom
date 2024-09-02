#pragma once
#ifdef WISDOM_BUILD_BINARIES
#include <wisdom/dx12/dx12_allocator.h>
#endif // !WISDOM_HEADER_ONLY

#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_resource_helpers.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/util/misc.h>

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateBuffer(uint64_t size, wis::BufferUsage usage, wis::MemoryType memory, wis::MemoryFlags mem_flags) const noexcept
{
    D3D12_RESOURCE_DESC1 buffer_desc;
    DX12FillBufferDesc(size, usage, buffer_desc);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
        .ExtraHeapFlags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS
    };

    return DX12CreateResource(all_desc, buffer_desc, D3D12_RESOURCE_STATE_COMMON);
}
wis::ResultValue<wis::DX12Texture>
wis::DX12ResourceAllocator::CreateTexture(const wis::TextureDesc& desc, wis::MemoryType memory, wis::MemoryFlags mem_flags) const noexcept
{
    D3D12_RESOURCE_DESC1 tex_desc;
    DX12FillTextureDesc(desc, tex_desc);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
        .ExtraHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS
    };

    return DX12CreateResource(all_desc, tex_desc, D3D12_RESOURCE_STATE_COMMON);
}

wis::AllocationInfo
wis::DX12ResourceAllocator::GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept
{
    D3D12_RESOURCE_DESC1 resource_desc;
    DX12FillTextureDesc(desc, resource_desc);

    D3D12_RESOURCE_ALLOCATION_INFO1 info;
    device->GetResourceAllocationInfo2(0, 1, &resource_desc, &info);

    return AllocationInfo{
        .size_bytes = info.SizeInBytes,
        .alignment_bytes = info.Alignment
    };
}
wis::AllocationInfo
wis::DX12ResourceAllocator::GetBufferAllocationInfo(uint64_t size, BufferUsage flags) const noexcept
{
    D3D12_RESOURCE_DESC1 resource_desc;
    DX12FillBufferDesc(size, flags, resource_desc);

    D3D12_RESOURCE_ALLOCATION_INFO1 info;
    device->GetResourceAllocationInfo2(0, 1, &resource_desc, &info);

    return AllocationInfo{
        .size_bytes = info.SizeInBytes,
        .alignment_bytes = info.Alignment
    };
}

wis::ResultValue<wis::DX12Memory>
wis::DX12ResourceAllocator::AllocateImageMemory(uint64_t size, wis::TextureUsage usage,
                                                wis::MemoryType memory,
                                                wis::MemoryFlags mem_flags) const noexcept

{
    auto info = GetTextureAllocationInfo({ .format = wis::DataFormat::RGBA8Unorm, .size = { 1, 1, 1 }, .mip_levels = 1, .usage = usage });

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
        .ExtraHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS
    };

    D3D12_RESOURCE_ALLOCATION_INFO alloc_info{
        .SizeInBytes = size,
        .Alignment = info.alignment_bytes,
    };

    wis::com_ptr<D3D12MA::Allocation> allocation;
    auto hr = allocator->AllocateMemory(&all_desc, &alloc_info, allocation.put());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Image memory allocation failed">(hr);

    return DX12Memory{
        allocator,
        std::move(allocation)
    };
}

wis::ResultValue<wis::DX12Memory>
wis::DX12ResourceAllocator::AllocateBufferMemory(uint64_t size, wis::BufferUsage usage,
                                                 wis::MemoryType memory,
                                                 wis::MemoryFlags mem_flags) const noexcept

{
    auto info = GetBufferAllocationInfo(size, usage);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
        .ExtraHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS
    };

    D3D12_RESOURCE_ALLOCATION_INFO alloc_info{
        .SizeInBytes = size,
        .Alignment = info.alignment_bytes,
    };

    wis::com_ptr<D3D12MA::Allocation> allocation;
    auto hr = allocator->AllocateMemory(&all_desc, &alloc_info, allocation.put());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer memory allocation failed">(hr);

    return DX12Memory{
        allocator,
        std::move(allocation),
    };
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::PlaceBuffer(DX12MemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept
{
    auto* alloc = std::get<1>(memory);
    D3D12_RESOURCE_DESC1 buffer_desc;
    DX12FillBufferDesc(size, usage, buffer_desc);

    wis::com_ptr<ID3D12Resource> rc;
    auto hr = allocator->CreateAliasingResource1(alloc, memory_offset, &buffer_desc,
                                                 D3D12_RESOURCE_STATE_COMMON, nullptr, rc.iid(), rc.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer Placement failed">(hr);

    return DX12Buffer{ std::move(rc), nullptr, nullptr };
}

wis::ResultValue<wis::DX12Texture>
wis::DX12ResourceAllocator::PlaceTexture(DX12MemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept
{
    auto* alloc = std::get<1>(memory);
    D3D12_RESOURCE_DESC1 tex_desc;
    DX12FillTextureDesc(desc, tex_desc);

    wis::com_ptr<ID3D12Resource> rc;
    auto hr = allocator->CreateAliasingResource1(alloc, memory_offset, &tex_desc,
                                                 D3D12_RESOURCE_STATE_COMMON, nullptr, rc.iid(), rc.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer Placement failed">(hr);

    return DX12Buffer{ std::move(rc), nullptr, nullptr };
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::DX12CreateResource(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept
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

void wis::DX12ResourceAllocator::DX12FillBufferDesc(uint64_t size, BufferUsage flags, D3D12_RESOURCE_DESC1& info) noexcept
{
    uint64_t alignment = flags & BufferUsage::ConstantBuffer ? D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT : 1;
    size = wis::detail::aligned_size(size, alignment);
    info = CD3DX12_RESOURCE_DESC1::Buffer(size);
}
void wis::DX12ResourceAllocator::DX12FillTextureDesc(const TextureDesc& desc, D3D12_RESOURCE_DESC1& info) noexcept
{
    switch (desc.layout) {
    case wis::TextureLayout::Texture1D:
        info = CD3DX12_RESOURCE_DESC1::Tex1D(
                convert_dx(desc.format),
                desc.size.width, uint16_t(1),
                uint16_t(desc.mip_levels), convert_dx(desc.usage));
        break;
    default:
    case wis::TextureLayout::Texture2D:
        info = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(1),
                uint16_t(desc.mip_levels), 1, 0, convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture3D:
        info = CD3DX12_RESOURCE_DESC1::Tex3D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(desc.size.depth_or_layers),
                uint16_t(desc.mip_levels), convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture1DArray:
        info = CD3DX12_RESOURCE_DESC1::Tex1D(
                convert_dx(desc.format),
                desc.size.width, uint16_t(desc.size.depth_or_layers),
                uint16_t(desc.mip_levels), convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture2DArray:
        info = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(desc.size.depth_or_layers),
                uint16_t(desc.mip_levels), 1, 0, convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture2DMS:
        info = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(1),
                uint16_t(1), convert_dx(desc.sample_count), 4, convert_dx(desc.usage));
        break;
    case wis::TextureLayout::Texture2DMSArray:
        info = CD3DX12_RESOURCE_DESC1::Tex2D(
                convert_dx(desc.format),
                desc.size.width,
                desc.size.height,
                uint16_t(desc.size.depth_or_layers),
                uint16_t(1), convert_dx(desc.sample_count), 4, convert_dx(desc.usage));
        break;
    }
}
