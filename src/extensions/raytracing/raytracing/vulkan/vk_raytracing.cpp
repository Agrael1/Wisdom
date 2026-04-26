#ifndef WIS_VK_RAYTRACING_CPP
#define WIS_VK_RAYTRACING_CPP

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

    if (!buffer_impl.buffer_header) {
        return wis::detail::make_result<
            wis::detail::Func(),
            "Provided buffer is not suitable for acceleration structure creation, did you forget to add "
            "WisBufferUsageFlagsAccelerationStructureBuffer?">(VK_ERROR_UNKNOWN);
    }

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
        .buffer_control_block = buffer_impl.buffer_header,
    };
    buffer_impl.buffer_header->AddRef(); // Hold reference to device control block for acceleration structure
    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API void wisVKDestroyAccelerationStructure(WisVKAccelerationStructure* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKAccelerationStructureImpl>(self);
    if (impl.acceleration_structure != VK_NULL_HANDLE) {
        impl.buffer_control_block->header.device_table->vkDestroyAccelerationStructureKHR(
            impl.buffer_control_block->header.device,
            impl.acceleration_structure,
            nullptr
        );
        impl.acceleration_structure = VK_NULL_HANDLE;

        wis::detail::VKReleaseBuffer(impl.buffer_control_block); // Release reference to device control block
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_RAYTRACING_API uint64_t wisVKAccelerationStructureGetGPUAddress(WisVKAccelerationStructure* self)
{
    return wis::from_handle_ref<wis::impl::VKAccelerationStructureImpl>(self).device_address;
}

#endif
