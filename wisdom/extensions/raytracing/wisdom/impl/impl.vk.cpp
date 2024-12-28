#ifndef WISDOM_RAYTRACING_VK_CPP
#define WISDOM_RAYTRACING_VK_CPP
#include <wisdom/wisdom_raytracing.hpp>

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

    ext_name_set.emplace(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR] = sizeof(VkPhysicalDeviceRayTracingPipelineFeaturesKHR);
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR] = sizeof(VkPhysicalDeviceRayTracingPipelinePropertiesKHR);

    ext_name_set.emplace(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR] = sizeof(VkPhysicalDeviceAccelerationStructureFeaturesKHR);
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR] = sizeof(VkPhysicalDeviceAccelerationStructurePropertiesKHR);

    ext_name_set.emplace(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
    return supports_raytracing = true;
}

wis::Result wis::ImplVKRaytracing::Init(const wis::VKDevice& instance,
                                        const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                        const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    device = instance.GetInternal().device;
    return wis::success;
}

#endif // WISDOM_VULKAN
#endif // WISDOM_RAYTRACING_VK_CPP