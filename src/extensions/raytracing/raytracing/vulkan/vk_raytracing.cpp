#ifndef WIS_VK_RAYTRACING_CPP
#define WIS_VK_RAYTRACING_CPP

#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>
#include <raytracing/generated/cpp_api.hpp>
#include <raytracing/generated/vk_convert.hpp>
#include <bit>

namespace wis::detail {
inline WisResult VKRaytracingExtensionInit(
    VKDeviceExtensionHeader* self,
    impl::VKDeviceImpl* device_impl,
    VKDeviceExtensionCollector* collector
) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);

    if (!device_impl) {
        auto& coll = *collector;
        if (!coll.IsExtensionPresent(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME)
            || !coll.IsExtensionPresent(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME)
            || !coll.IsExtensionPresent(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME)
            || !coll.IsExtensionPresent(VK_KHR_RAY_QUERY_EXTENSION_NAME)) {
            return {}; // Required extension not present
        }

        coll.EnableExtension({
            .name = VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        });

        // Ray tracing pipeline
        coll.EnableExtension(
            {.name = VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
             .feature_struct = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
             .feature_struct_size = sizeof(VkPhysicalDeviceRayTracingPipelineFeaturesKHR),
             .property_struct = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR,
             .property_struct_size = sizeof(VkPhysicalDeviceRayTracingPipelinePropertiesKHR)}
        );

        // Acceleration structure
        coll.EnableExtension(
            {.name = VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
             .feature_struct = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
             .feature_struct_size = sizeof(VkPhysicalDeviceAccelerationStructureFeaturesKHR),
             .property_struct = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR,
             .property_struct_size = sizeof(VkPhysicalDeviceAccelerationStructurePropertiesKHR)}
        );

        // Ray query
        coll.EnableExtension(
            {.name = VK_KHR_RAY_QUERY_EXTENSION_NAME,
             .feature_struct = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR,
             .feature_struct_size = sizeof(VkPhysicalDeviceRayTracingPipelineFeaturesKHR)}
        );

    } else {
        if (impl.device_control_block) {
            delete impl.rt_table;
            wis::detail::VKReleaseDevice(impl.device_control_block);
        }

        // Create table and pass
        std::unique_ptr<impl::VKRaytracingPipelineTable> rt_table{new (std::nothrow) impl::VKRaytracingPipelineTable};
        if (!rt_table) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to allocate raytracing pipeline table">(
                VK_ERROR_OUT_OF_HOST_MEMORY
            );
        }

        if (!rt_table->Init(
                device_impl->device,
                device_impl->device_header->header.shared_header->header.global_table.vkGetDeviceProcAddr
            )) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to initialize raytracing pipeline table">(
                VK_ERROR_INITIALIZATION_FAILED
            );
        }

        impl.device = device_impl->device;
        impl.device_control_block = device_impl->device_header;
        impl.device_control_block->AddRef(); // extension holds a reference to the device control block
        impl.rt_table = rt_table.release(); // ownership transferred to extension, will be freed in destructor
    }

    return wis::detail::vk_success;
}

[[nodiscard]] inline constexpr VkAccelerationStructureGeometryKHR VKCreateGeometryDesc(
    const WisAcceleratedGeometryDesc& desc
) noexcept
{
    VkAccelerationStructureGeometryKHR out{
        .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        .geometryType = VKConvert(desc.type),
        .flags = VKConvert(desc.flags)
    };
    switch (desc.type) {
    case WisGeometryTypeTriangles:
        out.geometry.triangles = {
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
            .vertexFormat = VKConvert(desc.vertex_format),
            .vertexData = {.deviceAddress = desc.vertex_or_aabb_buffer_address},
            .vertexStride = desc.vertex_or_aabb_stride,
            .maxVertex = desc.vertex_count,
            .indexType = VKConvert(desc.index_format),
            .indexData = {.deviceAddress = desc.index_buffer_address},
            .transformData = {.deviceAddress = desc.transform_matrix_address}
        };
        break;
    case WisGeometryTypeAABBs:
        out.geometry.aabbs = {
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR,
            .data = {.deviceAddress = desc.vertex_or_aabb_buffer_address},
            .stride = desc.vertex_or_aabb_stride
        };
        break;
    default:
        break;
    }
    return out;
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisVKInitRaytracingExtension(WisVKRaytracingExtension* self)
{
    new (self) wis::impl::VKRaytracingExtensionImpl{
        .header = {&wis::detail::VKRaytracingExtensionInit},
    };
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisVKDestroyRaytracingExtension(WisVKRaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);
    if (impl.device_control_block) {
        delete impl.rt_table;
        wis::detail::VKReleaseDevice(impl.device_control_block);
    }
    impl.header = {}; // Clear header to prevent accidental use after destruction
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API bool wisVKRaytracingExtensionSupported(WisVKRaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);
    return impl.device_control_block != nullptr; // Supported if the extension was successfully initialized
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API WisResult wisVKRaytracingExtensionCreateAccelerationStructure(
    WisVKRaytracingExtension* self,
    WisVKBuffer* buffer,
    const WisVKAccelerationStructureDesc* desc,
    WisVKAccelerationStructure* acceleration_structure
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);
    auto& buffer_impl = wis::from_handle_ref<wis::impl::VKBufferImpl>(buffer);

    // Build acceleration structure using the provided description
    // This is a simplified example, actual implementation would involve more detailed handling of the description
    VkAccelerationStructureCreateInfoKHR create_info{
        .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .createFlags = 0,
        .buffer = buffer_impl.buffer,
        .offset = desc->offset,
        .size = desc->size,
        .type = wis::detail::VKConvert(desc->level),
    };
    VkAccelerationStructureKHR as_handle = VK_NULL_HANDLE;
    VkResult vr = impl.rt_table->vkCreateAccelerationStructureKHR(impl.device, &create_info, nullptr, &as_handle);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create acceleration structure">(vr);
    }

    VkAccelerationStructureDeviceAddressInfoKHR info{
        .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
        .accelerationStructure = as_handle
    };

    auto& as_impl = *new (acceleration_structure) wis::impl::VKAccelerationStructureImpl{
        .acceleration_structure = as_handle,
        .device_address = impl.rt_table->vkGetAccelerationStructureDeviceAddressKHR(impl.device, &info),
        .device = impl.device,
        .vkDestroyAccelerationStructureKHR = impl.rt_table->vkDestroyAccelerationStructureKHR
    };
    // Don't ref
    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisVKDestroyAccelerationStructure(WisVKAccelerationStructure* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKAccelerationStructureImpl>(self);
    if (impl.acceleration_structure != VK_NULL_HANDLE) {
        impl.vkDestroyAccelerationStructureKHR(impl.device, impl.acceleration_structure, nullptr);
        impl.acceleration_structure = VK_NULL_HANDLE;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API uint64_t wisVKAccelerationStructureGetGPUAddress(WisVKAccelerationStructure* self)
{
    return wis::from_handle_ref<wis::impl::VKAccelerationStructureImpl>(self).device_address;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API WisResult wisVKRaytracingExtensionGetBottomLevelStructureInfo(
    WisVKRaytracingExtension* self,
    const WisBottomLevelStructureBuildDesc* build_desc,
    WisStructureAllocationInfo* info
)
{
    static constexpr size_t max_preallocated_descs = 32;
    VkAccelerationStructureGeometryKHR geometry_descs[max_preallocated_descs];
    uint32_t primitive_counts[max_preallocated_descs];
    std::unique_ptr<VkAccelerationStructureGeometryKHR[]> dynamic_descs;
    wis::span<VkAccelerationStructureGeometryKHR> geometry_desc_span;
    wis::span<uint32_t> primitive_counts_span;
    if (build_desc->geometry_count > max_preallocated_descs) {
        dynamic_descs = std::unique_ptr<VkAccelerationStructureGeometryKHR[]>{
            static_cast<VkAccelerationStructureGeometryKHR*>(::operator new(
                sizeof(VkAccelerationStructureGeometryKHR) * build_desc->geometry_count
                    + sizeof(uint32_t) * build_desc->geometry_count,
                std::nothrow
            ))
        };
        if (!dynamic_descs) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for geometry descriptions">(
                VK_ERROR_OUT_OF_HOST_MEMORY
            );
        }
        geometry_desc_span = {dynamic_descs.get(), build_desc->geometry_count};
        primitive_counts_span = {
            reinterpret_cast<uint32_t*>(dynamic_descs.get() + build_desc->geometry_count),
            build_desc->geometry_count
        };
    } else {
        geometry_desc_span = {geometry_descs, build_desc->geometry_count};
        primitive_counts_span = {primitive_counts, build_desc->geometry_count};
    }

    // Fill geometry descriptions
    if (build_desc->geometries) {
        for (uint32_t i = 0; i < build_desc->geometry_count; ++i) {
            geometry_desc_span[i] = wis::detail::VKCreateGeometryDesc(build_desc->geometries[i]);
            primitive_counts_span[i] = build_desc->geometries[i].triangle_or_aabb_count;
        }
    } else if (build_desc->indirect_geometries) {
        for (uint32_t i = 0; i < build_desc->geometry_count; ++i) {
            geometry_desc_span[i] = wis::detail::VKCreateGeometryDesc(*build_desc->indirect_geometries[i]);
            primitive_counts_span[i] = build_desc->indirect_geometries[i]->triangle_or_aabb_count;
        }
    }

    VkAccelerationStructureBuildGeometryInfoKHR build_info{
        .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
        .type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
        .flags = wis::detail::VKConvert(build_desc->flags),
        .mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
        .geometryCount = build_desc->geometry_count,
        .pGeometries = geometry_desc_span.data(),
    };
    VkAccelerationStructureBuildSizesInfoKHR build_sizes_info{
        .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR,
    };

    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);
    impl.rt_table->vkGetAccelerationStructureBuildSizesKHR(
        impl.device,
        VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
        &build_info,
        primitive_counts_span.data(),
        &build_sizes_info
    );

    constexpr static size_t alignment = 256; // 256 is a common alignment requirement for acceleration structures
    *info = {
        wis::aligned_size(build_sizes_info.buildScratchSize, alignment),
        wis::aligned_size(build_sizes_info.accelerationStructureSize, alignment),
        wis::aligned_size(build_sizes_info.updateScratchSize, alignment)
    };
    return wis::detail::vk_success;
}

#endif
