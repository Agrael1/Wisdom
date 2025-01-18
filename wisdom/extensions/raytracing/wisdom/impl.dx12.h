#ifndef WISDOM_RAYTRACING_DX12_HPP
#define WISDOM_RAYTRACING_DX12_HPP
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>
#include <wisdom/dx12_raytracing_pipeline.h>

namespace wis {
class DX12Raytracing;

using DX12AccelerationStructure = D3D12_GPU_VIRTUAL_ADDRESS;
using DX12AccelerationStructureView = std::tuple<DX12AccelerationStructure>;

template<>
struct Internal<DX12Raytracing> {
    wis::com_ptr<ID3D12Device10> shared_device;

    bool supports_raytracing = false;
};

class ImplDX12Raytracing : public QueryInternalExtension<DX12Raytracing, DX12DeviceExtension>
{
protected:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept override
    {
        shared_device = instance.GetInternal().device;
        D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSupportData = {};
        supports_raytracing = wis::succeeded(shared_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureSupportData, sizeof(featureSupportData))) &&
                featureSupportData.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
        return wis::success;
    }

public:
    virtual bool Supported() const noexcept override
    {
        return supports_raytracing;
    }

public:
    [[nodiscard]] wis::ASAllocationInfo
    GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc) const noexcept
    {
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{
            .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
            .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(convert_dx(tlas_desc.flags) | (tlas_desc.update ? D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE : 0)),
            .NumDescs = tlas_desc.instance_count,
            .DescsLayout = tlas_desc.indirect ? D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS : D3D12_ELEMENTS_LAYOUT_ARRAY,
            .InstanceDescs = tlas_desc.gpu_address
        };
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuild_info = {};
        shared_device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuild_info);
        return { wis::detail::aligned_size(uint32_t(prebuild_info.ScratchDataSizeInBytes), uint32_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)),
                 wis::detail::aligned_size(uint32_t(prebuild_info.ResultDataMaxSizeInBytes), uint32_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)),
                 wis::detail::aligned_size(uint32_t(prebuild_info.UpdateScratchDataSizeInBytes), uint32_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)) };
    }

    [[nodiscard]] wis::ASAllocationInfo
    GetBottomLevelASSize(const wis::DX12BottomLevelASBuildDesc& tlas_desc) const noexcept
    {
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{
            .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
            .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(convert_dx(tlas_desc.flags) | (tlas_desc.update ? D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE : 0)),
            .NumDescs = tlas_desc.geometry_count,
            .DescsLayout = tlas_desc.geometry_array ? D3D12_ELEMENTS_LAYOUT_ARRAY : D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS,
        };
        if (tlas_desc.geometry_array) {
            inputs.pGeometryDescs = tlas_desc.geometry_array;
        } else {
            inputs.ppGeometryDescs = tlas_desc.geometry_indirect;
        }

        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuild_info = {};
        shared_device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuild_info);
        return { wis::detail::aligned_size(uint32_t(prebuild_info.ScratchDataSizeInBytes), uint32_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)),
                 wis::detail::aligned_size(uint32_t(prebuild_info.ResultDataMaxSizeInBytes), uint32_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)),
                 wis::detail::aligned_size(uint32_t(prebuild_info.UpdateScratchDataSizeInBytes), uint32_t(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT)) };
    }

    [[nodiscard]] wis::DX12AccelerationStructure
    CreateAccelerationStructure(wis::Result& result, wis::DX12BufferView buffer, uint64_t buffer_offset, [[maybe_unused]] uint64_t structure_size, [[maybe_unused]] ASLevel structure_level) const noexcept
    {
        return std::get<0>(buffer)->GetGPUVirtualAddress() + buffer_offset;
    }

    [[nodiscard]] uint64_t GetAccelerationStructureDeviceAddress(wis::DX12AccelerationStructureView as) const noexcept
    {
        return std::get<0>(as);
    }

    [[nodiscard]] WIS_INLINE wis::DX12RaytracingPipeline
    CreateRaytracingPipeline(wis::Result& result, const wis::DX12RaytracingPipeineDesc& desc) const noexcept;

    [[nodiscard]] static constexpr wis::ShaderBindingTableInfo GetShaderBindingTableInfo() noexcept
    {
        return wis::ShaderBindingTableInfo{ D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT };
    }

public:
    void BuildBottomLevelAS(wis::DX12CommandListView cmd_list,
                            const wis::DX12BottomLevelASBuildDesc& blas_desc,
                            wis::DX12AccelerationStructureView dst_acceleration_structure,
                            uint64_t scratch_buffer_gpu_address,
                            wis::DX12AccelerationStructureView src_acceleration_structure = {}) const noexcept
    {
        auto* cmd_list_i = static_cast<ID3D12GraphicsCommandList4*>(std::get<0>(cmd_list));
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC build_desc{
            .DestAccelerationStructureData = std::get<0>(dst_acceleration_structure),
            .Inputs = {
                    .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
                    .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(convert_dx(blas_desc.flags) | (blas_desc.update ? D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE : 0)),
                    .NumDescs = blas_desc.geometry_count,
                    .DescsLayout = blas_desc.geometry_array ? D3D12_ELEMENTS_LAYOUT_ARRAY : D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS,
            },
            .SourceAccelerationStructureData = std::get<0>(src_acceleration_structure),
            .ScratchAccelerationStructureData = { scratch_buffer_gpu_address },
        };
        if (blas_desc.geometry_array) {
            build_desc.Inputs.pGeometryDescs = blas_desc.geometry_array;
        } else {
            build_desc.Inputs.ppGeometryDescs = blas_desc.geometry_indirect;
        }
        cmd_list_i->BuildRaytracingAccelerationStructure(&build_desc, 0, nullptr);
    }

    void BuildTopLevelAS(wis::DX12CommandListView cmd_list,
                         const wis::TopLevelASBuildDesc& tlas_desc,
                         wis::DX12AccelerationStructureView dst_acceleration_structure,
                         uint64_t scratch_buffer_gpu_address,
                         wis::DX12AccelerationStructureView src_acceleration_structure = {}) const noexcept
    {
        auto* cmd_list_i = static_cast<ID3D12GraphicsCommandList4*>(std::get<0>(cmd_list));

        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC build_desc{
            .DestAccelerationStructureData = std::get<0>(dst_acceleration_structure),
            .Inputs = {
                    .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
                    .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(convert_dx(tlas_desc.flags) | (tlas_desc.update ? D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE : 0)),
                    .NumDescs = tlas_desc.instance_count,
                    .DescsLayout = tlas_desc.indirect ? D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS : D3D12_ELEMENTS_LAYOUT_ARRAY,
                    .InstanceDescs = tlas_desc.gpu_address },
            .SourceAccelerationStructureData = std::get<0>(src_acceleration_structure),
            .ScratchAccelerationStructureData = { scratch_buffer_gpu_address }
        };

        cmd_list_i->BuildRaytracingAccelerationStructure(&build_desc, 0, nullptr);
    }

    void SetPipelineState(wis::DX12CommandListView cmd_list, wis::DX12RaytracingPipelineView pipeline) const noexcept
    {
        auto* cmd_list_i = static_cast<ID3D12GraphicsCommandList4*>(std::get<0>(cmd_list));
        auto* pipeline_i = std::get<0>(pipeline);
        cmd_list_i->SetPipelineState1(pipeline_i);
    }

    void SetDescriptorStorage(wis::DX12CommandList& cmd_list, wis::DX12DescriptorStorageView desc_storage) const noexcept
    {
        cmd_list.SetComputeDescriptorStorage(desc_storage);
    }

    void PushDescriptor(wis::DX12CommandList& cmd_list, wis::DescriptorType type, uint32_t root_index, wis::DX12BufferView buffer, uint32_t offset) const noexcept
    {
        cmd_list.PushDescriptorCompute(type, root_index, buffer, offset);
    }

    void DispatchRays(wis::DX12CommandListView cmd_list, const wis::RaytracingDispatchDesc& desc) const noexcept
    {
        auto* cmd_list_i = static_cast<ID3D12GraphicsCommandList4*>(std::get<0>(cmd_list));

        D3D12_DISPATCH_RAYS_DESC dispatch_desc{
            .RayGenerationShaderRecord = { desc.ray_gen_shader_table_address, desc.ray_gen_shader_table_size },
            .MissShaderTable = { desc.miss_shader_table_address, desc.miss_shader_table_size, desc.miss_shader_table_stride },
            .HitGroupTable = { desc.hit_group_table_address, desc.hit_group_table_size, desc.hit_group_table_stride },
            .CallableShaderTable = { desc.callable_shader_table_address, desc.callable_shader_table_size, desc.callable_shader_table_stride },
            .Width = desc.width,
            .Height = desc.height,
            .Depth = desc.depth,
        };
        cmd_list_i->DispatchRays(&dispatch_desc);
    }

    void WriteAccelerationStructure(wis::DX12DescriptorStorageView storage, uint32_t binding_set, uint32_t index, wis::DX12AccelerationStructureView as) noexcept
    {
        auto& internal = std::get<0>(storage)->GetInternal();
        D3D12_SHADER_RESOURCE_VIEW_DESC desc{
            .Format = DXGI_FORMAT_UNKNOWN,
            .ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE,
            .Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .RaytracingAccelerationStructure = { std::get<0>(as) }
        };
        auto handle = D3D12_CPU_DESCRIPTOR_HANDLE(internal.heap_cpu_starts[0].ptr + internal.heap_offsets[binding_set].offset_in_bytes);
        shared_device->CreateShaderResourceView(nullptr, &desc, handle);
    }
};

[[nodiscard]] inline constexpr wis::DX12AcceleratedGeometryDesc
DX12CreateGeometryDesc(const wis::AcceleratedGeometryInput& desc) noexcept
{
    D3D12_RAYTRACING_GEOMETRY_DESC geometry{
        .Type = convert_dx(desc.geometry_type),
        .Flags = convert_dx(desc.flags),
    };
    switch (desc.geometry_type) {
    case wis::ASGeometryType::Triangles:
        geometry.Triangles = {
            .Transform3x4 = desc.transform_matrix_address,
            .IndexFormat = convert_dx(desc.index_format),
            .VertexFormat = convert_dx(desc.vertex_format),
            .IndexCount = desc.triangle_or_aabb_count * 3,
            .VertexCount = desc.vertex_count,
            .IndexBuffer = desc.index_buffer_address,
            .VertexBuffer = { .StartAddress = desc.vertex_or_aabb_buffer_address,
                              .StrideInBytes = desc.vertex_or_aabb_buffer_stride }
        };
        break;
    case wis::ASGeometryType::AABBs:
        geometry.AABBs = {
            .AABBCount = desc.triangle_or_aabb_count,
            .AABBs = { .StartAddress = desc.vertex_or_aabb_buffer_address,
                       .StrideInBytes = desc.vertex_or_aabb_buffer_stride }
        };
        break;
    default:
        break;
    }
    return geometry;
}

#pragma region DX12Raytracing
#pragma endregion DX12Raytracing
} // namespace wis

// #ifndef WISDOM_BUILD_BINARIES
// #include "impl/impl.dx12.cpp"
// #endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_DX12
#endif // !WISDOM_RAYTRACING_DX12_HPP