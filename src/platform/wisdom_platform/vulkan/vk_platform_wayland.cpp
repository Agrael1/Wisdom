#ifndef WIS_VK_PLATFORM_WAYLAND_CPP
#define WIS_VK_PLATFORM_WAYLAND_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WAYLAND_PRESENT)

#ifndef WIS_USE_PLATFORM_WAYLAND
#define WIS_USE_PLATFORM_WAYLAND 1
#endif
#include <wisdom/wisdom.h>
#include <wisdom_platform/generated/c_platform_api.h>

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WAYLAND_PRESENT)
#endif // WIS_VK_PLATFORM_WAYLAND_CPP
