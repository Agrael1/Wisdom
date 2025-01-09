#ifndef WISDOM_RAYTRACING_HPP
#define WISDOM_RAYTRACING_HPP
#include <cstdint>

namespace wis {
struct AccelerationInstance {
    float transform[3][4];
    uint32_t instance_id : 24;
    uint32_t mask : 8;
    uint32_t instance_offset : 24;
    uint32_t flags : 8;
    uint64_t acceleration_structure_handle;
};
} // namespace wis

#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>

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

    void BuildBottomLevelAS(wis::DX12CommandListView cmd_list,
                            const wis::DX12BottomLevelASBuildDesc& blas_desc,
                            wis::DX12AccelerationStructureView dst_acceleration_structure,
                            uint64_t scratch_buffer_gpu_address,
                            wis::DX12AccelerationStructureView src_acceleration_structure = {})
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

    void BuildTopLevelAS(const wis::DX12CommandList& cmd_list,
                         const wis::TopLevelASBuildDesc& tlas_desc,
                         wis::DX12AccelerationStructureView dst_acceleration_structure,
                         uint64_t scratch_buffer_gpu_address,
                         wis::DX12AccelerationStructureView src_acceleration_structure = {})
    {
        auto& cmd_list_i = cmd_list.GetInternal().list;

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
#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vk_rtas.h>

namespace wis {
class VKRaytracing;

template<>
struct Internal<VKRaytracing> {
    wis::SharedDevice device;

    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = nullptr;
    PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = nullptr;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = nullptr;
    PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = nullptr;
    PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR = nullptr;
};

class ImplVKRaytracing : public QueryInternalExtension<VKRaytracing, wis::VKDeviceExtension>
{
protected:
    virtual WIS_INLINE bool
    GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                     std::unordered_set<std::string_view>& ext_name_set,
                     std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                     std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

    virtual WIS_INLINE wis::Result
    Init(const wis::VKDevice& instance,
         const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
         const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

public:
    virtual bool Supported() const noexcept override
    {
        return vkGetAccelerationStructureBuildSizesKHR;
    }

public:
    [[nodiscard]] wis::ASAllocationInfo
    GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc)
    {
        VkAccelerationStructureGeometryKHR geometry{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
            .geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR,
            .geometry = {
                    .instances = {
                            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
                            .arrayOfPointers = tlas_desc.indirect,
                            .data = { .deviceAddress = tlas_desc.gpu_address } } },
            .flags = convert_vk(tlas_desc.flags)
        };
        VkAccelerationStructureBuildGeometryInfoKHR build_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
            .type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
            .flags = convert_vk(tlas_desc.flags),
            .mode = tlas_desc.update ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
            .geometryCount = 1u,
            .pGeometries = &geometry,
        };
        VkAccelerationStructureBuildSizesInfoKHR build_sizes_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR,
        };

        uint32_t max_instance_count = tlas_desc.instance_count;
        vkGetAccelerationStructureBuildSizesKHR(device.get(),
                                                VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                                                &build_info,
                                                &max_instance_count,
                                                &build_sizes_info);
        return { build_sizes_info.buildScratchSize, build_sizes_info.accelerationStructureSize, build_sizes_info.updateScratchSize };
    }
    [[nodiscard]] wis::ASAllocationInfo
    GetBottomLevelASSize(const wis::VKBottomLevelASBuildDesc& blas_desc) const noexcept
    {
        // one is ppGeometries, the other is maxPrimitiveCount
        uint32_t direct = bool(blas_desc.geometry_array);
        size_t num_bytes = blas_desc.geometry_count * (sizeof(uint32_t) + direct * sizeof(VkAccelerationStructureGeometryKHR*));

        constexpr size_t initial_geometry_guess = 64;
        wis::detail::limited_allocator<uint8_t, initial_geometry_guess * (sizeof(uint32_t) + sizeof(VkAccelerationStructureGeometryKHR*))> allocator{ uint32_t(num_bytes), true };
        auto* data = allocator.data();

        const VkAccelerationStructureGeometryKHR** pp_geometries = reinterpret_cast<const VkAccelerationStructureGeometryKHR**>(data);
        uint32_t* max_primitive_count = reinterpret_cast<uint32_t*>(pp_geometries + direct * blas_desc.geometry_count);

        if (direct) {
            for (size_t i = 0; i < blas_desc.geometry_count; ++i) {
                pp_geometries[i] = &blas_desc.geometry_array[i].first;
                max_primitive_count[i] = blas_desc.geometry_array[i].second.primitiveCount;
            }
        } else {
            for (size_t i = 0; i < blas_desc.geometry_count; ++i) {
                max_primitive_count[i] = blas_desc.geometry_indirect[i]->second.primitiveCount;
            }
        }

        VkAccelerationStructureBuildGeometryInfoKHR build_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
            .type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
            .flags = convert_vk(blas_desc.flags),
            .mode = blas_desc.update ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
            .geometryCount = blas_desc.geometry_count,
            .ppGeometries = direct
                    ? reinterpret_cast<const VkAccelerationStructureGeometryKHR* const*>(data)
                    : reinterpret_cast<const VkAccelerationStructureGeometryKHR* const*>(blas_desc.geometry_indirect),
        };
        VkAccelerationStructureBuildSizesInfoKHR build_sizes_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR,
        };
        vkGetAccelerationStructureBuildSizesKHR(device.get(),
                                                VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                                                &build_info,
                                                max_primitive_count,
                                                &build_sizes_info);
        return { build_sizes_info.buildScratchSize, build_sizes_info.accelerationStructureSize, build_sizes_info.updateScratchSize };
    }

    [[nodiscard]] wis::VKAccelerationStructure
    CreateAccelerationStructure(wis::Result& result, wis::VKBufferView buffer, uint64_t buffer_offset, uint64_t structure_size, ASLevel structure_level) const noexcept
    {
        VkAccelerationStructureCreateInfoKHR create_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
            .buffer = std::get<0>(buffer),
            .offset = buffer_offset,
            .size = structure_size,
            .type = structure_level == ASLevel::Top ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR
        };
        VKAccelerationStructure as;
        auto& as_i = as.GetMutableInternal();

        auto res = vkCreateAccelerationStructureKHR(device.get(), &create_info, nullptr, &as_i.handle);
        if (res != VK_SUCCESS) {
            result = wis::make_result<FUNC, "Acceleration structure creation failed">(res);
            return as;
        }
        as_i.device = device;
        as_i.vkDestroyAccelerationStructureKHR = vkDestroyAccelerationStructureKHR;
        return as;
    }

    [[nodiscard]] uint64_t GetAccelerationStructureDeviceAddress(wis::VKAccelerationStructureView as) const noexcept
    {
        VkAccelerationStructureDeviceAddressInfoKHR info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
            .accelerationStructure = std::get<0>(as)
        };
        return vkGetAccelerationStructureDeviceAddressKHR(device.get(), &info);
    }

public:
    void BuildBottomLevelAS(wis::VKCommandListView cmd_buffer,
                            const wis::VKBottomLevelASBuildDesc& blas_desc,
                            wis::VKAccelerationStructureView dst_acceleration_structure,
                            uint64_t scratch_buffer_gpu_address,
                            wis::VKAccelerationStructureView src_acceleration_structure = {}) const noexcept
    {
        // one is ppGeometries, the other is maxPrimitiveCount
        uint32_t direct = bool(blas_desc.geometry_array);
        size_t num_bytes = blas_desc.geometry_count * (sizeof(VkAccelerationStructureBuildRangeInfoKHR*) + direct * sizeof(VkAccelerationStructureGeometryKHR*));

        constexpr size_t initial_geometry_guess = 64;
        wis::detail::limited_allocator<uint8_t, initial_geometry_guess * (sizeof(VkAccelerationStructureBuildRangeInfoKHR*) + sizeof(VkAccelerationStructureGeometryKHR*))> allocator{ uint32_t(num_bytes), true };
        auto* data = allocator.data();

        const VkAccelerationStructureGeometryKHR** pp_geometries = reinterpret_cast<const VkAccelerationStructureGeometryKHR**>(data);
        const VkAccelerationStructureBuildRangeInfoKHR** pp_ranges = reinterpret_cast<const VkAccelerationStructureBuildRangeInfoKHR**>(pp_geometries + direct * blas_desc.geometry_count);

        if (direct) {
            for (size_t i = 0; i < blas_desc.geometry_count; ++i) {
                pp_geometries[i] = &blas_desc.geometry_array[i].first;
                pp_ranges[i] = &blas_desc.geometry_array[i].second;
            }
        } else {
            // May be optimized
            for (size_t i = 0; i < blas_desc.geometry_count; ++i) {
                pp_ranges[i] = &blas_desc.geometry_indirect[i]->second;
            }
        }

        VkAccelerationStructureBuildGeometryInfoKHR build_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
            .type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
            .flags = convert_vk(blas_desc.flags),
            .mode = blas_desc.update ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
            .srcAccelerationStructure = std::get<0>(src_acceleration_structure),
            .dstAccelerationStructure = std::get<0>(dst_acceleration_structure),
            .geometryCount = blas_desc.geometry_count,
            .ppGeometries = direct
                    ? reinterpret_cast<const VkAccelerationStructureGeometryKHR* const*>(data)
                    : reinterpret_cast<const VkAccelerationStructureGeometryKHR* const*>(blas_desc.geometry_indirect),
            .scratchData = scratch_buffer_gpu_address
        };
        vkCmdBuildAccelerationStructuresKHR(std::get<0>(cmd_buffer), 1, &build_info, pp_ranges);
    }

    void BuildTopLevelAS(wis::VKCommandListView cmd_buffer,
                         const wis::TopLevelASBuildDesc& tlas_desc,
                         wis::VKAccelerationStructureView dst_acceleration_structure,
                         uint64_t scratch_buffer_gpu_address,
                         wis::VKAccelerationStructureView src_acceleration_structure = {}) const noexcept
    {
        VkAccelerationStructureGeometryKHR geometry{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
            .geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR,
            .geometry = {
                    .instances = {
                            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
                            .arrayOfPointers = tlas_desc.indirect,
                            .data = { .deviceAddress = tlas_desc.gpu_address } } },
            .flags = convert_vk(tlas_desc.flags)
        };
        VkAccelerationStructureBuildGeometryInfoKHR build_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
            .type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
            .flags = convert_vk(tlas_desc.flags),
            .mode = tlas_desc.update ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
            .srcAccelerationStructure = std::get<0>(src_acceleration_structure),
            .dstAccelerationStructure = std::get<0>(dst_acceleration_structure),
            .geometryCount = 1u,
            .pGeometries = &geometry,
            .scratchData = scratch_buffer_gpu_address,
        };
        VkAccelerationStructureBuildRangeInfoKHR range_info{
            .primitiveCount = tlas_desc.instance_count,
        };
        VkAccelerationStructureBuildRangeInfoKHR* p_range_info = &range_info;
        vkCmdBuildAccelerationStructuresKHR(std::get<0>(cmd_buffer), 1, &build_info, &p_range_info);
    }

    void WriteAccelerationStructure(wis::VKDescriptorStorageView storage, uint32_t binding_set, uint32_t index, wis::VKAccelerationStructureView as) noexcept
    {
        VkWriteDescriptorSetAccelerationStructureKHR as_info{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
            .accelerationStructureCount = 1,
            .pAccelerationStructures = &std::get<0>(as),
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = &as_info,
            .dstSet = std::get<0>(storage)[binding_set],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
};

[[nodiscard]] inline constexpr wis::VKAcceleratedGeometryDesc
VKCreateGeometryDesc(const wis::AcceleratedGeometryInput& desc) noexcept
{
    wis::VKAcceleratedGeometryDesc out;
    out.first = {
        .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        .geometryType = convert_vk(desc.geometry_type),
        .flags = convert_vk(desc.flags)
    };
    out.second = {
        .primitiveCount = desc.triangle_or_aabb_count,
    };
    switch (desc.geometry_type) {
    case wis::ASGeometryType::Triangles:
        out.first.geometry.triangles = {
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
            .vertexFormat = convert_vk(desc.vertex_format),
            .vertexData = { .deviceAddress = desc.vertex_or_aabb_buffer_address },
            .vertexStride = desc.vertex_or_aabb_buffer_stride,
            .maxVertex = desc.vertex_count,
            .indexType = convert_vk(desc.index_format),
            .indexData = { .deviceAddress = desc.index_buffer_address },
            .transformData = { .deviceAddress = desc.transform_matrix_address }
        };
        break;
    case wis::ASGeometryType::AABBs:
        out.first.geometry.aabbs = {
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR,
            .data = { .deviceAddress = desc.vertex_or_aabb_buffer_address },
            .stride = desc.vertex_or_aabb_buffer_stride
        };
        break;
    default:
        break;
    }
    return out;
}

#pragma region VKRaytracing
#pragma endregion VKRaytracing
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using Raytracing = ImplDX12Raytracing;
using AccelerationStructure = DX12AccelerationStructure;
using AcceleratedGeometryDesc = DX12AcceleratedGeometryDesc;

[[nodiscard]] inline constexpr wis::AcceleratedGeometryDesc
CreateGeometryDesc(const wis::AcceleratedGeometryInput& desc) noexcept
{
    return DX12CreateGeometryDesc(desc);
}

#elif defined(WISDOM_VULKAN)
using Raytracing = ImplVKRaytracing;
using AccelerationStructure = VKAccelerationStructure;
using AcceleratedGeometryDesc = VKAcceleratedGeometryDesc;

[[nodiscard]] inline constexpr wis::VKAcceleratedGeometryDesc
CreateGeometryDesc(const wis::AcceleratedGeometryInput& desc) noexcept
{
    return VKCreateGeometryDesc(desc);
}
#endif // WISDOM_DX12
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // !WISDOM_RAYTRACING_HPP
