#ifndef WIS_DX12_DESCRIPTOR_HEAP_CPP
#define WIS_DX12_DESCRIPTOR_HEAP_CPP

#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/util/allocation.hpp>
#include <array>
#include <bit>
#include <algorithm>

namespace wis::detail {
inline DXGI_FORMAT DX12GetSRVFormat(const WisTextureBinding& binding) noexcept
{
    switch (binding.format) {
    case WisDataFormatD16Unorm:
        return DXGI_FORMAT_R16_UNORM;
    case WisDataFormatD24UnormS8Uint:
        if (binding.flags & WisTextureBindingFlagsStencilView) {
            return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        }
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case WisDataFormatD32Float:
        return DXGI_FORMAT_R32_FLOAT;
    case WisDataFormatD32FloatS8Uint:
        if (binding.flags & WisTextureBindingFlagsStencilView) {
            return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
        }
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    default:
        return wis::detail::DX12Convert(binding.format);
    }
}

inline uint32_t
DX12GetComponentMapping(WisComponentMapping mapping) noexcept
{
    uint32_t r = mapping.r ? wis::detail::DX12Convert(mapping.r) : D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0;
    uint32_t g = mapping.g ? wis::detail::DX12Convert(mapping.g) : D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1;
    uint32_t b = mapping.b ? wis::detail::DX12Convert(mapping.b) : D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2;
    uint32_t a = mapping.a ? wis::detail::DX12Convert(mapping.a) : D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3;
    return D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(r, g, b, a);
}

inline D3D12_SHADER_RESOURCE_VIEW_DESC
DX12FillTextureSRVDesc(const WisTextureBinding& binding)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{
        .Format                  = DX12GetSRVFormat(binding),
        .Shader4ComponentMapping = DX12GetComponentMapping(binding.component_mapping),
    };

    switch (binding.layout) {
    case WisTextureLayoutTexture1D:
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
        srv_desc.Texture1D     = {
                .MostDetailedMip     = binding.range.base_mip_level,
                .MipLevels           = binding.range.mip_level_count,
                .ResourceMinLODClamp = 0.0f,
        };
        break;
    case WisTextureLayoutTexture1DArray:
        srv_desc.ViewDimension  = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
        srv_desc.Texture1DArray = {
            .MostDetailedMip     = binding.range.base_mip_level,
            .MipLevels           = binding.range.mip_level_count,
            .FirstArraySlice     = binding.range.base_array_layer,
            .ArraySize           = binding.range.array_layer_count,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case WisTextureLayoutTexture2D:
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D     = {
                .MostDetailedMip     = binding.range.base_mip_level,
                .MipLevels           = binding.range.mip_level_count,
                .PlaneSlice          = binding.range.plane_slice,
                .ResourceMinLODClamp = 0.0f,
        };
        break;
    default:
    case WisTextureLayoutTexture2DArray:
        srv_desc.ViewDimension  = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
        srv_desc.Texture2DArray = {
            .MostDetailedMip     = binding.range.base_mip_level,
            .MipLevels           = binding.range.mip_level_count,
            .FirstArraySlice     = binding.range.base_array_layer,
            .ArraySize           = binding.range.array_layer_count,
            .PlaneSlice          = binding.range.plane_slice,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case WisTextureLayoutTexture3D:
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
        srv_desc.Texture3D     = {
                .MostDetailedMip     = binding.range.base_mip_level,
                .MipLevels           = binding.range.mip_level_count,
                .ResourceMinLODClamp = 0.0f,
        };
        break;
    case WisTextureLayoutTexture2DMS:
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
        srv_desc.Texture2DMS   = {
              .UnusedField_NothingToDefine = 0,
        };
        break;
    case WisTextureLayoutTexture2DMSArray:
        srv_desc.ViewDimension    = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
        srv_desc.Texture2DMSArray = {
            .FirstArraySlice = binding.range.base_array_layer,
            .ArraySize       = binding.range.array_layer_count,
        };
        break;
    case WisTextureLayoutTextureCube:
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
        srv_desc.TextureCube   = {
              .MostDetailedMip     = binding.range.base_mip_level,
              .MipLevels           = binding.range.mip_level_count,
              .ResourceMinLODClamp = 0.0f,
        };
        break;
    case WisTextureLayoutTextureCubeArray:
        srv_desc.ViewDimension    = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
        srv_desc.TextureCubeArray = {
            .MostDetailedMip     = binding.range.base_mip_level,
            .MipLevels           = binding.range.mip_level_count,
            .First2DArrayFace    = binding.range.base_array_layer,
            .NumCubes            = binding.range.array_layer_count / 6u, // cube array must have array layer count divisible by 6
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    }

    return srv_desc;
}

inline D3D12_UNORDERED_ACCESS_VIEW_DESC
DX12FillTextureUAVDesc(const WisTextureBinding& binding)
{
    D3D12_UNORDERED_ACCESS_VIEW_DESC srv_desc{
        .Format = DX12GetSRVFormat(binding),
    };

    switch (binding.layout) {
    case WisTextureLayoutTexture1D:
        srv_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
        srv_desc.Texture1D     = {
                .MipSlice = binding.range.base_mip_level,
        };
        break;
    case WisTextureLayoutTexture1DArray:
        srv_desc.ViewDimension  = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
        srv_desc.Texture1DArray = {
            .MipSlice        = binding.range.base_mip_level,
            .FirstArraySlice = binding.range.base_array_layer,
            .ArraySize       = binding.range.array_layer_count,
        };
        break;
    case WisTextureLayoutTexture2D:
        srv_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D     = {
                .MipSlice   = binding.range.base_mip_level,
                .PlaneSlice = binding.range.plane_slice,
        };
        break;
    case WisTextureLayoutTexture2DArray:
        srv_desc.ViewDimension  = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
        srv_desc.Texture2DArray = {
            .MipSlice        = binding.range.base_mip_level,
            .FirstArraySlice = binding.range.base_array_layer,
            .ArraySize       = binding.range.array_layer_count,
            .PlaneSlice      = binding.range.plane_slice,
        };
        break;
    case WisTextureLayoutTexture3D:
        srv_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
        srv_desc.Texture3D     = {
                .MipSlice    = binding.range.base_mip_level,
                .FirstWSlice = binding.range.base_array_layer,
                .WSize       = binding.range.array_layer_count,
        };
        break;
    case WisTextureLayoutTexture2DMS:
        srv_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMS;
        srv_desc.Texture2DMS   = {
              .UnusedField_NothingToDefine = 0,
        };
        break;
    case WisTextureLayoutTexture2DMSArray:
        srv_desc.ViewDimension    = D3D12_UAV_DIMENSION_TEXTURE2DMSARRAY;
        srv_desc.Texture2DMSArray = {
            .FirstArraySlice = binding.range.base_array_layer,
            .ArraySize       = binding.range.array_layer_count,
        };
        break;
    }

    return srv_desc;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyDescriptorHeap(WisDX12DescriptorHeap* self)
{
    auto& heap = wis::from_handle_ref<wis::impl::DX12DescriptorHeapImpl>(self);
    if (!heap.descriptor_heap) {
        return;
    }
    heap.descriptor_heap->Release();
    heap.descriptor_heap = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyViewHeap(WisDX12ViewHeap* self)
{
    auto& heap = wis::from_handle_ref<wis::impl::DX12ViewHeapImpl>(self);
    if (!heap.view_heap) {
        return;
    }
    heap.view_heap->Release();
    heap.view_heap = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisDX12DescriptorHeapGetCPUHandle(const WisDX12DescriptorHeap* self)
{
    auto& heap = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
    return reinterpret_cast<void*>(heap.descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteConstantBuffer(const WisDX12DescriptorHeap*    self,
                                                                           const WisConstantBufferBinding* data,
                                                                           uint32_t                        index)
{
    auto&                           heap = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
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
    auto& heap     = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
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
    auto& heap     = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
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
    auto& heap = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);

    auto min_filter     = !sampler->is_anisotropic ? wis::detail::DX12Convert(sampler->min_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto mag_filter     = !sampler->is_anisotropic ? wis::detail::DX12Convert(sampler->mag_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto reduction_mode = sampler->comparison_op != WisCompareOpNone
            ? D3D12_FILTER_REDUCTION_TYPE::D3D12_FILTER_REDUCTION_TYPE_COMPARISON
            : wis::detail::DX12Convert(sampler->reduction_mode);

    auto basic_filter = D3D12_ENCODE_BASIC_FILTER(min_filter, mag_filter, wis::detail::DX12Convert(sampler->mip_filter), reduction_mode);
    auto filter       = D3D12_FILTER(sampler->is_anisotropic * D3D12_ANISOTROPIC_FILTERING_BIT | basic_filter);

    constexpr static std::array<float, 4> border_colors[] = {
        { 0.0f, 0.0f, 0.0f, 0.0f }, // Transparent black
        { 0.0f, 0.0f, 0.0f, 1.0f }, // Opaque black
        { 1.0f, 1.0f, 1.0f, 1.0f }, // Opaque white
    };
    const auto& border_color = border_colors[static_cast<size_t>(sampler->static_border_color)];

    D3D12_SAMPLER_DESC sampler_desc{
        .Filter         = filter,
        .AddressU       = wis::detail::DX12Convert(sampler->address_u),
        .AddressV       = wis::detail::DX12Convert(sampler->address_v),
        .AddressW       = wis::detail::DX12Convert(sampler->address_w),
        .MipLODBias     = sampler->mip_lod_bias,
        .MaxAnisotropy  = std::clamp(sampler->max_anisotropy, 1u, uint32_t(D3D12_MAX_MAXANISOTROPY)),
        .ComparisonFunc = wis::detail::DX12Convert(sampler->comparison_op),
        .BorderColor    = { border_color[0], border_color[1], border_color[2], border_color[3] },
        .MinLOD         = sampler->min_lod,
        .MaxLOD         = sampler->max_lod,
    };
    heap.device->CreateSampler(&sampler_desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteTexture(const WisDX12DescriptorHeap* self,
                                                                    WisDX12TextureView           texture,
                                                                    const WisTextureBinding*     data,
                                                                    uint32_t                     index)
{
    auto&                           heap     = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
    auto*                           resource = std::bit_cast<ID3D12Resource*>(texture);
    D3D12_SHADER_RESOURCE_VIEW_DESC desc     = wis::detail::DX12FillTextureSRVDesc(*data);
    heap.device->CreateShaderResourceView(resource, &desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteRWTexture(const WisDX12DescriptorHeap* self,
                                                                      WisDX12TextureView           texture,
                                                                      const WisTextureBinding*     data,
                                                                      uint32_t                     index)
{
    auto&                            heap     = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
    auto*                            resource = std::bit_cast<ID3D12Resource*>(texture);
    D3D12_UNORDERED_ACCESS_VIEW_DESC desc     = wis::detail::DX12FillTextureUAVDesc(*data);
    heap.device->CreateUnorderedAccessView(resource, nullptr, &desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DescriptorHeapWriteAccelerationStructure(const WisDX12DescriptorHeap* self,
                                                                                  uint64_t                     address,
                                                                                  uint32_t                     index)
{
    auto&                           heap = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
    D3D12_SHADER_RESOURCE_VIEW_DESC desc{
        .Format                          = DXGI_FORMAT_UNKNOWN,
        .ViewDimension                   = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE,
        .Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
        .RaytracingAccelerationStructure = { address }
    };
    heap.device->CreateShaderResourceView(nullptr, &desc, { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DescriptorHeapCopyDescriptors(const WisDX12DescriptorHeap* self,
                                                                  uint32_t                     dst_index,
                                                                  const void*                  src_ptr,
                                                                  uint32_t                     src_index,
                                                                  uint32_t                     count)
{
    auto& heap = wis::from_handle_ref<const wis::impl::DX12DescriptorHeapImpl>(self);
    heap.device->CopyDescriptorsSimple(
            count,
            { heap.cpu_handle.ptr + static_cast<uint64_t>(dst_index) * heap.descriptor_size },
            { std::bit_cast<std::size_t>(src_ptr) + static_cast<uint64_t>(src_index) * heap.descriptor_size },
            heap.type);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12ViewHeapWriteRenderTarget(const WisDX12ViewHeap*     self,
                                                                  WisDX12TextureView         texture,
                                                                  const WisRenderTargetDesc* render_target,
                                                                  uint32_t                   index)
{
    auto&                         heap = wis::from_handle_ref<const wis::impl::DX12ViewHeapImpl>(self);
    D3D12_RENDER_TARGET_VIEW_DESC rtv_desc{
        .Format = wis::detail::DX12Convert(render_target->format),
    };
    switch (render_target->layout) {
    case WisTextureLayoutTexture1D:
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
        rtv_desc.Texture1D     = {
                .MipSlice = render_target->mip_level
        };
        break;
    case WisTextureLayoutTexture1DArray:
        rtv_desc.ViewDimension  = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
        rtv_desc.Texture1DArray = {
            .MipSlice        = render_target->mip_level,
            .FirstArraySlice = render_target->base_array_layer,
            .ArraySize       = render_target->array_layer_count
        };
        break;
    case WisTextureLayoutTexture2D:
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtv_desc.Texture2D     = {
                .MipSlice   = render_target->mip_level,
                .PlaneSlice = render_target->plane_slice
        };
        break;
    case WisTextureLayoutTexture2DArray:
        rtv_desc.ViewDimension  = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
        rtv_desc.Texture2DArray = {
            .MipSlice        = render_target->mip_level,
            .FirstArraySlice = render_target->base_array_layer,
            .ArraySize       = render_target->array_layer_count,
            .PlaneSlice      = render_target->plane_slice
        };
        break;
    case WisTextureLayoutTexture2DMS:
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
        break;
    case WisTextureLayoutTexture2DMSArray:
        rtv_desc.ViewDimension    = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
        rtv_desc.Texture2DMSArray = {
            .FirstArraySlice = render_target->base_array_layer,
            .ArraySize       = render_target->array_layer_count
        };
        break;
    case WisTextureLayoutTexture3D:
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
        rtv_desc.Texture3D     = {
                .MipSlice    = render_target->mip_level,
                .FirstWSlice = render_target->base_array_layer,
                .WSize       = render_target->array_layer_count
        };
        break;
    default:
        break;
    }

    heap.device->CreateRenderTargetView(
            std::bit_cast<ID3D12Resource*>(texture),
            &rtv_desc,
            { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12ViewHeapWriteDepthStencil(const WisDX12ViewHeap*     self,
                                                                  WisDX12TextureView         texture,
                                                                  const WisRenderTargetDesc* render_target,
                                                                  uint32_t                   index)
{
    auto&                         heap = wis::from_handle_ref<const wis::impl::DX12ViewHeapImpl>(self);
    D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{
        .Format = wis::detail::DX12Convert(render_target->format),
    };
    switch (render_target->layout) {
    case WisTextureLayoutTexture1D:
        dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
        dsv_desc.Texture1D     = {
                .MipSlice = render_target->mip_level
        };
        break;
    case WisTextureLayoutTexture1DArray:
        dsv_desc.ViewDimension  = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
        dsv_desc.Texture1DArray = {
            .MipSlice        = render_target->mip_level,
            .FirstArraySlice = render_target->base_array_layer,
            .ArraySize       = render_target->array_layer_count
        };
        break;
    case WisTextureLayoutTexture2D:
        dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsv_desc.Texture2D     = {
                .MipSlice = render_target->mip_level
        };
        break;
    case WisTextureLayoutTexture2DArray:
        dsv_desc.ViewDimension  = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
        dsv_desc.Texture2DArray = {
            .MipSlice        = render_target->mip_level,
            .FirstArraySlice = render_target->base_array_layer,
            .ArraySize       = render_target->array_layer_count,
        };
        break;
    case WisTextureLayoutTexture2DMS:
        dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
        break;
    case WisTextureLayoutTexture2DMSArray:
        dsv_desc.ViewDimension    = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
        dsv_desc.Texture2DMSArray = {
            .FirstArraySlice = render_target->base_array_layer,
            .ArraySize       = render_target->array_layer_count
        };
        break;
    default:
        break;
    }

    heap.device->CreateDepthStencilView(
            std::bit_cast<ID3D12Resource*>(texture),
            &dsv_desc,
            { heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size });
    return heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12ViewHeapGetViewAddress(const WisDX12ViewHeap* self,
                                                               uint32_t               index)
{
    auto& heap = wis::from_handle_ref<const wis::impl::DX12ViewHeapImpl>(self);
    return heap.cpu_handle.ptr + static_cast<uint64_t>(index) * heap.descriptor_size;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12ViewHeapCopyViews(const WisDX12ViewHeap* self,
                                                      uint32_t               dst_index,
                                                      uint64_t               src_ptr,
                                                      uint32_t               src_index,
                                                      uint32_t               count)
{
    auto& heap = wis::from_handle_ref<const wis::impl::DX12ViewHeapImpl>(self);
    heap.device->CopyDescriptorsSimple(
            count,
            { heap.cpu_handle.ptr + static_cast<uint64_t>(dst_index) * heap.descriptor_size },
            { std::bit_cast<std::size_t>(src_ptr) + static_cast<uint64_t>(src_index) * heap.descriptor_size },
            heap.type);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12ViewHeapGetCPUHandle(const WisDX12ViewHeap* self)
{
    auto& heap = wis::from_handle_ref<const wis::impl::DX12ViewHeapImpl>(self);
    return heap.cpu_handle.ptr;
}

#endif // WIS_DX12_DESCRIPTOR_HEAP_CPP