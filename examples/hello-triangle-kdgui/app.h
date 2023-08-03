#pragma once
#include <wisdom/wisdom.h>
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

    std::optional<wis::Factory> factory;

    wis::Device device;
    wis::CommandQueue queue;
    wis::SwapChain swap;

    wis::DescriptorHeap uniforms;

    wis::CommandList context;
    wis::Fence fence;
    wis::ResourceAllocator allocator;

    wis::Shader vs;
    wis::Shader ps;

    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::VertexBufferView vb;
    wis::RenderTargetView rtvs[2];
    wis::RenderTargetView rtvs2[2];
    wis::Texture texture;
    wis::VKShaderResourceView srv;
    wis::Sampler sampler;

    wis::DescriptorHeap srv_heap;
    wis::DescriptorHeap sampler_heap;

    wis::DescriptorSet srv_set;
    wis::DescriptorSet sampler_set;

    wis::Buffer vertex_buffer;
    wis::RenderPass render_pass;
    uint64_t fence_value = 1;
};
} // namespace Test
