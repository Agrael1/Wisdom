#pragma once
#include <example/window.h>
#include <wisdom/wisdom.hpp>

namespace Test {
class App
{
public:
    App(uint32_t width, uint32_t height);

public:
    int Start();
    void Frame();
    void ProcessEvent(Event e);
    void OnResize(uint32_t width, uint32_t height);
    void WaitForGPU();

private:
    Window wnd;

    wis::Factory factory;

    wis::Device device;
    wis::CommandQueue queue;
    wis::SwapChain swap;

    wis::Fence fence;
    uint64_t fence_value = 1;
};
} // namespace Test
