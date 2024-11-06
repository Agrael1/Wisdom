#pragma once
#include <wisdom/wisdom.hpp>

namespace Test {
class App
{
public:
    // input from platform window
    App(std::span<wis::FactoryExtension*> factory_exts);

    void SetSwapChain(wis::SwapChain swap, uint32_t width, uint32_t height);

public:
    void CreateResources();

    void Frame();
    void OnResize(uint32_t width, uint32_t height);
    void WaitForGPU();

public:
    const wis::Device& GetDevice() const
    {
        return device;
    }
    const wis::CommandQueue& GetQueue() const
    {
        return queue;
    }

private:
    void CreateRootSignature();
    void CreateRootSignature2();
    void DumpFrame(const char* name);
private:
    uint32_t width;
    uint32_t height;

    wis::DebugMessenger info;

    wis::Device device;
    wis::CommandQueue queue;
    wis::CommandList cmd_list;
    wis::CommandList cmd_list2;
    wis::SwapChain swap;

    std::span<const wis::Texture> back_buffers;
    std::array<wis::RenderTarget, 2> render_targets;
    std::array<wis::RenderTarget, 2> render_targets2;

    wis::Fence fence;
    uint64_t fence_value = 1;

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
