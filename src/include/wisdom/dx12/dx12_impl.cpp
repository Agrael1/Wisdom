#ifndef WIS_DX12_IMPL_CPP
#define WIS_DX12_IMPL_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/util/allocation.hpp>

#include <d3dx12/d3dx12_resource_helpers.h>

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyRootSignature(WisDX12RootSignature* self)
{
    auto& [root_signature] = wis::from_handle_ref<wis::impl::DX12RootSignatureImpl>(self);
    if (!root_signature) {
        return;
    }

    root_signature->Release();
    root_signature = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyBuffer(WisDX12Buffer* self)
{
    auto& [resource, allocation, allocator] = wis::from_handle_ref<wis::impl::DX12BufferImpl>(self);
    if (!resource) {
        return;
    }

    resource->Release();
    allocation->Release();
    allocator->Release();
    resource = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisDX12BufferMap(const WisDX12Buffer* self)
{
    auto& [resource, allocation, allocator] = wis::from_handle_ref<const wis::impl::DX12BufferImpl>(self);
    void* mapped_ptr = nullptr;
    auto hr = resource->Map(0, nullptr, &mapped_ptr);
    (void)hr; // Ignore mapping failure, return nullptr in that case
    return mapped_ptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12BufferGetGPUAddress(const WisDX12Buffer* self)
{
    auto& [resource, allocation, allocator] = wis::from_handle_ref<const wis::impl::DX12BufferImpl>(self);
    return resource->GetGPUVirtualAddress();
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyTexture(WisDX12Texture* self)
{
    auto& [resource, allocation, allocator] = wis::from_handle_ref<wis::impl::DX12BufferImpl>(self);
    if (!resource) {
        return;
    }

    resource->Release();
    resource = nullptr;
    if (!allocation && !allocator) {
        // If allocation or allocator are null,
        // we assume that the texture does not own the resource.
        return;
    }

    allocation->Release();
    allocator->Release();
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12TextureWriteSubresource(
    const WisDX12Texture* self,
    const void* source_data,
    const WisTextureRegion* target_region
)
{
    auto& [resource, allocation, allocator] = wis::from_handle_ref<const wis::impl::DX12BufferImpl>(self);

    auto desc = resource->GetDesc();
    bool is_3d = desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D;

    UINT row_pitch = 0;
    UINT slice_pitch = 0;
    auto hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorRowPitch(
                  desc.Format,
                  target_region->box.width,
                  row_pitch
              );
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to calculate row pitch">(hr);
    }

    hr = D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorSlicePitch(
             desc.Format,
             row_pitch,
             target_region->box.height,
             slice_pitch
         );
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to calculate slice pitch">(hr);
    }

    D3D12_BOX dst_box{
        .left = target_region->box.x,
        .top = target_region->box.y,
        .front = is_3d ? target_region->box.z : 0,
        .right = target_region->box.x + target_region->box.width,
        .bottom = target_region->box.y + target_region->box.height,
        .back = is_3d ? target_region->box.z + target_region->box.depth : 1,
    };
    auto subresource = D3D12CalcSubresource(
                           target_region->target_subresource.mip_level,
                           target_region->target_subresource.array_layer,
                           target_region->target_subresource.plane_slice,
                           desc.MipLevels,
                           is_3d ? 1 : desc.DepthOrArraySize
                       );
    hr = resource->WriteToSubresource(subresource, &dst_box, source_data, row_pitch, slice_pitch);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to write to subresource">(hr);
    }
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyShader(WisDX12Shader* self)
{
    auto& [shader] = wis::from_handle_ref<wis::impl::DX12ShaderImpl>(self);
    if (!shader) {
        return;
    }
    delete shader;
    shader = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyPipeline(WisDX12Pipeline* self)
{
    auto& [pipeline] = wis::from_handle_ref<wis::impl::DX12PipelineImpl>(self);
    if (!pipeline) {
        return;
    }
    pipeline->Release();
    pipeline = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroySurface(WisDX12Surface* self)
{
    (void)self; // No resources to release for surface, as it's just a wrapper around the HWND or CoreWindow handle,
    // which is owned by the application
}

#endif // WIS_DX12_IMPL_CPP
