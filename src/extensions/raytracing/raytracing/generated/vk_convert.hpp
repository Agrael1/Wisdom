// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_CPP_VK_CONVERT_HPP
#define WISDOM_RAYTRACING_CPP_VK_CONVERT_HPP
#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>
#include "c_api.h"

namespace wis {
namespace detail {
constexpr inline VkAccelerationStructureTypeKHR VKConvert(WisAccelerationStructureLevel value) noexcept
{
    switch (value) {
    case WisAccelerationStructureLevelTopLevel:
        return VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    case WisAccelerationStructureLevelBottomLevel:
        return VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
    default:
        return static_cast<VkAccelerationStructureTypeKHR>(0);
    }
}

constexpr inline VkGeometryTypeKHR VKConvert(WisGeometryType value) noexcept
{
    switch (value) {
    case WisGeometryTypeTriangles:
        return VK_GEOMETRY_TYPE_TRIANGLES_KHR;
    case WisGeometryTypeAABBs:
        return VK_GEOMETRY_TYPE_AABBS_KHR;
    default:
        return static_cast<VkGeometryTypeKHR>(0);
    }
}

constexpr inline VkGeometryFlagsKHR VKConvert(WisGeometryFlags value) noexcept
{
    VkGeometryFlagsKHR result = static_cast<VkGeometryFlagsKHR>(0);
    if (value & WisGeometryFlagsOpaque) {
        result |= VK_GEOMETRY_OPAQUE_BIT_KHR;
    }
    if (value & WisGeometryFlagsNoDuplicateAnyHitInvocation) {
        result |= VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
    }
    return result;
}

constexpr inline VkBuildAccelerationStructureFlagsKHR VKConvert(WisAccelerationStructureFlags value) noexcept
{
    VkBuildAccelerationStructureFlagsKHR result = static_cast<VkBuildAccelerationStructureFlagsKHR>(0);
    if (value & WisAccelerationStructureFlagsAllowUpdate) {
        result |= VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
    }
    if (value & WisAccelerationStructureFlagsAllowCompaction) {
        result |= VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR;
    }
    if (value & WisAccelerationStructureFlagsPreferFastTrace) {
        result |= VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
    }
    if (value & WisAccelerationStructureFlagsPreferFastBuild) {
        result |= VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
    }
    if (value & WisAccelerationStructureFlagsMinimizeMemory) {
        result |= VK_BUILD_ACCELERATION_STRUCTURE_LOW_MEMORY_BIT_KHR;
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_RAYTRACING_CPP_VK_CONVERT_HPP
