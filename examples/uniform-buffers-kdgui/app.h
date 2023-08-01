#pragma once
#include <wisdom/wisdom.h>
#include "window.h"
#include <optional>
#include <glm/mat4x4.hpp>

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

    glm::mat4 cube_transform{ 1.0f };
    glm::mat4 projection{ 1.0f };
    glm::mat4 view{ 1.0f };

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

    struct SceneConstantBuffer {
        glm::mat4 model_view_projection{ 1.0f };
        std::byte padding[256 - sizeof(glm::mat4)]; // Padding so the constant buffer is 256-byte aligned.
    } buffer{};
    std::span<std::byte> mapped_buffer;
};
} // namespace Test
