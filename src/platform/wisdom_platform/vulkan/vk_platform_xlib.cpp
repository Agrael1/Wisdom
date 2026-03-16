#ifndef WIS_VK_PLATFORM_XLIB_CPP
#define WIS_VK_PLATFORM_XLIB_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XLIB_PRESENT)

#ifndef WIS_USE_PLATFORM_XLIB
#define WIS_USE_PLATFORM_XLIB 1
#endif
#include <wisdom/wisdom.h>
#include <wisdom_platform/generated/c_platform_api.h>

WIS_EXTERN_C WISDOM_API void wisVKDestroyXlibExtension(WisVKXlibExtension* self)
{
    (void)self;
}

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XLIB_PRESENT)
#endif // WIS_VK_PLATFORM_XLIB_CPP
