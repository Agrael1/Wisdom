#ifndef WIS_VK_PLATFORM_WIN32_CPP
#define WIS_VK_PLATFORM_WIN32_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WIN32_PRESENT)

#ifndef WIS_USE_PLATFORM_WIN32
#define WIS_USE_PLATFORM_WIN32 1
#endif
#include <wisdom/wisdom.h>
#include <wisdom_platform/generated/c_platform_api.h>

WIS_EXTERN_C WISDOM_API void wisVKDestroyWin32Extension(WisVKWin32Extension* self)
{
    (void)self;
}

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WIN32_PRESENT)
#endif // WIS_VK_PLATFORM_WIN32_CPP
