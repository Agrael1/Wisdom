#ifndef WIS_VK_RAYTRACING_TABLES_HPP
#define WIS_VK_RAYTRACING_TABLES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus
#include <wisdom/vulkan/vk_loader.hpp>

#include <vulkan/vulkan.h>

namespace wis {
namespace impl {
struct VKRaytracingPipelineTable {
    PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
    PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
    PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
    PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
    PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;

    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateAccelerationStructureKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetAccelerationStructureBuildSizesKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyAccelerationStructureKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdCopyAccelerationStructureKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdBuildAccelerationStructuresKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetAccelerationStructureDeviceAddressKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetRayTracingShaderGroupHandlesKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdTraceRaysKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateRayTracingPipelinesKHR);
        return true;
    }
};
} // namespace impl
} // namespace wis

#endif // !WIS_VK_RAYTRACING_TABLES_HPP
