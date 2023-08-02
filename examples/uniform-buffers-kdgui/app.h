#pragma once
#include <wisdom/wisdom.h>
#include "window.h"
#include <optional>
#include "graphics.h"

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
    void UpdateConstantBuffer();

    XApp app;
    Window wnd;
    Graphics gfx;

    glm::mat4 cube_transform{ 1.0f };
    static constexpr auto near_plane = 0.1f;
    static constexpr auto far_plane = 100.0f;
    static constexpr auto fov_degrees = 90.0f;

    wis::Buffer constant_buffer;
    wis::DescriptorSet constants_set;

    wis::Shader vs;
    wis::Shader ps;

    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::VertexBufferView vb;

    wis::Buffer vertex_buffer;
    wis::Buffer index_buffer;


    struct SceneConstantBuffer {
        glm::mat4 model_view_projection{ 1.0f };
        std::byte padding[256 - sizeof(glm::mat4)]; // Padding so the constant buffer is 256-byte aligned.
    } buffer{};
    std::span<std::byte> mapped_buffer;
};
} // namespace Test
