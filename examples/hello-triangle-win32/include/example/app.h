#pragma once
#include <example/window.h>
#include <wisdom/wisdom.h>

namespace Test {
class App
{
public:
    App(uint32_t width, uint32_t height);
    ~App()
    {
        WaitForGPU();
    }

public:
    int Start();
    void Frame();
    void ProcessEvent(Event e);
    void OnResize(uint32_t width, uint32_t height);
    void WaitForGPU();

private:
    Window wnd;

    std::optional<wis::Factory> factory;

    wis::Device device;
    wis::CommandQueue queue;
    wis::SwapChain swap;

    wis::CommandList context;
    wis::Fence fence;
    wis::ResourceAllocator allocator;

    wis::Shader vs;
    wis::Shader ps;

    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::VertexBufferView vb;
    wis::RenderTarget rtvs[2];
    wis::RenderTarget rtvs2[2];

    wis::Buffer vertex_buffer;
    wis::RenderPass render_pass;
    uint64_t fence_value = 1;
};
} // namespace Test
