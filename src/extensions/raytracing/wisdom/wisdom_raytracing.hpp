// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_HPP
#define WISDOM_RAYTRACING_HPP

#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#ifndef FORCEVK_SWITCH
#    if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#        define FORCEVK_SWITCH 1
#    else
#        define FORCEVK_SWITCH 0
#    endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "../raytracing/generated/cpp_api.hpp"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

namespace wis {

//==============================================================
// Handles
//==============================================================

using AccelerationStructure = wis::DX12AccelerationStructure;
using RaytracingExtension = wis::DX12RaytracingExtension;

//==============================================================
// Variants
//==============================================================

using AccelerationStructureDesc = wis::DX12AccelerationStructureDesc;

} // namespace wis

#elif defined(WISDOM_VULKAN)

namespace wis {

//==============================================================
// Handles
//==============================================================

using AccelerationStructure = wis::VKAccelerationStructure;
using RaytracingExtension = wis::VKRaytracingExtension;

//==============================================================
// Variants
//==============================================================

using AccelerationStructureDesc = wis::VKAccelerationStructureDesc;

} // namespace wis
#else
#    error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_RAYTRACING_HPP
