#include "window.h"
#include <wisdom/wisdom_platform.h>

ex::WindowEvent ex::Window::PollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (SDL_EventType(e.type)) {
        case SDL_EVENT_QUIT:
            return WindowEvent::Quit;
        case SDL_EVENT_WINDOW_RESIZED:
            return WindowEvent::Resize;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                _mouse.OnLeftPressed(e.button.x, e.button.y);
            } else if (e.button.button == SDL_BUTTON_RIGHT) {
                _mouse.OnRightPressed(e.button.x, e.button.y);
            } else if (e.button.button == SDL_BUTTON_MIDDLE) {
                _mouse.OnWheelDown(e.button.x, e.button.y);
            }
            continue;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                _mouse.OnLeftReleased(e.button.x, e.button.y);
            } else if (e.button.button == SDL_BUTTON_RIGHT) {
                _mouse.OnRightReleased(e.button.x, e.button.y);
            } else if (e.button.button == SDL_BUTTON_MIDDLE) {
                _mouse.OnWheelUp(e.button.x, e.button.y);
            }
            continue;
        case SDL_EVENT_MOUSE_MOTION:
            _mouse.OnRawDelta(e.motion.xrel, -e.motion.yrel);
            continue;
        case SDL_EVENT_KEY_DOWN:
            _keyboard.OnKeyPressed(e.key.key);
            continue;
        case SDL_EVENT_KEY_UP:
            _keyboard.OnKeyReleased(e.key.key);
            continue;
        }
    }
    return WindowEvent::NoEvent;
}

wis::SwapChain ex::Window::CreateSwapchain(wis::Result& result, ex::ExampleSetup& setup, wis::DataFormat fmt, bool stereo)
{
    using enum PlatformExtension::Selector;
    if (_platform.current == None) {
        throw ex::Exception("Platform is not selected");
    }

    auto [width, height] = PixelSize();
    wis::SwapchainDesc desc{
        .size = { uint32_t(width), uint32_t(height) },
        .format = fmt,
        .buffer_count = ex::swap_buffer_count,
        .stereo = stereo,
        .vsync = true,
        .tearing = false,
    };

    switch (_platform.current) {
#if defined(SDL_PLATFORM_WIN32)
    case Windows: {
        HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
        if (hwnd) {
            return static_cast<wis::platform::WindowsExtension*>(_platform.get())
                    ->CreateSwapchain(result, setup.device, setup.queue, &desc, hwnd);
        }
    } break;
#elif defined(SDL_PLATFORM_LINUX)
    case X11: {
        Display* xdisplay = (Display*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        ::Window xwindow = (::Window)SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        if (xdisplay && xwindow) {
            return Unwrap(static_cast<wis::platform::X11Extension*>(_platform.get())
                                  ->CreateSwapchain(result, setup.device, setup.queue, &desc, xdisplay, xwindow));
        }
    } break;
    case Wayland: {
        struct wl_display* display = (struct wl_display*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        struct wl_surface* surface = (struct wl_surface*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
        if (display && surface) {
            return Unwrap(static_cast<wis::platform::WaylandExtension*>(_platform.get())
                                  ->CreateSwapchain(result, setup.device, setup.queue, &desc, display, surface));
        }
    } break;
#endif
    }
    throw ex::Exception("Failed to create swapchain");
}

void ex::Window::PostQuit()
{
    SDL_Event event;
    event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&event);
}

ex::PlatformExtension::PlatformExtension()
{
    current = Selector::None;
    platform = {};
    const char* platform_name = SDL_GetCurrentVideoDriver();
#if defined(SDL_PLATFORM_WIN32)
    platform = std::make_unique<wis::platform::WindowsExtension>();
    current = Selector::Windows;
#elif defined(SDL_PLATFORM_LINUX)
    // if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
    platform = std::make_unique<wis::platform::X11Extension>();
    current = Selector::X11;
    //} else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
    //    platform = std::make_unique<wis::platform::WaylandExtension>();
    //    current = Selector::Wayland;
    //}
#endif
}
