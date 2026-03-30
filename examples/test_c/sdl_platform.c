#include "sdl_platform.h"

#include <wisdom/wisdom_platform.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef SDL_PLATFORM_WIN32
#    include <Windows.h>
#endif

SDLPlatform CreatePlatform()
{
    SDLPlatform platform = {0};
    const char* platform_name = SDL_GetCurrentVideoDriver();
#if defined(SDL_PLATFORM_WIN32)
    WisWin32Extension* win32_extension = (WisWin32Extension*)malloc(sizeof(WisWin32Extension));
    if (!win32_extension) {
        return platform; // Return empty platform on allocation failure
    }

    wisInitWin32Extension(win32_extension);
    platform.platform_extension = &win32_extension->header;
    platform.extension_type = SDL_PLATFORM_EXTENSION_WIN32;
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {

#    ifdef WIS_PLATFORM_XLIB_PRESENT
        WisXlibExtension* xlib_extension = (WisXlibExtension*)malloc(sizeof(WisXlibExtension));
        if (!xlib_extension) {
            return platform; // Return empty platform on allocation failure
        }
        wisInitXlibExtension(xlib_extension);
        platform.platform_extension = &xlib_extension->header;
        platform.extension_type = SDL_PLATFORM_EXTENSION_WIN32;
#    endif

    } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {

#    ifdef WIS_PLATFORM_WAYLAND_PRESENT
        WisWaylandExtension* wayland_extension = (WisWaylandExtension*)malloc(sizeof(WisWaylandExtension));
        if (!wayland_extension) {
            return platform; // Return empty platform on allocation failure
        }

        wisInitWaylandExtension(wayland_extension);
        platform.platform_extension = &wayland_extension->header;
        platform.extension_type = SDL_PLATFORM_EXTENSION_WAYLAND;
#    endif
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
        HWND hwnd = (HWND)
            SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
        if (hwnd) {
            WisWin32WindowDesc desc = {
                .hinstance = GetModuleHandle(NULL),
                .hwnd = hwnd,
            };
            WisSurface surface = {0};
            WisResult result = wisWin32ExtensionCreateSurface(win32_extension, &desc, &surface);
            printf("wisWin32ExtensionCreateSurface result: %d, error: %s\n", result.status, result.error);
            return surface;
        }
        break;
    }
#elif defined(SDL_PLATFORM_LINUX)
    case SDL_PLATFORM_EXTENSION_X11: {
#    ifdef WIS_PLATFORM_XLIB_PRESENT
        void* xdisplay = (void*)
            SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        uint64_t xwindow = (uint64_t)
            SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        if (xdisplay && xwindow) {
            WisXlibWindowDesc desc = {
                .display = xdisplay,
                .window = xwindow,
            };
            WisSurface surface = {0};
            WisResult result = wisXlibExtensionCreateSurface(
                (WisXlibExtension*)platform->platform_extension,
                &desc,
                &surface
            );
            printf("wisXlibExtensionCreateSurface result: %d, error: %s\n", result.status, result.error);
            return surface;
        }
        break;
#    endif
    }
    case SDL_PLATFORM_EXTENSION_WAYLAND: {
#    ifdef WIS_PLATFORM_WAYLAND_PRESENT
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
            WisSurface surface_out = {0};
            WisResult result = wisWaylandExtensionCreateSurface(wayland_extension, &desc, &surface_out);
            printf("wisWaylandExtensionCreateSurface result: %d, error: %s\n", result.status, result.error);
            return surface_out;
        }
#    endif
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
    case SDL_PLATFORM_EXTENSION_WIN32: {
        wisDestroyWin32Extension((WisWin32Extension*)platform->platform_extension);
        break;
    }
#elif defined(SDL_PLATFORM_LINUX)
    case SDL_PLATFORM_EXTENSION_X11: {
#    ifdef WIS_PLATFORM_XLIB_PRESENT
        wisDestroyXlibExtension((WisXlibExtension*)platform->platform_extension);
        break;
#    endif
    }
    case SDL_PLATFORM_EXTENSION_WAYLAND: {
#    ifdef WIS_PLATFORM_WAYLAND_PRESENT
        wisDestroyWaylandExtension((WisWaylandExtension*)platform->platform_extension);
#    endif
        break;
    }
#endif
    default:
        break;
    }
    free(platform->platform_extension);
}
