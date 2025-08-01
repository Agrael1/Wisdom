#ifndef WISDOM_RAYTRACING_VK_HPP
#define WISDOM_RAYTRACING_VK_HPP
#if defined(WISDOM_VULKAN)
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vk_rtas.h>
#include "generated/vk_functions.hpp"
#include "vk_raytracing_pipeline.h"
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
class VKRaytracing;

template<>
struct Internal<VKRaytracing> {
    wis::SharedDevice device;
    wis::VKRaytracingExtDevice table;

    wis::ShaderBindingTableInfo sbt_info;
    uint32_t max_recursion_depth = 0;
    uint32_t compressed_handle_size = 0;
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
        return table.vkGetAccelerationStructureBuildSizesKHR;
    }

public:
    [[nodiscard]] WIS_INLINE wis::RaytracingConstants
    GetRaytracingConstants() const noexcept;

    [[nodiscard]] WIS_INLINE wis::ASAllocationInfo
    GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ASAllocationInfo
    GetBottomLevelASSize(const wis::VKBottomLevelASBuildDesc& blas_desc) const noexcept;

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

        auto res = table.vkCreateAccelerationStructureKHR(device.get(), &create_info, nullptr, &as_i.handle);
        if (res != VK_SUCCESS) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Acceleration structure creation failed">(res);
            return as;
        }
        as_i.device = device;
        as_i.vkDestroyAccelerationStructureKHR = table.vkDestroyAccelerationStructureKHR;
        return as;
    }

    [[nodiscard]] uint64_t GetAccelerationStructureDeviceAddress(wis::VKAccelerationStructureView as) const noexcept
    {
        VkAccelerationStructureDeviceAddressInfoKHR info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
            .accelerationStructure = std::get<0>(as)
        };
        return table.vkGetAccelerationStructureDeviceAddressKHR(device.get(), &info);
    }

    [[nodiscard]] WIS_INLINE wis::VKRaytracingPipeline
    CreateRaytracingPipeline(wis::Result& result, const wis::VKRaytracingPipeineDesc& rt_pipeline_desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ShaderBindingTableInfo GetShaderBindingTableInfo() const noexcept
    {
        return sbt_info;
    }

public:
    void WIS_INLINE CopyAccelerationStructure(wis::VKCommandListView cmd_list,
                                              wis::VKAccelerationStructureView dst,
                                              wis::VKAccelerationStructureView src,
                                              wis::ASCopyMode mode) const noexcept;

    void WIS_INLINE BuildBottomLevelAS(wis::VKCommandListView cmd_buffer,
                                       const wis::VKBottomLevelASBuildDesc& blas_desc,
                                       wis::VKAccelerationStructureView dst_acceleration_structure,
                                       uint64_t scratch_buffer_gpu_address,
                                       wis::VKAccelerationStructureView src_acceleration_structure = {}) const noexcept;

    void WIS_INLINE BuildTopLevelAS(wis::VKCommandListView cmd_buffer,
                                    const wis::TopLevelASBuildDesc& tlas_desc,
                                    wis::VKAccelerationStructureView dst_acceleration_structure,
                                    uint64_t scratch_buffer_gpu_address,
                                    wis::VKAccelerationStructureView src_acceleration_structure = {}) const noexcept;

    void SetPipelineState(wis::VKCommandListView cmd_list, wis::VKRaytracingPipelineView pipeline) const noexcept
    {
        device.table().vkCmdBindPipeline(std::get<0>(cmd_list), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, std::get<0>(pipeline));
    }
    void SetDescriptorStorage(wis::VKCommandList& cmd_list, wis::VKDescriptorStorageView desc_storage) const noexcept
    {
        cmd_list.VKSetDescriptorStorage(desc_storage, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR);
    }
    void PushDescriptor(wis::VKCommandList& cmd_list, wis::DescriptorType type, uint32_t binding, wis::VKBufferView view, uint32_t offset) const noexcept
    {
        cmd_list.VKPushDescriptor(type, binding, view, offset, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR);
    }

    void DispatchRays(wis::VKCommandListView cmd_list, const wis::RaytracingDispatchDesc& desc) const noexcept
    {
        auto* cmd_list_i = std::get<0>(cmd_list);
        VkStridedDeviceAddressRegionKHR raygen{
            .deviceAddress = desc.ray_gen_shader_table_address,
            .stride = desc.ray_gen_shader_table_size,
            .size = desc.ray_gen_shader_table_size,
        };
        VkStridedDeviceAddressRegionKHR miss{
            .deviceAddress = desc.miss_shader_table_address,
            .stride = desc.miss_shader_table_stride,
            .size = desc.miss_shader_table_size
        };
        VkStridedDeviceAddressRegionKHR hit{
            .deviceAddress = desc.hit_group_table_address,
            .stride = desc.hit_group_table_stride,
            .size = desc.hit_group_table_size
        };
        VkStridedDeviceAddressRegionKHR callable{
            .deviceAddress = desc.callable_shader_table_address,
            .stride = desc.callable_shader_table_stride,
            .size = desc.callable_shader_table_size
        };
        table.vkCmdTraceRaysKHR(cmd_list_i, &raygen, &miss, &hit, &callable, desc.width, desc.height, desc.depth);
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

#ifndef WISDOM_BUILD_BINARIES
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_VULKAN
#endif // !WISDOM_RAYTRACING_HPP
