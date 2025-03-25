#ifndef WIS_DX12_ALLOCATOR_CPP
#define WIS_DX12_ALLOCATOR_CPP

#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_allocator.h>

#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_resource_helpers.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/util/misc.h>
#include <wisdom/dx12/dx12_checks.h>
#endif

wis::DX12Buffer
wis::ImplDX12ResourceAllocator::CreateBuffer(wis::Result& result, uint64_t size, wis::BufferUsage usage, wis::MemoryType memory, wis::MemoryFlags mem_flags) const noexcept
{
    D3D12_RESOURCE_DESC1 buffer_desc;
    DX12FillBufferDesc(size, usage, buffer_desc);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
    };
    if (usage & wis::BufferUsage::AccelerationStructureBuffer) {
        return DX12CreateResource2(result, all_desc, buffer_desc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);
    }
    return DX12CreateResource(result, all_desc, buffer_desc, D3D12_RESOURCE_STATE_COMMON);
}
wis::DX12Texture
wis::ImplDX12ResourceAllocator::CreateTexture(wis::Result& result, const wis::TextureDesc& desc, wis::MemoryType memory, wis::MemoryFlags mem_flags) const noexcept
{
    D3D12_RESOURCE_DESC1 tex_desc;
    DX12FillTextureDesc(desc, tex_desc);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
    };

    return DX12CreateResource(result, all_desc, tex_desc, D3D12_RESOURCE_STATE_COMMON);
}

wis::AllocationInfo
wis::ImplDX12ResourceAllocator::GetTextureAllocationInfo(const wis::TextureDesc& desc) const noexcept
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
wis::ImplDX12ResourceAllocator::GetBufferAllocationInfo(uint64_t size, BufferUsage flags) const noexcept
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

wis::DX12Memory
wis::ImplDX12ResourceAllocator::AllocateTextureMemory(wis::Result& result, uint64_t size, wis::TextureUsage usage,
        wis::MemoryType memory,
        wis::MemoryFlags mem_flags) const noexcept

{
    DX12Memory out_memory;
    auto& internal = out_memory.GetMutableInternal();

    D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_DENY_BUFFERS;
    if ((mem_flags & MemoryFlags::Exportable)) {
        if (memory != MemoryType::Default) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Exportable memory must be Default heap type">(E_INVALIDARG);
            return out_memory;
        }
        flags |= D3D12_HEAP_FLAG_SHARED;
    }
    if (!(usage & (wis::TextureUsage::RenderTarget | wis::TextureUsage::DepthStencil))) {
        flags |= D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES;
    } else if (!(usage & ~(wis::TextureUsage::RenderTarget | wis::TextureUsage::DepthStencil))) {
        flags |= D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES;
    }

    auto info = GetTextureAllocationInfo({ .format = wis::DataFormat::RGBA8Unorm, .size = { 1, 1, 1 }, .mip_levels = 1, .usage = usage });

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
        .ExtraHeapFlags = flags
    };

    D3D12_RESOURCE_ALLOCATION_INFO alloc_info{
        .SizeInBytes = size,
        .Alignment = info.alignment_bytes,
    };
    auto hr = allocator->AllocateMemory(&all_desc, &alloc_info, internal.allocation.put());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Image memory allocation failed">(hr);
    }

    internal.allocator = allocator; // Copy allocator to memory
    return out_memory;
}

wis::DX12Memory
wis::ImplDX12ResourceAllocator::AllocateBufferMemory(wis::Result& result, uint64_t size, wis::BufferUsage usage,
        wis::MemoryType memory,
        wis::MemoryFlags mem_flags) const noexcept

{
    DX12Memory out_memory;
    auto& internal = out_memory.GetMutableInternal();

    D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
    if ((mem_flags & MemoryFlags::Exportable)) {
        if (memory != MemoryType::Default) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Exportable memory must be Default heap type">(E_INVALIDARG);
            return out_memory;
        }
        flags |= D3D12_HEAP_FLAG_SHARED;
    }

    auto info = GetBufferAllocationInfo(size, usage);
    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(mem_flags),
        .HeapType = convert_dx(memory),
        .ExtraHeapFlags = flags
    };

    D3D12_RESOURCE_ALLOCATION_INFO alloc_info{
        .SizeInBytes = size,
        .Alignment = info.alignment_bytes,
    };

    auto hr = allocator->AllocateMemory(&all_desc, &alloc_info, internal.allocation.put());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Buffer memory allocation failed">(hr);
    }

    internal.allocator = allocator; // Copy allocator to memory
    return out_memory;
}

wis::DX12Buffer
wis::ImplDX12ResourceAllocator::PlaceBuffer(wis::Result& result, DX12MemoryView memory, uint64_t memory_offset, uint64_t size, wis::BufferUsage usage) const noexcept
{
    wis::DX12Buffer buffer;
    auto& internal = buffer.GetMutableInternal();

    auto* alloc = std::get<1>(memory);
    D3D12_RESOURCE_DESC1 buffer_desc;
    DX12FillBufferDesc(size, usage, buffer_desc);

    auto hr = allocator->CreateAliasingResource1(alloc, memory_offset, &buffer_desc,
              D3D12_RESOURCE_STATE_COMMON, nullptr, internal.resource.iid(), internal.resource.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Buffer Placement failed">(hr);
    }
    return buffer;
}

wis::DX12Texture
wis::ImplDX12ResourceAllocator::PlaceTexture(wis::Result& result, DX12MemoryView memory, uint64_t memory_offset, const wis::TextureDesc& desc) const noexcept
{
    wis::DX12Texture texture;
    auto& internal = texture.GetMutableInternal();

    auto* alloc = std::get<1>(memory);
    D3D12_RESOURCE_DESC1 tex_desc;
    DX12FillTextureDesc(desc, tex_desc);

    auto hr = allocator->CreateAliasingResource1(alloc, memory_offset, &tex_desc,
              D3D12_RESOURCE_STATE_COMMON, nullptr, internal.resource.iid(), internal.resource.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Buffer Placement failed">(hr);
    }
    return texture;
}

wis::DX12Buffer
wis::ImplDX12ResourceAllocator::DX12CreateResource(wis::Result& result, const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept
{
    wis::DX12Buffer buffer;
    auto& internal = buffer.GetMutableInternal();
    auto& memory_internal = internal.memory.GetMutableInternal();

    HRESULT hr = allocator->CreateResource2(&all_desc, &res_desc,
                                            state, nullptr,
                                            memory_internal.allocation.put(), __uuidof(*internal.resource), internal.resource.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Buffer Allocation failed">(hr);
    }

    memory_internal.allocator = allocator; // Copy allocator to memory

    return buffer;
}

wis::DX12Buffer
wis::ImplDX12ResourceAllocator::DX12CreateResource2(wis::Result& result, const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept
{
    wis::DX12Buffer buffer;
    auto& internal = buffer.GetMutableInternal();
    auto& memory_internal = internal.memory.GetMutableInternal();

    HRESULT hr = allocator->CreateResource3(&all_desc, &res_desc,
                                            D3D12_BARRIER_LAYOUT_UNDEFINED, nullptr, 0, nullptr,
                                            memory_internal.allocation.put(), __uuidof(*internal.resource), internal.resource.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Buffer Allocation failed">(hr);
    }

    memory_internal.allocator = allocator; // Copy allocator to memory

    return buffer;
}

void wis::ImplDX12ResourceAllocator::DX12FillBufferDesc(uint64_t size, BufferUsage flags, D3D12_RESOURCE_DESC1& info) noexcept
{
    uint64_t alignment = flags & wis::BufferUsage::ConstantBuffer ? D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT : 1;
    size = wis::detail::aligned_size(size, alignment);
    info = CD3DX12_RESOURCE_DESC1::Buffer(size, convert_dx(flags));
}
void wis::ImplDX12ResourceAllocator::DX12FillTextureDesc(const TextureDesc& desc, D3D12_RESOURCE_DESC1& info) noexcept
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

#endif // WIS_DX12_ALLOCATOR_CPP
