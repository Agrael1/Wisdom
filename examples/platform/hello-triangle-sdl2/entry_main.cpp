#include <SDL.h>
#include <cstddef>
#include <cstdio>

constexpr size_t screen_width = 800;
constexpr size_t screen_height = 600;

void sdl_printerr(const char* errstring)
{
    fprintf(stderr, "%s: %s\n", SDL_GetError());
}

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        sdl_printerr("SDL could not be initialized");
        return -1;
    }

#if defined WISDOM_LINUX && SDL_VERSION_ATLEAST(2, 0, 8)
    if (!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
        sdl_printerr("SDL can not disable compositor bypass");
        return -1;
    }
#endif

    SDL_Window* window = SDL_CreateWindow("Wisdom SDL2 Example",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        sdl_printerr("could not create SDL window");
        SDL_Quit();
        return -1;
    }

    bool quit = false;
    while (!quit) {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        if (e.type == SDL_QUIT) {
            quit = true;
        }

        // TODO: wisdom logic here
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
