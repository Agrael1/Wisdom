#pragma once
#include <array>
#include <vulkan/vulkan.h>
#include <wisvk/vk_libinit.hpp>

namespace wis {

#if !(defined(VK_KHR_ray_tracing_pipeline))
#if defined(VK_NV_ray_tracing)
using PFN_vkGetRayTracingShaderGroupHandlesKHR = PFN_vkGetRayTracingShaderGroupHandlesNV;
#endif
#endif

struct VKRaytracingExtDevice {
    PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
    PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
    PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
    PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
    PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        vkCreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR)vkGetDeviceProcAddr(device, "vkCreateRayTracingPipelinesKHR");
        if (vkCreateRayTracingPipelinesKHR == nullptr) {
            return false;
        }
        vkCmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR)vkGetDeviceProcAddr(device, "vkCmdTraceRaysKHR");
        if (vkCmdTraceRaysKHR == nullptr) {
            return false;
        }
        vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)vkGetDeviceProcAddr(device, "vkCreateAccelerationStructureKHR");
        if (vkCreateAccelerationStructureKHR == nullptr) {
            return false;
        }
        vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)vkGetDeviceProcAddr(device, "vkGetAccelerationStructureBuildSizesKHR");
        if (vkGetAccelerationStructureBuildSizesKHR == nullptr) {
            return false;
        }
        vkDestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR)vkGetDeviceProcAddr(device, "vkDestroyAccelerationStructureKHR");
        if (vkDestroyAccelerationStructureKHR == nullptr) {
            return false;
        }
        vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)vkGetDeviceProcAddr(device, "vkCmdBuildAccelerationStructuresKHR");
        if (vkCmdBuildAccelerationStructuresKHR == nullptr) {
            return false;
        }
        vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)vkGetDeviceProcAddr(device, "vkGetAccelerationStructureDeviceAddressKHR");
        if (vkGetAccelerationStructureDeviceAddressKHR == nullptr) {
            return false;
        }
        vkCmdCopyAccelerationStructureKHR = (PFN_vkCmdCopyAccelerationStructureKHR)vkGetDeviceProcAddr(device, "vkCmdCopyAccelerationStructureKHR");
        if (vkCmdCopyAccelerationStructureKHR == nullptr) {
            return false;
        }
        static constexpr std::array vkGetRayTracingShaderGroupHandlesKHR_strings{
#if defined(VK_KHR_ray_tracing_pipeline)
            "vkGetRayTracingShaderGroupHandlesKHR",
#endif
#if defined(VK_NV_ray_tracing)
            "vkGetRayTracingShaderGroupHandlesNV",
#endif
        };
        for (auto vkGetRayTracingShaderGroupHandlesKHR_it : vkGetRayTracingShaderGroupHandlesKHR_strings) {
            if ((vkGetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR)vkGetDeviceProcAddr(device, vkGetRayTracingShaderGroupHandlesKHR_it))) {
                break;
            }
        }
        if (vkGetRayTracingShaderGroupHandlesKHR == nullptr) {
            return false;
        }
        return true;
    }
};

} // namespace wis
