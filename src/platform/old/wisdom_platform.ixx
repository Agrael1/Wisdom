module;
#define WISDOM_MODULE_DECL
#include <wisdom/platform.include.h>
export module wisdom.platform;

#ifdef WIS_USE_PLATFORM_XCB
#ifndef WIS_PLATFORM_XCB_PRESENT
#error "WIS_USE_PLATFORM_XCB is defined, but XCB platform extension is not available."
#endif
export import wisdom.xcb;
#endif // WIS_USE_PLATFORM_XCB

#ifdef WIS_USE_PLATFORM_XLIB
#ifndef WIS_PLATFORM_XLIB_PRESENT
#error "WIS_USE_PLATFORM_XLIB is defined, but Xlib platform extension is not available."
#endif
export import wisdom.x11;
#endif // WIS_USE_PLATFORM_XLIB

#ifdef WIS_USE_PLATFORM_WAYLAND
#ifndef WIS_PLATFORM_WAYLAND_PRESENT
#error "WIS_USE_PLATFORM_WAYLAND is defined, but Wayland platform extension is not available."
#endif
export import wisdom.wayland;
#endif // WIS_USE_PLATFORM_WAYLAND

#if defined(WIS_USE_PLATFORM_WIN32)
#ifndef WIS_PLATFORM_WIN32_PRESENT
#error "WIS_USE_PLATFORM_WIN32 is defined, but Win32 platform extension is not available."
#endif
export import wisdom.windows;
#elif defined(WIS_USE_PLATFORM_UWP)
#ifndef WIS_PLATFORM_UWP_PRESENT
#error "WIS_USE_PLATFORM_UWP is defined, but UWP platform extension is not available."
#endif
export import wisdom.windows;
#else
import wisdom.vk;
#include <wisdom/linux/interop_device.hpp>
#endif
