#ifndef WIS_DX12_RAYTRACING_CPP
#define WIS_DX12_RAYTRACING_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <raytracing/generated/cpp_api.hpp>
#include <raytracing/generated/dx12_convert.hpp>
#include <bit>

namespace wis::detail {
inline WisResult DX12RaytracingExtensionInit(
    wis::DX12DeviceExtensionHeader* self,
    const wis::impl::DX12DeviceImpl& device
) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);

    if (impl.device) {
        impl.device->Release();
    }

    impl.device = device.device;
    impl.device->AddRef(); // AddRef factory to ensure it lives as long as the extension
    return wis::detail::dx_success;
}

[[nodiscard]] inline constexpr D3D12_RAYTRACING_GEOMETRY_DESC DX12CreateGeometryDesc(
    const WisAcceleratedGeometryDesc& desc
) noexcept
{
    D3D12_RAYTRACING_GEOMETRY_DESC geometry{
        .Type = wis::detail::DX12Convert(desc.type),
        .Flags = wis::detail::DX12Convert(desc.flags),
    };
    switch (desc.type) {
    case WisGeometryTypeTriangles:
        geometry.Triangles = {
            .Transform3x4 = desc.transform_matrix_address,
            .IndexFormat = wis::detail::DX12Convert(desc.index_format),
            .VertexFormat = wis::detail::DX12Convert(desc.vertex_format),
            .IndexCount = desc.triangle_or_aabb_count * 3,
            .VertexCount = desc.vertex_count,
            .IndexBuffer = desc.index_buffer_address,
            .VertexBuffer = {
                .StartAddress = desc.vertex_or_aabb_buffer_address,
                .StrideInBytes = desc.vertex_or_aabb_stride
            }
        };
        break;
    case WisGeometryTypeAABBs:
        geometry.AABBs = {
            .AABBCount = desc.triangle_or_aabb_count,
            .AABBs = {
                .StartAddress = desc.vertex_or_aabb_buffer_address,
                .StrideInBytes = desc.vertex_or_aabb_stride
            }
        };
        break;
    default:
        break;
    }
    return geometry;
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisDX12InitRaytracingExtension(WisDX12RaytracingExtension* self)
{
    new (self) wis::impl::DX12RaytracingExtensionImpl{
        .header = {&wis::detail::DX12RaytracingExtensionInit},
    };
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisDX12DestroyRaytracingExtension(WisDX12RaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);
    if (impl.device) {
        impl.device->Release();
        impl.device = nullptr;
    }
    impl.header = {nullptr};
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API bool wisDX12RaytracingExtensionSupported(WisDX12RaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);
    if (!impl.device) {
        return false;
    }
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5{};
    HRESULT hr = impl.device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5));
    if (FAILED(hr)) {
        return false;
    }
    return options5.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API WisResult wisDX12RaytracingExtensionCreateAccelerationStructure(
    WisDX12RaytracingExtension* self,
    WisDX12Buffer* buffer,
    const WisDX12AccelerationStructureDesc* desc,
    WisDX12AccelerationStructure* acceleration_structure
)
{
    auto& buffer_impl = wis::from_handle_ref<wis::impl::DX12BufferImpl>(buffer);
    auto address = buffer_impl.resource->GetGPUVirtualAddress(); // AddRef buffer to ensure it lives as long as the acceleration
                                                        // structure
    if (address == 0) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to get GPU virtual address of the buffer">(E_FAIL);
    }
    
    new (acceleration_structure) wis::impl::DX12AccelerationStructureImpl{
        .gpu_address = address + desc->offset,
    };
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisDX12DestroyAccelerationStructure(WisDX12AccelerationStructure* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12AccelerationStructureImpl>(self);
    impl.gpu_address = 0;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API uint64_t wisDX12AccelerationStructureGetGPUAddress(WisDX12AccelerationStructure* self)
{
    return wis::from_handle_ref<wis::impl::DX12AccelerationStructureImpl>(self).gpu_address;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API WisResult wisDX12RaytracingExtensionGetBottomLevelStructureInfo(
    WisDX12RaytracingExtension* self,
    const WisBottomLevelStructureBuildDesc* build_desc,
    WisStructureAllocationInfo* info
)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{
        .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
        .Flags = wis::detail::DX12Convert(build_desc->flags),
        .NumDescs = build_desc->geometry_count,
        .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
    };

    // Allocate temporary array for geometry descriptions if needed
    static constexpr size_t max_preallocated_descs = 32;
    D3D12_RAYTRACING_GEOMETRY_DESC preallocted_descs[max_preallocated_descs];
    std::unique_ptr<D3D12_RAYTRACING_GEOMETRY_DESC[]> dynamic_descs;
    wis::span<D3D12_RAYTRACING_GEOMETRY_DESC> geometry_descs;
    if (build_desc->geometry_count > max_preallocated_descs) {
        dynamic_descs = wis::make_unique<D3D12_RAYTRACING_GEOMETRY_DESC[]>(build_desc->geometry_count);
        if (!dynamic_descs) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for geometry descriptions">(
                E_OUTOFMEMORY
            );
        }
        geometry_descs = {dynamic_descs.get(), build_desc->geometry_count};
    } else {
        geometry_descs = {preallocted_descs, build_desc->geometry_count};
    }

    // Convert geometry descriptions
    if (build_desc->geometries) {
        for (uint32_t i = 0; i < build_desc->geometry_count; ++i) {
            geometry_descs[i] = wis::detail::DX12CreateGeometryDesc(build_desc->geometries[i]); 
        }
    } else if (build_desc->indirect_geometries) {
        for (uint32_t i = 0; i < build_desc->geometry_count; ++i) {
            geometry_descs[i] = wis::detail::DX12CreateGeometryDesc(*build_desc->indirect_geometries[i]);
        }
    }

    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuild_info = {};
    impl.device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuild_info);
    *info = {
        wis::aligned_size(
            uint64_t(prebuild_info.ScratchDataSizeInBytes),
            uint64_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)
        ),
        wis::aligned_size(
            uint64_t(prebuild_info.ResultDataMaxSizeInBytes),
            uint64_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)
        ),
        wis::aligned_size(
            uint64_t(prebuild_info.UpdateScratchDataSizeInBytes),
            uint64_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)
        )
    };
    return wis::detail::dx_success;
}

#endif
