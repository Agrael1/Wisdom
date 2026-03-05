#ifndef WIS_DX12_DESCRIPTOR_HEAP_CPP
#define WIS_DX12_DESCRIPTOR_HEAP_CPP

#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <bit>
#include <algorithm>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyDescriptorHeap(WisDX12DescriptorHeap* self)
{
    auto& heap = *reinterpret_cast<wis::impl::DX12DescriptorHeapImpl*>(self);
    if (!heap.descriptor_heap) {
        return;
    }
    heap.descriptor_heap->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisDX12DescriptorHeapGetCPUHandle(const WisDX12DescriptorHeap* self)
{
    auto& heap = *reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(self);
    return reinterpret_cast<void*>(heap.descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteConstantBuffer(const WisDX12DescriptorHeap*    self,
                                                                           const WisConstantBufferBinding* data,
                                                                           uint32_t                        index)
{
    auto&                           heap = *reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(self);
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc{
        .BufferLocation = { data->buffer_address },
        .SizeInBytes    = static_cast<UINT>(data->size_bytes),
    };
    heap.device->CreateConstantBufferView(&cbv_desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteStructuredBuffer(const WisDX12DescriptorHeap* self,
                                                                             WisDX12BufferView            buffer,
                                                                             const WisBufferBinding*      data,
                                                                             uint32_t                     index)
{
    auto& heap     = *reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(self);
    auto* resource = std::bit_cast<ID3D12Resource*>(buffer);

    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{
        .Format                  = DXGI_FORMAT_UNKNOWN, // must be UNKNOWN for structured buffers
        .ViewDimension           = D3D12_SRV_DIMENSION_BUFFER,
        .Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
        .Buffer                  = {
                                    .FirstElement        = data->array_offset,
                                    .NumElements         = data->structure_count,
                                    .StructureByteStride = data->stride_bytes,
                                    .Flags               = D3D12_BUFFER_SRV_FLAG_NONE,
                                    },
    };
    heap.device->CreateShaderResourceView(resource, &srv_desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteRWStructuredBuffer(const WisDX12DescriptorHeap* self,
                                                                               WisDX12BufferView            buffer,
                                                                               const WisBufferBinding*      data,
                                                                               uint32_t                     index)
{
    auto& heap     = *reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(self);
    auto* resource = std::bit_cast<ID3D12Resource*>(buffer);

    D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{
        .Format        = DXGI_FORMAT_UNKNOWN, // must be UNKNOWN for structured buffers
        .ViewDimension = D3D12_UAV_DIMENSION_BUFFER,
        .Buffer        = {
                          .FirstElement        = data->array_offset,
                          .NumElements         = data->structure_count,
                          .StructureByteStride = data->stride_bytes,
                          .Flags               = D3D12_BUFFER_UAV_FLAG_NONE,
                          },
    };
    heap.device->CreateUnorderedAccessView(resource, nullptr, &uav_desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteSampler(const WisDX12DescriptorHeap* self,
                                                                    const WisSamplerDesc*        sampler,
                                                                    uint32_t                     index)
{
    auto& heap = *reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(self);

    auto min_filter   = !sampler->is_anisotropic ? wis::detail::convert_dx(sampler->min_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto mag_filter   = !sampler->is_anisotropic ? wis::detail::convert_dx(sampler->mag_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto basic_filter = D3D12_ENCODE_BASIC_FILTER(min_filter, mag_filter, wis::detail::convert_dx(sampler->mip_filter), D3D12_FILTER_REDUCTION_TYPE::D3D12_FILTER_REDUCTION_TYPE_STANDARD);
    auto filter       = D3D12_FILTER(sampler->is_anisotropic * D3D12_ANISOTROPIC_FILTERING_BIT | basic_filter);

    constexpr static std::array<float, 4> border_colors[] = {
        { 0.0f, 0.0f, 0.0f, 0.0f }, // Transparent black
        { 0.0f, 0.0f, 0.0f, 1.0f }, // Opaque black
        { 1.0f, 1.0f, 1.0f, 1.0f }, // Opaque white
    };
    const auto& border_color = border_colors[static_cast<size_t>(sampler->static_border_color)];

    D3D12_SAMPLER_DESC sampler_desc{
        .Filter         = filter,
        .AddressU       = wis::detail::convert_dx(sampler->address_u),
        .AddressV       = wis::detail::convert_dx(sampler->address_v),
        .AddressW       = wis::detail::convert_dx(sampler->address_w),
        .MipLODBias     = sampler->mip_lod_bias,
        .MaxAnisotropy  = std::clamp(sampler->max_anisotropy, 1u, uint32_t(D3D12_MAX_MAXANISOTROPY)),
        .ComparisonFunc = wis::detail::convert_dx(sampler->comparison_op),
        .BorderColor    = { border_color[0], border_color[1], border_color[2], border_color[3] },
        .MinLOD         = sampler->min_lod,
        .MaxLOD         = sampler->max_lod,
    };
    heap.device->CreateSampler(&sampler_desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}
#endif // WIS_DX12_DESCRIPTOR_HEAP_CPP