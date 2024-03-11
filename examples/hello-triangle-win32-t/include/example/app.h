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
    void CreateResources();

    void Frame();
    void ProcessEvent(Event e);
    void OnResize(uint32_t width, uint32_t height);
    void WaitForGPU();

private:
    Window wnd;

    wis::DebugMessenger info;

    wis::Device device;
    wis::CommandQueue queue;
    wis::CommandList cmd_list;
    wis::SwapChain swap;

    wis::Fence fence;
    uint64_t fence_value = 1;

    wis::Buffer vertex_buffer;
    wis::ResourceAllocator allocator;

    wis::Shader vertex_shader;
    wis::Shader pixel_shader;
};
} // namespace Test
