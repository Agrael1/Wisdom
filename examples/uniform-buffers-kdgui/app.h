#pragma once
#include <wisdom/wisdom.h>
#include "window.h"
#include <optional>
#include <glm/vec4.hpp>

namespace Test {
class App
{
public:
    App(uint32_t width, uint32_t height);
    ~App();

public:
    int Start();

private:
    void Frame();
    void WaitForGPU();

private:
    XApp app;
    Window wnd;

    std::optional<wis::Factory> factory;

    wis::Device device;
    wis::CommandQueue queue;
    wis::SwapChain swap;

    wis::CommandList context;
    wis::Fence fence;
    wis::ResourceAllocator allocator;

    wis::DescriptorHeap constants_heap;
    wis::Buffer constant_buffer;
    wis::DescriptorSet constants_set;

    wis::Shader vs;
    wis::Shader ps;

    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::VertexBufferView vb;
    wis::RenderTargetView rtvs[2];
    wis::RenderTargetView rtvs2[2];

    wis::Buffer vertex_buffer;
    wis::RenderPass render_pass;
    uint64_t fence_value = 1;

private:
    struct SceneConstantBuffer {
        glm::vec4 offset;
        float padding[60]; // Padding so the constant buffer is 256-byte aligned.
    };
};
} // namespace Test
