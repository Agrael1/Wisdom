#pragma once
#include <SDL3/SDL.h>
#include <memory>

#ifdef WISDOM_FORCE_VULKAN
import wisdom.fvk;
#else
import wisdom;
#endif

namespace ex {
static inline constexpr uint32_t swap_buffer_count = 2;
static inline constexpr uint32_t flight_frames = 2;
static inline constexpr wis::DataFormat swapchain_format = wis::DataFormat::RGBA8Unorm; // supported by all APIs

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
    wis::SwapChain CreateSwapchain(wis::Result& result, ex::PlatformExtension& external_ext, wis::Device& device, wis::CommandQueue& queue, wis::DataFormat fmt = ex::swapchain_format, bool stereo = false);

    void PostQuit();
    std::pair<int, int> PixelSize() const noexcept
    {
        int w = 0, h = 0;
        SDL_GetWindowSizeInPixels(window, &w, &h);
        return { w, h };
    }

private:
    SDL_Window* window;
    ex::PlatformExtension _platform;
};
} // namespace ex
