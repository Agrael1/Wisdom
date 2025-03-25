#ifndef WISDOM_EXTENDED_ALLOCATION_DX_CPP
#define WISDOM_EXTENDED_ALLOCATION_DX_CPP
#include <wisdom/wisdom_extended_allocation.hpp>

#if defined(WISDOM_DX12)
#ifndef WISDOM_MODULE_DECL
#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_property_format_table.h>
#endif // !WISDOM_MODULE_DECL

wis::DX12Texture
wis::ImplDX12ExtendedAllocation::CreateGPUUploadTexture(wis::Result& result, const wis::DX12ResourceAllocator& allocator,
                                                        wis::TextureDesc desc,
                                                        wis::TextureState initial_state,
                                                        wis::MemoryFlags flags) const noexcept
{
    DX12Texture out_texture;
    auto& internal = out_texture.GetMutableInternal();
    auto& mem_internal = internal.memory.GetMutableInternal();

    if (!supports_gpu_upload) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "GPU upload heap not supported by device">(E_INVALIDARG);
        return out_texture;
    }

    D3D12_RESOURCE_DESC1 tex_desc;
    DX12ResourceAllocator::DX12FillTextureDesc(desc, tex_desc);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(flags),
        .HeapType = convert_dx(wis::MemoryType::GPUUpload),
        .ExtraHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS
    };
    HRESULT hr = allocator.GetInternal().allocator->CreateResource3(&all_desc, &tex_desc,
                                                                    convert_dx(initial_state), nullptr, 0, nullptr,
                                                                    mem_internal.allocation.put(), __uuidof(*internal.resource), internal.resource.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Buffer Allocation failed">(hr);
        return out_texture;
    }
    mem_internal.allocator = allocator.GetInternal().allocator;
    return out_texture;
}

wis::Result
wis::ImplDX12ExtendedAllocation::WriteMemoryToSubresourceDirect(const void* host_data,
                                                                wis::DX12TextureView dst_texture,
                                                                wis::TextureState initial_state,
                                                                wis::TextureRegion region) const noexcept
{
    auto resource = std::get<0>(dst_texture);
    auto texture_desc = resource->GetDesc();
    UINT dest_subresource = D3D12CalcSubresource(region.mip, region.array_layer, 0u, texture_desc.MipLevels, texture_desc.DepthOrArraySize);

    UINT row_pitch = 0;
    UINT slice_pitch = 0;
    auto hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorRowPitch(convert_dx(region.format), region.size.width, row_pitch);
    if (!wis::succeeded(hr)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to calculate row pitch">(hr);
    }

    hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorSlicePitch(convert_dx(region.format), row_pitch, region.size.height, slice_pitch);
    if (!wis::succeeded(hr)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to calculate slice pitch">(hr);
    }

    D3D12_BOX box{
        .left = region.offset.width,
        .top = region.offset.height,
        .front = region.offset.depth_or_layers,
        .right = region.offset.width + region.size.width,
        .bottom = region.offset.height + region.size.height,
        .back = region.offset.depth_or_layers + region.size.depth_or_layers,
    };
    hr = resource->WriteToSubresource(dest_subresource, &box, host_data, row_pitch, slice_pitch);
    if (!wis::succeeded(hr)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to write to subresource">(hr);
    }
    return wis::success;
}
#endif // WISDOM_DX12
#endif // WISDOM_EXTENDED_ALLOCATION_CPP
