// This file is generated. Do not edit directly.
#ifndef WISDOM_PLATFORM_HPP
#define WISDOM_PLATFORM_HPP

#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#ifndef FORCEVK_SWITCH
#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "../wisdom_platform/generated/cpp_api.hpp"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

namespace wis {

//==============================================================
// Handles
//==============================================================

using Win32Extension = wis::DX12Win32Extension;
using UWPExtension   = wis::DX12UWPExtension;

} // namespace wis

#elif defined(WISDOM_VULKAN)

namespace wis {

//==============================================================
// Handles
//==============================================================

using XlibExtension    = wis::VKXlibExtension;
using XCBExtension     = wis::VKXCBExtension;
using WaylandExtension = wis::VKWaylandExtension;
using Win32Extension   = wis::VKWin32Extension;

} // namespace wis
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_PLATFORM_HPP
