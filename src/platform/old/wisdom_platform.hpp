#ifndef WISDOM_PLATFORM_H
#define WISDOM_PLATFORM_H

#if defined(WIS_USE_PLATFORM_WIN32)
#ifndef WIS_PLATFORM_WIN32_PRESENT
#error "WIS_USE_PLATFORM_WIN32 is defined, but Win32 platform extension is not available."
#endif
#include <wisdom/wisdom_windows.hpp>
#elif defined(WIS_USE_PLATFORM_UWP)
#ifndef WIS_PLATFORM_UWP_PRESENT
#error "WIS_USE_PLATFORM_UWP is defined, but UWP platform extension is not available."
#endif
#include <wisdom/wisdom_windows.hpp>
#else
#include <wisdom/linux/interop_device.hpp>
#endif

#ifdef WIS_USE_PLATFORM_XCB
#ifndef WIS_PLATFORM_XCB_PRESENT
#error "WIS_USE_PLATFORM_XCB is defined, but XCB platform extension is not available."
#endif
#include <wisdom/wisdom_xcb.hpp>
#endif // WIS_USE_PLATFORM_XCB

#ifdef WIS_USE_PLATFORM_XLIB
#ifndef WIS_PLATFORM_XLIB_PRESENT
#error "WIS_USE_PLATFORM_XLIB is defined, but Xlib platform extension is not available."
#endif
#include <wisdom/wisdom_x11.hpp>
#endif // WIS_USE_PLATFORM_XLIB

#ifdef WIS_USE_PLATFORM_WAYLAND
#ifndef WIS_PLATFORM_WAYLAND_PRESENT
#error "WIS_USE_PLATFORM_WAYLAND is defined, but Wayland platform extension is not available."
#endif
#include <wisdom/wisdom_wayland.hpp>
#endif // WIS_USE_PLATFORM_WAYLAND

#endif // WISDOM_PLATFORM_H
