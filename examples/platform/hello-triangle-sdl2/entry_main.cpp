#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstddef>
#include <cstdio>
#include "app.h"
#include "platform.h"
#include "util.h"

constexpr size_t screen_width = 800;
constexpr size_t screen_height = 600;

void PrintSDLError(const char* errstring)
{
    fprintf(stderr, "%s: %s\n", errstring, SDL_GetError()); // NOLINT
}

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        PrintSDLError("SDL could not be initialized");
        return -1;
    }

#if defined WISDOM_LINUX && SDL_VERSION_ATLEAST(2, 0, 8)
    if (SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0") == 0u) {
        PrintSDLError("SDL can not disable compositor bypass");
        return -1;
    }
#endif

    SDL_Window* window = SDL_CreateWindow("Wisdom SDL2 Example",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        PrintSDLError("could not create SDL window");
        SDL_Quit();
        return -1;
    }

    Test::App app;

    {
        auto [swapchainResult, swapchain] = CreateSwapchain({
                .window = *window,
                .device = app.GetDevice(),
                .queue = app.GetQueue(),
        });

        if (swapchainResult.status != wis::Status::Ok) {
            fprintf(stderr, "failed to create swapchain: %s\n", swapchainResult.error);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }

        auto size = SDLWindowSize(*window);
        app.SetSwapChain(std::move(swapchain), size.width, size.height);
        app.CreateResources();
    }

    bool quit = false;
    while (!quit) {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                app.OnResize(uint32_t(e.window.data1), uint32_t(e.window.data2));
            }
            break;
        default:
            break;
        }

        app.Frame();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
