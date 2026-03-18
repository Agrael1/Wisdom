#include "sdl_platform.h"
#include <wisdom/wisdom_platform.h>
#include <stdlib.h>

SDLPlatform CreatePlatform()
{
    SDLPlatform platform      = { 0 };
    const char* platform_name = SDL_GetCurrentVideoDriver();
#if defined(SDL_PLATFORM_WIN32)
    WisWin32Extension* win32_extension = (WisWin32Extension*)malloc(sizeof(WisWin32Extension));
    if (!win32_extension) {
        return platform; // Return empty platform on allocation failure
    }

    wisInitWin32Extension(win32_extension);
    platform.platform_extension = &win32_extension->header;
    platform.extension_type     = SDL_PLATFORM_EXTENSION_WIN32;
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {

#ifdef WIS_PLATFORM_XLIB_PRESENT
        WisXlibExtension* xlib_extension = (WisXlibExtension*)malloc(sizeof(WisXlibExtension));
        if (!xlib_extension) {
            return platform; // Return empty platform on allocation failure
        }
        wisInitXlibExtension(xlib_extension);
        platform.platform_extension = &xlib_extension->header;
        platform.extension_type     = SDL_PLATFORM_EXTENSION_WIN32;
#endif

    } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {

#ifdef WIS_PLATFORM_WAYLAND_PRESENT
        WisWaylandExtension* wayland_extension = (WisWaylandExtension*)malloc(sizeof(WisWaylandExtension));
        if (!wayland_extension) {
            return platform; // Return empty platform on allocation failure
        }

        wisInitWaylandExtension(wayland_extension);
        platform.platform_extension = &wayland_extension->header;
        platform.extension_type     = SDL_PLATFORM_EXTENSION_WAYLAND;
#endif
    }
#endif

    return platform;
}

void DestroyPlatform(SDLPlatform* platform)
{
    switch (platform->extension_type) {
#if defined(SDL_PLATFORM_WIN32)
    case SDL_PLATFORM_EXTENSION_WIN32: {
        wisDestroyWin32Extension((WisWin32Extension*)platform->platform_extension);
        break;
    }
#elif defined(SDL_PLATFORM_LINUX)
    case SDL_PLATFORM_EXTENSION_X11: {
#ifdef WIS_PLATFORM_XLIB_PRESENT
        wisDestroyXlibExtension((WisXlibExtension*)platform->platform_extension);
        break;
#endif
    }
    case SDL_PLATFORM_EXTENSION_WAYLAND: {
#ifdef WIS_PLATFORM_WAYLAND_PRESENT
        wisDestroyWaylandExtension((WisWaylandExtension*)platform->platform_extension);
#endif
        break;
    }
#endif
    default:
        break;
    }
    free(platform->platform_extension);
}
