#ifndef WIS_VK_PLATFORM_TYPES_HPP
#define WIS_VK_PLATFORM_TYPES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#define WISDOM_BUILD_BINARIES 1
#include <wisdom/generated/vk_cpp_api.hpp>

namespace wis {
//-----------------------------------------------------------------------------
namespace detail {
struct VKXlibExtensionTable;
struct VKXCBExtensionTable;
struct VKWaylandExtensionTable;
struct VKWin32ExtensionTable;
} // namespace detail

namespace impl {
struct VKXlibExtensionImpl {
    VKInstanceExtensionHeader       header;
    detail::VKInstanceControlBlock* instance_control_block;
    PFN_vkVoidFunction              vkCreateXlibSurfaceKHR;
};

struct VKXCBExtensionImpl {
    VKInstanceExtensionHeader       header;
    detail::VKInstanceControlBlock* instance_control_block;
    PFN_vkVoidFunction              vkCreateXcbSurfaceKHR;
};

struct VKWaylandExtensionImpl {
    VKInstanceExtensionHeader       header;
    detail::VKInstanceControlBlock* instance_control_block;
    PFN_vkVoidFunction              vkCreateWaylandSurfaceKHR;
};

struct VKWin32ExtensionImpl {
    VKInstanceExtensionHeader       header; // The instance extension header, containing the function pointer for initialization
    detail::VKInstanceControlBlock* instance_control_block; // Pointer to the instance control block, used to access instance-level data and functions
    PFN_vkVoidFunction              vkCreateWin32SurfaceKHR; // Pointer to the function table containing the loaded extension functions
};

} // namespace impl
} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_PLATFORM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "vk_platform_win32.cpp"
#include "vk_platform_xlib.cpp"
#include "vk_platform_xcb.cpp"
#include "vk_platform_wayland.cpp"

#endif // WISDOM_BUILD_BINARIES
#endif // WIS_VK_PLATFORM_TYPES_HPP
