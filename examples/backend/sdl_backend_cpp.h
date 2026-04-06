#pragma once

#include "sdl_backend_c.h"

#include <wisdom/wisdom.hpp>

#include <cstring>

class SDLPlatformCpp
{
public:
    SDLPlatformCpp() = default;
    ~SDLPlatformCpp() { Reset(); }

    SDLPlatformCpp(const SDLPlatformCpp&) = delete;
    SDLPlatformCpp& operator=(const SDLPlatformCpp&) = delete;

    SDLPlatformCpp(SDLPlatformCpp&& other) noexcept
        : platform(other.platform)
    {
        other.platform = {};
    }

    SDLPlatformCpp& operator=(SDLPlatformCpp&& other) noexcept
    {
        if (this != &other) {
            Reset();
            platform = other.platform;
            other.platform = {};
        }
        return *this;
    }

    bool Init()
    {
        platform = CreatePlatform();
        return platform.platform_extension != nullptr;
    }

    wis::Surface CreateWindowSurface(SDL_Window* window) const
    {
        WisSurface surface = CreateSurface(&platform, window);
        wis::Surface cpp_surface;
        std::memcpy(cpp_surface.GetStorage(), &surface, sizeof(surface));
        return cpp_surface;
    }

    wis::InstanceExtensionHeader* Extension() const
    {
        return reinterpret_cast<wis::InstanceExtensionHeader*>(platform.platform_extension);
    }

private:
    void Reset()
    {
        if (platform.platform_extension != nullptr) {
            DestroyPlatform(&platform);
        }
    }

private:
    SDLPlatform platform{};
};
