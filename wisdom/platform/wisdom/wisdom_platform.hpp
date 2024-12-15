#ifndef WISDOM_PLATFORM_H
#define WISDOM_PLATFORM_H

#ifdef WIS_WINDOWS_PLATFORM
#include <wisdom/wisdom_windows.hpp>
#else
#include <wisdom/linux/interop_device.hpp>
#endif

#ifdef WIS_XCB_PLATFORM
#include <wisdom/wisdom_xcb.hpp>
#endif // WIS_XCB_PLATFORM
#ifdef WIS_X11_PLATFORM
#include <wisdom/wisdom_x11.hpp>
#endif // WIS_X11_PLATFORM
#ifdef WIS_WAYLAND_PLATFORM
#include <wisdom/wisdom_wayland.hpp>
#endif // WIS_WAYLAND_PLATFORM

#endif // WISDOM_PLATFORM_H
