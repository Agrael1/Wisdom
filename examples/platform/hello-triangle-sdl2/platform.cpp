#include "platform.h"
#include "util.h"

#include <SDL_syswm.h>
#ifdef WISDOM_LINUX
#include <wisdom/platform/linux.h>
#ifndef NO_X11
#include <X11/Xlib-xcb.h>
#endif // !NO_X11
#elif WISDOM_MAC
#include <wisdom/platform/mac.h>
#else
#include <wisdom/platform/win32.h>
#endif

wis::ResultValue<wis::SwapChain> CreateSwapchain(const SwapchainCreateOptions& options)
{
    wis::SwapchainDesc desc{
        .size = SDLWindowSize(options.window),
        .format = wis::DataFormat::BGRA8Unorm,
        .buffer_count = 2,
        .stereo = true,
        .vsync = true,
    };

    SDL_SysWMinfo windowInfo;
    SDL_VERSION(&windowInfo.version);
    if (SDL_GetWindowWMInfo(&options.window, &windowInfo) == 0u) {
        return wis::Result{ .error = SDL_GetError() };
    }

#ifdef WISDOM_LINUX
    if (windowInfo.subsystem == SDL_SYSWM_X11) {
#ifndef NO_X11
        return wis::CreateSwapchainXcb(
                options.device,
                options.queue,
                &desc,
                XGetXCBConnection(windowInfo.info.x11.display),
                uint32_t(windowInfo.info.x11.window));
#else
        return wis::Result{ .error = "X11 not supported, please install libx11-xcb-dev" };
#endif // !NO_X11

    } else if (windowInfo.subsystem == SDL_SYSWM_WAYLAND) {
#ifdef SDL_VIDEO_DRIVER_WAYLAND
        return wis::CreateSwapchainWayland(
                options.device,
                options.queue,
                &desc,
                windowInfo.info.wl.display,
                windowInfo.info.wl.surface);
#else
        return wis::Result{ .error = "Wayland is not supported" };
#endif // SDL_VIDEO_DRIVER_WAYLAND

    } else {
        return wis::Result{ .error = "unexpected WM subsystem on linux" };
    }
#elif WISDOM_MAC
#error "mac not implemented"
#else
    return wis::CreateSwapchainWin32(
            options.device,
            options.queue,
            &desc,
            windowInfo.info.win.window);
#endif
}
