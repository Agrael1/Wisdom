#pragma once
#include <wisdom/dx12/xdx12_factory.h>
#include "window.h"
#include <optional>

namespace Test {
class App
{
public:
    App(uint32_t width, uint32_t height);
    ~App();

    int Start();

private:
    void Frame();
    void WaitForGPU();
    void OnResize(uint32_t width, uint32_t height);

    XApp app;
    Window wnd;

    wis::DX12Factory factory;

    //wis::Device device;
    //wis::CommandQueue queue;
    //wis::SwapChain swap;
    //
    //wis::DescriptorHeap uniforms;
    //
    //wis::CommandList context;
    //wis::Fence fence;
    //wis::ResourceAllocator allocator;
    //
    //wis::Shader vs;
    //wis::Shader ps;
    //
    //wis::RootSignature root;
    //wis::PipelineState pipeline;
    //wis::VertexBufferView vb;
    //wis::RenderTarget rtvs[2];
    //wis::RenderTarget rtvs2[2];
    //
    //wis::Buffer vertex_buffer;
    //wis::RenderPass render_pass;
    //uint64_t fence_value = 1;
};
} // namespace Test
