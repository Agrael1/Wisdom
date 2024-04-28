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
    uint32_t width;
    uint32_t height;

    wis::DebugMessenger info;

    wis::Device device;
    wis::CommandQueue queue;
    wis::CommandList cmd_list;
    wis::SwapChain swap;

    std::span<const wis::Texture> back_buffers;
    std::array<wis::RenderTarget, 2> render_targets;

    wis::Fence fence;
    uint64_t fence_value = 1;

    wis::Buffer vertex_buffer;
    wis::Buffer ubuf_2;
    wis::Buffer cbuf;
    wis::VertexBufferBinding vertex_binding;
    wis::ResourceAllocator allocator;

    wis::Shader vertex_shader;
    wis::Shader pixel_shader;

    wis::RootSignature root;
    wis::PipelineState pipeline;

    wis::Texture texture;

    float rotation = 0.0f;

    wis::DescriptorBuffer desc_buffer;
    wis::DescriptorBuffer sampler_buffer;

    wis::Sampler sampler;
    wis::ShaderResource srv;
};
} // namespace Test