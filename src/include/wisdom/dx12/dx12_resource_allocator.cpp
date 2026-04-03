#ifndef WIS_DX12_RESOURCE_ALLOCATOR_CPP
#define WIS_DX12_RESOURCE_ALLOCATOR_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/util/allocation.hpp>

namespace wis::detail {
//----------------------------------------------------------------------------------------------------------------------
inline WisResult DX12CreateResource(
    const D3D12MA::ALLOCATION_DESC& all_desc,
    const D3D12_RESOURCE_DESC1& res_desc,
    D3D12_BARRIER_LAYOUT initial_layout,
    D3D12MA::Allocator* allocator,
    void* buffer
) noexcept
{
    if (all_desc.HeapType == D3D12_HEAP_TYPE_GPU_UPLOAD && !allocator->IsGPUUploadHeapSupported()) {
        return wis::detail::make_result<wis::detail::Func(), "GPU Upload Heaps are not supported on this system">(
            E_NOTIMPL
        );
    }

    wis::com_ptr<ID3D12Resource> resource;
    wis::com_ptr<D3D12MA::Allocation> allocation;
    HRESULT hr = allocator->CreateResource3(
        &all_desc,
        &res_desc,
        initial_layout,
        nullptr,
        0,
        nullptr,
        allocation.put_unchecked(),
        resource.iid(),
        resource.put_void_unchecked()
    );

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Resource Allocation failed">(hr);
    }

    allocator->AddRef();
    auto& impl = *new (buffer) wis::impl::DX12BufferImpl{
        .resource = resource.detach(),
        .allocation = allocation.detach(),
        .allocator = allocator, // store allocator to ensure correct release order
    };
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
inline D3D12_RESOURCE_DESC1 DX12FillTextureDesc(const WisTextureDesc& desc) noexcept
{
    D3D12_RESOURCE_DESC1 out{
        .Alignment = 0,
        .Width = desc.width,
        .Height = desc.height,
        .DepthOrArraySize = desc.depth_or_array_size,
        .MipLevels = desc.mip_levels,
        .Format = wis::detail::DX12Convert(desc.format),
        .SampleDesc = {1, 0},
        .Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
        .Flags = wis::detail::DX12Convert(desc.usage_flags),
    };
    switch (desc.layout) {
    case WisTextureLayoutTexture1D:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        out.Height = 1;
        out.DepthOrArraySize = 1;
        return out;
    case WisTextureLayoutTexture1DArray:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        out.Height = 1;
        return out;
    default:
    case WisTextureLayoutTexture2D:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = 1;
        return out;
    case WisTextureLayoutTexture2DArray:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        return out;
    case WisTextureLayoutTexture3D:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
        return out;
    case WisTextureLayoutTexture2DMS:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = 1;
        out.SampleDesc.Count = wis::detail::DX12Convert(desc.sample_count);
        out.MipLevels = 1;
        return out;
    case WisTextureLayoutTexture2DMSArray:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.SampleDesc.Count = wis::detail::DX12Convert(desc.sample_count);
        out.MipLevels = 1;
        return out;
    case WisTextureLayoutTextureCube:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = 6;
        return out;
    case WisTextureLayoutTextureCubeArray:
        out.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        out.DepthOrArraySize = desc.depth_or_array_size / 6 + (desc.depth_or_array_size % 6 != 0);
        return out;
    }
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyResourceAllocator(WisDX12ResourceAllocator* self)
{
    auto& [allocator, device] = wis::from_handle_ref<wis::impl::DX12ResourceAllocatorImpl>(self);
    if (!allocator) {
        return;
    }

    allocator->Release();
    allocator = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12ResourceAllocatorCreateBuffer(
    const WisDX12ResourceAllocator* self,
    const WisBufferDesc* desc,
    WisDX12Buffer* buffer
)
{
    auto& [allocator, device] = wis::from_handle_ref<const wis::impl::DX12ResourceAllocatorImpl>(self);
    uint64_t size = wis::aligned_size(
        desc->size_bytes,
        static_cast<uint64_t>(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)
    );
    D3D12_RESOURCE_DESC1 buffer_desc{
        .Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
        .Alignment = 0,
        .Width = size,
        .Height = 1,
        .DepthOrArraySize = 1,
        .MipLevels = 1,
        .Format = DXGI_FORMAT_UNKNOWN,
        .SampleDesc = {1, 0},
        .Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        .Flags = wis::detail::DX12Convert(desc->usage_flags),
        .SamplerFeedbackMipRegion = {0, 0, 0},
    };

    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = wis::detail::DX12Convert(desc->memory_flags),
        .HeapType = wis::detail::DX12Convert(desc->memory_type),
    };
    return wis::detail::DX12CreateResource(all_desc, buffer_desc, D3D12_BARRIER_LAYOUT_UNDEFINED, allocator, buffer);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12ResourceAllocatorCreateTexture(
    const WisDX12ResourceAllocator* self,
    const WisTextureDesc* desc,
    WisDX12Texture* buffer
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12ResourceAllocatorImpl>(self);
    D3D12_RESOURCE_DESC1 tex_desc = wis::detail::DX12FillTextureDesc(*desc);
    D3D12MA::ALLOCATION_DESC all_desc{
        .Flags = wis::detail::DX12Convert(desc->memory_flags),
        .HeapType = wis::detail::DX12Convert(desc->memory_type),
    };
    return wis::detail::DX12CreateResource(all_desc, tex_desc, D3D12_BARRIER_LAYOUT_UNDEFINED, impl.allocator, buffer);
}

#endif // WIS_DX12_RESOURCE_ALLOCATOR_CPP
