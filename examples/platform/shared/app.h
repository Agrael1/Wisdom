#pragma once
#include <wis_helper.h>
#include <wis_swapchain.h>

namespace ex {
class App
{
public:
    App(wis::FactoryExtension* platform_ext);
    ~App();

public:
    void SetSwapChain(wis::SwapChain swap, uint32_t width, uint32_t height);

public:
    void CreateResources();
    void Frame();
    void OnResize(uint32_t width, uint32_t height);

public:
    const wis::Device& GetDevice() const
    {
        return setup.device;
    }
    const wis::CommandQueue& GetQueue() const
    {
        return setup.queue;
    }

private:
    void CreateRootSignature();
    void CreateRootSignature2();
private:
    uint32_t width;
    uint32_t height;

    ex::ExampleSetup setup;
    ex::FramedCommandList cmd_list;
    ex::Swapchain swap;

    wis::Buffer vertex_buffer;
    wis::Buffer ubuf_2;
    wis::VertexBufferBinding vertex_binding;
    wis::ResourceAllocator allocator;

    wis::Shader vertex_shader;
    wis::Shader vertex_shader2;
    wis::Shader pixel_shader;
    wis::Shader pixel_shader2;

    wis::RootSignature root;
    wis::RootSignature root2;
    wis::PipelineState pipeline;
    wis::PipelineState pipeline2;

    wis::Texture texture;

    wis::Texture texture_inter;
    wis::RenderTarget rt_inter;
    wis::ShaderResource srv_inter[2];

    float rotation = 0.0f;
    float rotation2 = 0.0f;

    wis::DescriptorBuffer desc_buffer;
    wis::DescriptorBuffer sampler_buffer;

    wis::Sampler sampler;
    wis::ShaderResource srv;
};
} // namespace Test
