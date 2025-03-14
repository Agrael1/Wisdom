#pragma once
#include <SDL3/SDL.h>
#include "mouse.h"
#include "keyboard.h"
#include "window_event.h"
#include "wis_helper.h"

namespace ex {
class Instance
{
public:
    Instance()
    {
        SDL_Init(SDL_INIT_VIDEO);
    }
    ~Instance()
    {
        SDL_Quit();
    }
};

class PlatformExtension
{
public:
    enum class Selector {
        None,
        Windows,
        X11,
        Wayland
    };

public:
    PlatformExtension();

public:
    wis::FactoryExtension* get() noexcept
    {
        return platform.get();
    }

public:
    Selector current = Selector::None;
    std::unique_ptr<wis::FactoryExtension> platform;
};

class Window
{
public:
    Window(const char* title, int width, int height)
    {
        window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    }
    Window(const char* title, int x, int y, int width, int height, SDL_WindowFlags flags)
    {
        SDL_PropertiesID props = SDL_CreateProperties();
        if (title && *title) {
            SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title);
        }
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, x);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, flags);
        window = SDL_CreateWindowWithProperties(props);
        SDL_DestroyProperties(props);
    }
    ~Window()
    {
        SDL_DestroyWindow(window);
    }
    SDL_Window* GetWindow() const
    {
        return window;
    }

public:
    wis::FactoryExtension* GetPlatformExtension()
    {
        return _platform.get();
    }
    wis::SwapChain CreateSwapchain(wis::Result& result, ex::ExampleSetup& setup, wis::DataFormat fmt = ex::swapchain_format, bool stereo = false);
    wis::SwapChain CreateSwapchain(wis::Result& result, ex::PlatformExtension& external_ext, ex::ExampleSetup& setup, wis::DataFormat fmt = ex::swapchain_format, bool stereo = false);

    void PostQuit();
    ex::WindowEvent PollEvents();
    std::pair<int, int> PixelSize() const noexcept
    {
        int w = 0, h = 0;
        SDL_GetWindowSizeInPixels(window, &w, &h);
        return { w, h };
    }

public:
    Mouse& GetMouse()
    {
        return _mouse;
    }
    Keyboard& GetKeyboard()
    {
        return _keyboard;
    }

private:
    SDL_Window* window;
    Mouse _mouse;
    Keyboard _keyboard;
    ex::PlatformExtension _platform;
};
} // namespace ex
