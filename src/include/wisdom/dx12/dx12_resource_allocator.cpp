#ifndef WIS_DX12_RESOURCE_ALLOCATOR_CPP
#define WIS_DX12_RESOURCE_ALLOCATOR_CPP

#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

namespace wis::detail {
//-----------------------------------------------------------------------------
inline WisResult DX12CreateResource(const D3D12MA::ALLOCATION_DESC& all_desc,
                                    const D3D12_RESOURCE_DESC1&     res_desc,
                                    D3D12_RESOURCE_STATES           state,
                                    D3D12MA::Allocator*             allocator,
                                    void*                           buffer) noexcept
{
    wis::com_ptr<ID3D12Resource>      resource;
    wis::com_ptr<D3D12MA::Allocation> allocation;
    HRESULT                           hr = allocator->CreateResource2(&all_desc,
                                            &res_desc,
                                            state,
                                            nullptr,
                                            allocation.put_unchecked(),
                                            resource.iid(),
                                            resource.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Resource Allocation failed">(hr);
    }

    auto& impl      = *new (buffer) wis::impl::DX12BufferImpl;
    impl.allocation = allocation.detach();
    impl.resource   = resource.detach();
    impl.allocator  = allocator; // store allocator to ensure correct release order
    impl.allocator->AddRef();
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
inline WisResult DX12CreateResource2(const D3D12MA::ALLOCATION_DESC& all_desc,
                                     const D3D12_RESOURCE_DESC1&     res_desc,
                                     D3D12_RESOURCE_STATES           state,
                                     D3D12MA::Allocator*             allocator,
                                     WisDX12Buffer*                  buffer) noexcept
{
    wis::com_ptr<ID3D12Resource>      resource;
    wis::com_ptr<D3D12MA::Allocation> allocation;
    HRESULT                           hr = allocator->CreateResource3(&all_desc,
                                            &res_desc,
                                            D3D12_BARRIER_LAYOUT_UNDEFINED,
                                            nullptr,
                                            0,
                                            nullptr,
                                            allocation.put_unchecked(),
                                            resource.iid(),
                                            resource.put_void_unchecked());

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Resource Allocation failed">(hr);
    }

    auto& impl      = *new (buffer) wis::impl::DX12BufferImpl;
    impl.allocation = allocation.detach();
    impl.resource   = resource.detach();
    impl.allocator  = allocator; // store allocator to ensure correct release order
    impl.allocator->AddRef();
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
inline D3D12_RESOURCE_DESC1 DX12FillTextureDesc(const WisTextureDesc& desc) noexcept
{
    D3D12_RESOURCE_DESC1 out{
        .Alignment        = 0,
        .Width            = desc.width,
        .Height           = desc.height,
        .DepthOrArraySize = desc.depth_or_array_size,
        .MipLevels        = desc.mip_levels,
        .Format           = wis::detail::convert_dx(desc.format),
        .SampleDesc       = { 1, 0 },
        .Layout           = D3D12_TEXTURE_LAYOUT_UNKNOWN,
        .Flags            = wis::detail::convert_dx(desc.usage_flags),
    };
    switch (desc.layout) {
    case WisTextureLayoutTexture1D:
        out.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        out.Height           = 1;
        out.DepthOrArraySize = 1;
        return out;
    case WisTextureLayoutTexture1DArray:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        out.Height    = 1;
        return out;
    default:
    case WisTextureLayoutTexture2D:
        out.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = 1;
        return out;
    case WisTextureLayoutTexture2DArray:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        return out;
    case WisTextureLayoutTexture3D:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
        return out;
    case WisTextureLayoutTexture2DMS:
        out.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize   = 1;
        out.SampleDesc.Count   = wis::detail::convert_dx(desc.sample_count);
        out.SampleDesc.Quality = 4;
        out.MipLevels          = 1;
        return out;
    case WisTextureLayoutTexture2DMSArray:
        out.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.SampleDesc.Count   = wis::detail::convert_dx(desc.sample_count);
        out.SampleDesc.Quality = 4;
        out.MipLevels          = 1;
        return out;
    case WisTextureLayoutTextureCube:
        out.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = 6;
        return out;
    case WisTextureLayoutTextureCubeArray:
        out.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = desc.depth_or_array_size / 6 + (desc.depth_or_array_size % 6 != 0);
        return out;
    }
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyResourceAllocator(WisDX12ResourceAllocator* self)
{
    auto& [allocator] = *reinterpret_cast<wis::impl::DX12ResourceAllocatorImpl*>(self);
    if (!allocator) {
        return;
    }

    allocator->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12ResourceAllocatorCreateBuffer(const WisDX12ResourceAllocator* self,
                                                                       const WisBufferDesc*            desc,
                                                                       WisDX12Buffer*                  buffer)
{
    auto& [allocator]         = *reinterpret_cast<const wis::impl::DX12ResourceAllocatorImpl*>(self);
    uint64_t             size = wis::aligned_size(desc->size_bytes, static_cast<uint64_t>(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT));
    D3D12_RESOURCE_DESC1 buffer_desc{
        .Dimension                = D3D12_RESOURCE_DIMENSION_BUFFER,
        .Alignment                = 0,
        .Width                    = size,
        .Height                   = 1,
        .DepthOrArraySize         = 1,
        .MipLevels                = 1,
        .Format                   = DXGI_FORMAT_UNKNOWN,
        .SampleDesc               = { 1, 0 },
        .Layout                   = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        .Flags                    = wis::detail::convert_dx(desc->usage_flags),
        .SamplerFeedbackMipRegion = { 0, 0, 0 },
    };

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags    = wis::detail::convert_dx(desc->memory_flags),
        .HeapType = wis::detail::convert_dx(desc->memory_type),
    };
    if (desc->usage_flags & WisBufferUsageFlagsAccelerationStructureBuffer) {
        return wis::detail::DX12CreateResource2(all_desc, buffer_desc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, allocator, buffer);
    }
    return wis::detail::DX12CreateResource(all_desc, buffer_desc, D3D12_RESOURCE_STATE_COMMON, allocator, buffer);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12ResourceAllocatorCreateTexture(const WisDX12ResourceAllocator* self,
                                                                        const WisTextureDesc*           desc,
                                                                        WisDX12Texture*                 buffer)
{
    auto& [allocator]                 = *reinterpret_cast<const wis::impl::DX12ResourceAllocatorImpl*>(self);
    D3D12_RESOURCE_DESC1     tex_desc = wis::detail::DX12FillTextureDesc(*desc);
    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags    = wis::detail::convert_dx(desc->memory_flags),
        .HeapType = wis::detail::convert_dx(desc->memory_type),
    };
    return wis::detail::DX12CreateResource(all_desc, tex_desc, D3D12_RESOURCE_STATE_COMMON, allocator, buffer);
}

#endif // WIS_DX12_RESOURCE_ALLOCATOR_CPP