#ifndef WISDOM_PLATFORM_H
#define WISDOM_PLATFORM_H

#ifdef WIS_WINDOWS_PLATFORM
#include <wisdom/wisdom_windows.h>
#else 
#include <wisdom/linux/interop_device.h>
#endif

#ifdef WIS_XCB_PLATFORM
#include <wisdom/wisdom_xcb.h>
#endif // WIS_XCB_PLATFORM
#ifdef WIS_WAYLAND_PLATFORM
#include <wisdom/wisdom_wayland.h>
#endif // WIS_WAYLAND_PLATFORM

#endif // WISDOM_PLATFORM_H
