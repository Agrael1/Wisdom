#pragma once
#include <SDL3/SDL.h>
#include <wisdom/wisdom.h>

typedef enum SDLPlatformExtension {
    SDL_PLATFORM_EXTENSION_NONE    = 0,
    SDL_PLATFORM_EXTENSION_WIN32   = 1,
    SDL_PLATFORM_EXTENSION_X11     = 2,
    SDL_PLATFORM_EXTENSION_WAYLAND = 3,
} SDLPlatformExtension;

typedef struct SDLPlatform {
    SDLPlatformExtension        extension_type;
    WisInstanceExtensionHeader* platform_extension;
} SDLPlatform;

SDLPlatform CreatePlatform();
WisSurface  CreateSurface(const SDLPlatform* platform, SDL_Window* window);
void        DestroyPlatform(SDLPlatform* platform);