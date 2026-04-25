#ifndef WIS_VK_RAYTRACING_CPP
#define WIS_VK_RAYTRACING_CPP

#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>
#include <raytracing/generated/cpp_api.hpp>

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

WIS_EXTERN_C WISDOM_RAYTRACING_API void wisVKInitRaytracingExtension(WisVKRaytracingExtension* self)
{
    new (self) wis::impl::VKRaytracingExtensionImpl{
        .header = {&wis::detail::VKRaytracingExtensionInit},
    };
}

WIS_EXTERN_C WISDOM_RAYTRACING_API void wisVKDestroyRaytracingExtension(WisVKRaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);
    if (impl.device_control_block) {
        delete impl.rt_table;
        wis::detail::VKReleaseDevice(impl.device_control_block);
    }
    impl.header = {}; // Clear header to prevent accidental use after destruction
}

WIS_EXTERN_C WISDOM_RAYTRACING_API bool wisVKRaytracingExtensionSupported(WisVKRaytracingExtension* self) 
{
    auto& impl = wis::from_handle_ref<wis::impl::VKRaytracingExtensionImpl>(self);
    return impl.device_control_block != nullptr; // Supported if the extension was successfully initialized
}

#endif
