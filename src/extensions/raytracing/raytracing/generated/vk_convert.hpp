// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_CPP_VK_CONVERT_HPP
#define WISDOM_RAYTRACING_CPP_VK_CONVERT_HPP
#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

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

} // namespace detail
} // namespace wis
#endif // WISDOM_RAYTRACING_CPP_VK_CONVERT_HPP
