#pragma once
#include <wisdom/wisdom.hpp>
#include "window.h"

class App
{
public:
    App(uint32_t width, uint32_t height);

public:
    int Start();
    void CreateResources();

    void Frame();
    void DumpFrame(const char* name);
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

    wis::Texture out_texture;
    wis::RenderTarget render_target;


    wis::Fence fence;
    uint64_t fence_value = 1;
    wis::ResourceAllocator allocator;

    wis::Shader vertex_shader;
    wis::Shader pixel_shader;
    wis::Shader pixel_shader_tetra;

    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::PipelineState pipeline2;
    wis::PipelineState* pipeline_c;


    wis::Texture texture;
    wis::Texture lut;

    wis::DescriptorBuffer desc_buffer;
    wis::DescriptorBuffer sampler_buffer;

    wis::Sampler sampler;
    wis::Sampler sampler_lut_linear;
    wis::ShaderResource srv;
    wis::ShaderResource srv_lut;

    wis::SwapChain swap;

    std::span<const wis::Texture> back_buffers;
    std::array<wis::RenderTarget, 2> render_targets;
};