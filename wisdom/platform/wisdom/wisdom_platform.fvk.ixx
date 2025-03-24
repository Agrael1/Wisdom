module;
#define WISDOM_MODULE_DECL
#include <wisdom/platform.include.h>
#define WISDOM_EXPORT export
export module wisdom.platform.fvk;

#ifdef WIS_XCB_PLATFORM
export import wisdom.xcb;
#endif // WIS_XCB_PLATFORM
#ifdef WIS_X11_PLATFORM
export import wisdom.x11;
#endif // WIS_X11_PLATFORM
#ifdef WIS_WAYLAND_PLATFORM
export import wisdom.wayland;
#endif // WIS_WAYLAND_PLATFORM

#ifdef WIS_WINDOWS_PLATFORM
export import wisdom.windows.fvk;
#else
import wisdom.vk;
#include <wisdom/linux/interop_device.hpp>
#endif
