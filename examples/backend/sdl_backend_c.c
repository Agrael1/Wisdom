#include "sdl_backend_c.h"

#include <wisdom/wisdom_platform.h>

#include <stdlib.h>

#ifdef SDL_PLATFORM_WIN32
#    include <Windows.h>
#endif

SDLPlatform CreatePlatform()
{
    SDLPlatform platform = {0};
#if defined(SDL_PLATFORM_WIN32)
    WisWin32Extension* win32_extension = (WisWin32Extension*)malloc(sizeof(WisWin32Extension));
    if (!win32_extension) {
        return platform;
    }

    wisInitWin32Extension(win32_extension);
    platform.platform_extension = &win32_extension->header;
    platform.extension_type = SDL_PLATFORM_EXTENSION_WIN32;
#elif defined(SDL_PLATFORM_LINUX)
    const char* driver = SDL_GetCurrentVideoDriver();
    if (driver && SDL_strcmp(driver, "x11") == 0) {
        WisXlibExtension* xlib_extension = (WisXlibExtension*)malloc(sizeof(WisXlibExtension));
        if (!xlib_extension) {
            return platform;
        }

        wisInitXlibExtension(xlib_extension);
        platform.platform_extension = &xlib_extension->header;
        platform.extension_type = SDL_PLATFORM_EXTENSION_X11;
    } else if (driver && SDL_strcmp(driver, "wayland") == 0) {
        WisWaylandExtension* wayland_extension = (WisWaylandExtension*)malloc(sizeof(WisWaylandExtension));
        if (!wayland_extension) {
            return platform;
        }

        wisInitWaylandExtension(wayland_extension);
        platform.platform_extension = &wayland_extension->header;
        platform.extension_type = SDL_PLATFORM_EXTENSION_WAYLAND;
    }
#endif

    return platform;
}

WisSurface CreateSurface(const SDLPlatform* platform, SDL_Window* window)
{
    switch (platform->extension_type) {
#if defined(SDL_PLATFORM_WIN32)
    case SDL_PLATFORM_EXTENSION_WIN32: {
        WisWin32Extension* win32_extension = (WisWin32Extension*)platform->platform_extension;
        HWND hwnd = (HWND
        )SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
        if (hwnd) {
            WisWin32WindowDesc desc = {
                .hinstance = GetModuleHandle(NULL),
                .hwnd = hwnd,
            };
            WisSurface surface = {0};
            wisWin32ExtensionCreateSurface(win32_extension, &desc, &surface);
            return surface;
        }
        break;
    }
#elif defined(SDL_PLATFORM_LINUX)
    case SDL_PLATFORM_EXTENSION_X11: {
        void* xdisplay = (void*)
            SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        uint64_t xwindow = (uint64_t
        )SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        if (xdisplay && xwindow) {
            WisXlibWindowDesc desc = {
                .display = xdisplay,
                .window = xwindow,
            };
            WisSurface surface = {0};
            wisXlibExtensionCreateSurface((WisXlibExtension*)platform->platform_extension, &desc, &surface);
            return surface;
        }
        break;
    }
    case SDL_PLATFORM_EXTENSION_WAYLAND: {
        WisWaylandExtension* wayland_extension = (WisWaylandExtension*)platform->platform_extension;
        struct wl_display* display = (struct wl_display*)
            SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        struct wl_surface* surface = (struct wl_surface*)
            SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
        if (display && surface) {
            WisWaylandWindowDesc desc = {
                .display = display,
                .surface = surface,
            };
            WisSurface output = {0};
            wisWaylandExtensionCreateSurface(wayland_extension, &desc, &output);
            return output;
        }
        break;
    }
#endif
    default:
        break;
    }

    return (WisSurface){0};
}

void DestroyPlatform(SDLPlatform* platform)
{
    switch (platform->extension_type) {
#if defined(SDL_PLATFORM_WIN32)
    case SDL_PLATFORM_EXTENSION_WIN32:
        wisDestroyWin32Extension((WisWin32Extension*)platform->platform_extension);
        break;
#elif defined(SDL_PLATFORM_LINUX)
    case SDL_PLATFORM_EXTENSION_X11:
        wisDestroyXlibExtension((WisXlibExtension*)platform->platform_extension);
        break;
    case SDL_PLATFORM_EXTENSION_WAYLAND:
        wisDestroyWaylandExtension((WisWaylandExtension*)platform->platform_extension);
        break;
#endif
    default:
        break;
    }

    free(platform->platform_extension);
    platform->platform_extension = NULL;
    platform->extension_type = SDL_PLATFORM_EXTENSION_NONE;
}
