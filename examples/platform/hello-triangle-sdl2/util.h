#pragma once
#include <SDL.h>
#include <wisdom/wisdom.hpp>

inline wis::Size2D SDLWindowSize(SDL_Window& window) noexcept
{
    int width;
    int height;
    SDL_GetWindowSize(&window, &width, &height);
    return { uint32_t(width), uint32_t(height) };
}
