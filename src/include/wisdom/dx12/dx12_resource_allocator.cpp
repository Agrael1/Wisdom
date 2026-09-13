#ifndef WIS_DX12_RESOURCE_ALLOCATOR_CPP
#define WIS_DX12_RESOURCE_ALLOCATOR_CPP

#include <wisdom/bridge/span.hpp>
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
    wis::span<const DXGI_FORMAT> cast_formats,
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
        static_cast<uint32_t>(cast_formats.size()),
        cast_formats.data(),
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
    return wis::detail::DX12CreateResource(
        all_desc,
        buffer_desc,
        D3D12_BARRIER_LAYOUT_UNDEFINED,
        allocator,
        {},
        buffer
    );
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

    // planar formats are uncastable
    if (desc->format >= WisDataFormatNV12) {
        return wis::detail::DX12CreateResource(
            all_desc,
            tex_desc,
            D3D12_BARRIER_LAYOUT_UNDEFINED,
            impl.allocator,
            {},
            buffer
        );
    }

    static constexpr uint32_t max_cast_formats = 16;
    DXGI_FORMAT cast_formats[max_cast_formats];
    wis::span<DXGI_FORMAT> cast_formats_span;
    std::unique_ptr<DXGI_FORMAT[]> cast_formats_ptr;

    bool directly_mappable = true;
    uint32_t format_index = 0;
    for (; format_index < desc->cast_format_count; format_index++) {
        auto format = desc->cast_formats[format_index];
        if (static_cast<uint32_t>(format) >= 256) {
            directly_mappable = false;

            if (desc->cast_format_count > max_cast_formats) {
                cast_formats_ptr = std::make_unique<DXGI_FORMAT[]>(desc->cast_format_count);
                cast_formats_span = {cast_formats_ptr.get(), desc->cast_format_count};
            } else {
                cast_formats_span = {cast_formats, desc->cast_format_count};
            }

            // Copy the compatible formats into the span [0->format_index)
            std::memcpy(cast_formats_span.data(), desc->cast_formats, format_index);

            // Convert the rest
            for (uint32_t i = format_index; i < desc->cast_format_count; i++) {
                cast_formats_span[i] = wis::detail::DX12Convert(desc->cast_formats[i]);
            }

            break;
        }
    }

    if (directly_mappable) {
        return wis::detail::DX12CreateResource(
            all_desc,
            tex_desc,
            D3D12_BARRIER_LAYOUT_UNDEFINED,
            impl.allocator,
            {reinterpret_cast<const DXGI_FORMAT*>(desc->cast_formats), desc->cast_format_count},
            buffer
        );
    }
    return wis::detail::DX12CreateResource(
        all_desc,
        tex_desc,
        D3D12_BARRIER_LAYOUT_UNDEFINED,
        impl.allocator,
        cast_formats_span,
        buffer
    );
}

#endif // WIS_DX12_RESOURCE_ALLOCATOR_CPP
