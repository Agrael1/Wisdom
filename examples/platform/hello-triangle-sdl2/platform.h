#pragma once
#include <SDL.h>
#include <wisdom/wisdom.hpp>

struct SwapchainCreateOptions {
    SDL_Window& window;
    const wis::Device& device;
    const wis::CommandQueue& queue;
};

wis::ResultValue<wis::SwapChain> CreateSwapchain(const SwapchainCreateOptions& options);
