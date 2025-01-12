#ifndef WISDOM_RAYTRACING_VK_CPP
#define WISDOM_RAYTRACING_VK_CPP
#include <wisdom/impl.vk.h>

#if defined(WISDOM_VULKAN)
bool wis::ImplVKRaytracing::GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                                             std::unordered_set<std::string_view>& ext_name_set,
                                             std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                             std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    if (!available_extensions.contains(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME)) {
        return false;
    }
    if (!available_extensions.contains(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME)) {
        return false;
    }
    if (!available_extensions.contains(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME)) {
        return false;
    }
    if (!available_extensions.contains(VK_KHR_RAY_QUERY_EXTENSION_NAME)) {
        return false;
    }

    ext_name_set.emplace(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR] = sizeof(VkPhysicalDeviceRayTracingPipelineFeaturesKHR);
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR] = sizeof(VkPhysicalDeviceRayTracingPipelinePropertiesKHR);

    ext_name_set.emplace(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR] = sizeof(VkPhysicalDeviceAccelerationStructureFeaturesKHR);
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR] = sizeof(VkPhysicalDeviceAccelerationStructurePropertiesKHR);

    ext_name_set.emplace(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
    ext_name_set.emplace(VK_KHR_RAY_QUERY_EXTENSION_NAME);
    structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR] = sizeof(VkPhysicalDeviceRayQueryFeaturesKHR);
    return true;
}

wis::Result wis::ImplVKRaytracing::Init(const wis::VKDevice& instance,
                                        const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                        const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    device = instance.GetInternal().device;
    table.Init(device.get(), instance.GetInternal().device.gtable().vkGetDeviceProcAddr);

    auto props = reinterpret_cast<const VkPhysicalDeviceRayTracingPipelinePropertiesKHR*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR));

    compressed_handle_size = props->shaderGroupHandleSize;
    sbt_info = { wis::detail::aligned_size(compressed_handle_size, props->shaderGroupHandleAlignment), props->shaderGroupBaseAlignment };
    return wis::success;
}

wis::ASAllocationInfo wis::ImplVKRaytracing::GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc)
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
    table.vkGetAccelerationStructureBuildSizesKHR(device.get(),
                                                  VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                                                  &build_info,
                                                  &max_instance_count,
                                                  &build_sizes_info);
    return { build_sizes_info.buildScratchSize, build_sizes_info.accelerationStructureSize, build_sizes_info.updateScratchSize };
}

wis::ASAllocationInfo wis::ImplVKRaytracing::GetBottomLevelASSize(const wis::VKBottomLevelASBuildDesc& blas_desc) const noexcept
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
    table.vkGetAccelerationStructureBuildSizesKHR(device.get(),
                                                  VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                                                  &build_info,
                                                  max_primitive_count,
                                                  &build_sizes_info);
    return { build_sizes_info.buildScratchSize, build_sizes_info.accelerationStructureSize, build_sizes_info.updateScratchSize };
}

wis::VKRaytracingPipeline
wis::ImplVKRaytracing::CreateRaytracingPipeline(wis::Result& result, const wis::VKRaytracingPipeineDesc& rt_pipeline_desc) const noexcept
{
    wis::VKRaytracingPipeline pipeline;
    auto& pipe_i = pipeline.GetMutableInternal();


    uint32_t raygen_count = 0;
    uint32_t miss_count = 0;
    uint32_t callable_count = 0;
    for (size_t i = 0; i < rt_pipeline_desc.export_count; ++i) {
        raygen_count += rt_pipeline_desc.exports[i].shader_type == wis::RaytracingShaderType::Raygen;
        miss_count += rt_pipeline_desc.exports[i].shader_type == wis::RaytracingShaderType::Miss;
        callable_count += rt_pipeline_desc.exports[i].shader_type == wis::RaytracingShaderType::Callable;
    }

    // initialize shader stages
    std::unique_ptr<uint8_t[]> stages = wis::detail::make_unique_for_overwrite<uint8_t[]>(
            rt_pipeline_desc.export_count * sizeof(VkPipelineShaderStageCreateInfo) +
            raygen_count * sizeof(VkRayTracingShaderGroupCreateInfoKHR) +
            miss_count * sizeof(VkRayTracingShaderGroupCreateInfoKHR) +
            rt_pipeline_desc.hit_group_count * sizeof(VkRayTracingShaderGroupCreateInfoKHR) +
            callable_count * sizeof(VkRayTracingShaderGroupCreateInfoKHR));
    if (!stages) {
        result = wis::make_result<FUNC, "Failed to allocate memory for shader stages">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return pipeline;
    }

    // create memory for shader group handles
    uint32_t handle_count = raygen_count + miss_count + rt_pipeline_desc.hit_group_count + callable_count;
    uint32_t handle_size = handle_count * sbt_info.entry_size;
    pipe_i.shader_identifiers = wis::detail::make_unique_for_overwrite<uint8_t[]>(handle_size); // expanded size for all shader groups
    auto* handles = pipe_i.shader_identifiers.get();
    std::memset(handles, 0, handle_size);

    // initialize shader stages + raygen + miss shader groups
    std::span<VkPipelineShaderStageCreateInfo> stages_span{ reinterpret_cast<VkPipelineShaderStageCreateInfo*>(stages.get()), rt_pipeline_desc.export_count };
    std::span<VkRayTracingShaderGroupCreateInfoKHR> raygen_span{ reinterpret_cast<VkRayTracingShaderGroupCreateInfoKHR*>(stages_span.data() + rt_pipeline_desc.export_count), raygen_count };
    std::span<VkRayTracingShaderGroupCreateInfoKHR> miss_span{ raygen_span.data() + raygen_count, miss_count };
    std::span<VkRayTracingShaderGroupCreateInfoKHR> hit_group_span{ miss_span.data() + miss_count, rt_pipeline_desc.hit_group_count };
    std::span<VkRayTracingShaderGroupCreateInfoKHR> callable_span{ hit_group_span.data() + rt_pipeline_desc.hit_group_count, callable_count };

    raygen_count = 0;
    miss_count = 0;
    callable_count = 0;
    for (uint32_t i = 0; i < rt_pipeline_desc.export_count; ++i) {
        auto& _export = rt_pipeline_desc.exports[i]; // export is a keyword
        stages_span[i] = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = convert_vk(_export.shader_type),
            .module = std::get<0>(rt_pipeline_desc.shaders[_export.shader_array_index]),
            .pName = _export.entry_point,
        };

        switch (_export.shader_type) {
        case wis::RaytracingShaderType::Raygen:
            raygen_span[raygen_count++] = {
                .sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
                .type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
                .generalShader = i,
                .closestHitShader = VK_SHADER_UNUSED_KHR,
                .anyHitShader = VK_SHADER_UNUSED_KHR,
                .intersectionShader = VK_SHADER_UNUSED_KHR,
            };
            break;
        case wis::RaytracingShaderType::Miss:
            miss_span[miss_count++] = {
                .sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
                .type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
                .generalShader = i,
                .closestHitShader = VK_SHADER_UNUSED_KHR,
                .anyHitShader = VK_SHADER_UNUSED_KHR,
                .intersectionShader = VK_SHADER_UNUSED_KHR,
            };
            break;
        case wis::RaytracingShaderType::Callable:
            callable_span[callable_count++] = {
                .sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
                .type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
                .generalShader = i,
                .closestHitShader = VK_SHADER_UNUSED_KHR,
                .anyHitShader = VK_SHADER_UNUSED_KHR,
                .intersectionShader = VK_SHADER_UNUSED_KHR,
            };
            break;
        }
    }

    // initialize hit groups
    for (uint32_t i = 0; i < rt_pipeline_desc.hit_group_count; ++i) {
        auto& hg = rt_pipeline_desc.hit_groups[i];
        hit_group_span[i] = {
            .sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
            .type = convert_vk(hg.type),
            .generalShader = VK_SHADER_UNUSED_KHR,
            .closestHitShader = hg.closest_hit_export_index == UINT32_MAX ? VK_SHADER_UNUSED_KHR : hg.closest_hit_export_index,
            .anyHitShader = hg.any_hit_export_index == UINT32_MAX ? VK_SHADER_UNUSED_KHR : hg.any_hit_export_index,
            .intersectionShader = hg.intersection_export_index == UINT32_MAX ? VK_SHADER_UNUSED_KHR : hg.intersection_export_index
        };
    }

    VkRayTracingPipelineInterfaceCreateInfoKHR interface_info{
        .sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_KHR,
        .maxPipelineRayPayloadSize = rt_pipeline_desc.max_payload_size,
        .maxPipelineRayHitAttributeSize = rt_pipeline_desc.max_attribute_size,
    };

    VkRayTracingPipelineCreateInfoKHR pipeline_info{
        .sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
        .stageCount = uint32_t(stages_span.size()),
        .pStages = stages_span.data(),
        .groupCount = raygen_count + miss_count + rt_pipeline_desc.hit_group_count,
        .pGroups = raygen_span.data(),
        .maxPipelineRayRecursionDepth = rt_pipeline_desc.max_recursion_depth, // user defined
        .layout = std::get<0>(rt_pipeline_desc.root_signature),
    };

    auto vr = table.vkCreateRayTracingPipelinesKHR(device.get(), VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, pipe_i.state_object.put(device, device.table().vkDestroyPipeline));
    if (!wis::succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create raytracing pipeline">(vr);
    }

    // retrieve and uncompress shader group handles
    table.vkGetRayTracingShaderGroupHandlesKHR(device.get(), pipe_i.state_object.get(), 0, handle_count, handle_size, handles);
    if (sbt_info.entry_size == compressed_handle_size) {
        return pipeline;
    }

    // uncompress shader group handles in-place
    uint8_t* uncompressed_handles = handles + handle_size;
    uint8_t* compressed_handles = handles + handle_count * compressed_handle_size;
    while (compressed_handles != handles) {
        compressed_handles -= compressed_handle_size;
        uncompressed_handles -= sbt_info.entry_size;
        std::memmove(uncompressed_handles, compressed_handles, sbt_info.entry_size);
        std::memset(compressed_handles, 0, size_t(uncompressed_handles - compressed_handles));
    }

    return pipeline;
}

void wis::ImplVKRaytracing::BuildBottomLevelAS(wis::VKCommandListView cmd_buffer, const wis::VKBottomLevelASBuildDesc& blas_desc, wis::VKAccelerationStructureView dst_acceleration_structure, uint64_t scratch_buffer_gpu_address, wis::VKAccelerationStructureView src_acceleration_structure) const noexcept
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
    table.vkCmdBuildAccelerationStructuresKHR(std::get<0>(cmd_buffer), 1, &build_info, pp_ranges);
}

void wis::ImplVKRaytracing::BuildTopLevelAS(wis::VKCommandListView cmd_buffer, const wis::TopLevelASBuildDesc& tlas_desc, wis::VKAccelerationStructureView dst_acceleration_structure, uint64_t scratch_buffer_gpu_address, wis::VKAccelerationStructureView src_acceleration_structure) const noexcept
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
    table.vkCmdBuildAccelerationStructuresKHR(std::get<0>(cmd_buffer), 1, &build_info, &p_range_info);
}
#endif // WISDOM_VULKAN
#endif // WISDOM_RAYTRACING_VK_CPP