#ifndef WISDOM_RAYTRACING_VK_HPP
#define WISDOM_RAYTRACING_VK_HPP
#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vk_rtas.h>
#include "generated/vk_functions.hpp"

namespace wis {
/// @brief Raytracing extension for Vulkan
class VKRaytracingPipeline : public VKPipelineState // doesn't differ from VKPipelineState
{
    using VKPipelineState::VKPipelineState;
};


class VKRaytracingShaderTable;

class VKRaytracing;

template<>
struct Internal<VKRaytracing> {
    wis::SharedDevice device;
    wis::VKRaytracingExtDevice table;
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
    [[nodiscard]] WIS_INLINE wis::ASAllocationInfo
    GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc);

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
            result = wis::make_result<FUNC, "Acceleration structure creation failed">(res);
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

public:
    void
    BuildBottomLevelAS(wis::VKCommandListView cmd_buffer,
                       const wis::VKBottomLevelASBuildDesc& blas_desc,
                       wis::VKAccelerationStructureView dst_acceleration_structure,
                       uint64_t scratch_buffer_gpu_address,
                       wis::VKAccelerationStructureView src_acceleration_structure = {}) const noexcept;

    void BuildTopLevelAS(wis::VKCommandListView cmd_buffer,
                         const wis::TopLevelASBuildDesc& tlas_desc,
                         wis::VKAccelerationStructureView dst_acceleration_structure,
                         uint64_t scratch_buffer_gpu_address,
                         wis::VKAccelerationStructureView src_acceleration_structure = {}) const noexcept;

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

#ifndef WISDOM_BUILD_BINARIES
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_VULKAN
#endif // !WISDOM_RAYTRACING_HPP