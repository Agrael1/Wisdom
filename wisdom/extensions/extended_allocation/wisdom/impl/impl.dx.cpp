#ifndef WISDOM_EXTENDED_ALLOCATION_DX_CPP
#define WISDOM_EXTENDED_ALLOCATION_DX_CPP
#include <wisdom/wisdom_extended_allocation.h>

#if defined(WISDOM_DX12)
#include <d3dx12/d3dx12_core.h>
#include <d3dx12/d3dx12_property_format_table.h>

wis::ResultValue<wis::DX12Texture>
wis::DX12ExtendedAllocation::CreateGPUUploadTexture(const wis::DX12ResourceAllocator& allocator,
                                                    wis::TextureDesc desc,
                                                    wis::TextureState initial_state,
                                                    wis::MemoryFlags flags) const noexcept
{
    if (!supports_gpu_upload)
        return wis::make_result<FUNC, "GPU upload heap not supported by device">(E_INVALIDARG);

    D3D12_RESOURCE_DESC1 tex_desc;
    DX12ResourceAllocator::DX12FillTextureDesc(desc, tex_desc);

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = convert_dx(flags),
        .HeapType = convert_dx(wis::MemoryType::GPUUpload),
        .ExtraHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS
    };

    wis::com_ptr<ID3D12Resource> rc;
    wis::com_ptr<D3D12MA::Allocation> al;

    auto* hallocator = allocator.GetInternal().allocator.get();

    HRESULT hr = hallocator->CreateResource3(&all_desc, &tex_desc,
                                             convert_dx(initial_state), nullptr, 0, nullptr,
                                             al.put(), __uuidof(*rc), rc.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer Allocation failed">(hr);

    return DX12Buffer{ std::move(rc), std::move(al), hallocator };
}

wis::Result
wis::DX12ExtendedAllocation::WriteMemoryToSubresourceDirect(const void* host_data,
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
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to calculate row pitch">(hr);

    hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorSlicePitch(convert_dx(region.format), row_pitch, region.size.height, slice_pitch);
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to calculate slice pitch">(hr);

    D3D12_BOX box{
        .left = region.offset.width,
        .top = region.offset.height,
        .front = region.offset.depth_or_layers,
        .right = region.offset.width + region.size.width,
        .bottom = region.offset.height + region.size.height,
        .back = region.offset.depth_or_layers + region.size.depth_or_layers,
    };
    hr = resource->WriteToSubresource(dest_subresource, &box, host_data, row_pitch, slice_pitch);
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to write to subresource">(hr);
    return wis::success;
}
#endif // WISDOM_DX12
#endif // WISDOM_EXTENDED_ALLOCATION_CPP
