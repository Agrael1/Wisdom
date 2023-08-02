#include "app.h"
#include "cube.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

template<class T>
std::span<std::byte> RawView(T& data)
{
    return { (std::byte*)&data, sizeof(T) };
}

Test::App::App(uint32_t width, uint32_t height)
    : wnd(app.createWindow(width, height)), gfx(width, height, wnd.GetSurfaceOptions())
{
    vs = gfx.device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.cb.transform.vs"), wis::ShaderType::vertex);
    ps = gfx.device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.ps"), wis::ShaderType::pixel);

    constant_buffer = gfx.allocator.CreateConstantBuffer(sizeof(SceneConstantBuffer));

    constexpr glm::vec3 cube_position{ 0.0f, 0.0f, 5.0f };
    constexpr glm::vec3 camera_position{ 0.0f, 0.2f, 0.0f };
    gfx.view = glm::lookAtLH(camera_position, cube_position, glm::vec3(0.0f, 1.0f, 0.0f));
    cube_transform = glm::translate(glm::mat4{ 1.0f }, cube_position);

    // upload to gpu
    mapped_buffer = constant_buffer.MapMemory();

    std::array bindings{
        wis::BindingDescriptor{
                .binding = 0,
                .stages = wis::ShaderStage::vertex,
                .type = wis::BindingType::CBV,
        }
    };
    wis::DescriptorSetLayout constants_layout = gfx.device.CreateDescriptorSetLayout(bindings);
    constants_set = gfx.constants_heap.AllocateDescriptorSet(constants_layout);
    gfx.device.CreateConstantBufferView(constant_buffer, sizeof(SceneConstantBuffer), constants_set, constants_layout, 0);

    root = gfx.device.CreateRootSignature({ &constants_layout, 1u }); // empty

    OnResize(width, height);

    vertex_buffer = gfx.allocator.CreatePersistentBuffer(sizeof(cube_vertices_indexed), wis::BufferFlags::VertexBuffer);
    index_buffer = gfx.allocator.CreatePersistentBuffer(sizeof(cube_indices), wis::BufferFlags::IndexBuffer);

    auto upl_vbuf = gfx.allocator.CreateUploadBuffer(sizeof(cube_vertices_indexed));
    auto upl_ibuf = gfx.allocator.CreateUploadBuffer(sizeof(cube_indices));
    upl_vbuf.UpdateSubresource(RawView(cube_vertices_indexed));
    upl_ibuf.UpdateSubresource(RawView(cube_indices));

    gfx.context.SetPipeline(pipeline);
    gfx.context.Reset();
    gfx.context.CopyBuffer(upl_vbuf, vertex_buffer, sizeof(cube_vertices_indexed));
    gfx.context.CopyBuffer(upl_ibuf, index_buffer, sizeof(cube_indices));
    gfx.context.BufferBarrier({ .access_before = wis::ResourceAccess::CopyDest,
                                .access_after = wis::ResourceAccess::VertexBuffer },
                              vertex_buffer);
    gfx.context.BufferBarrier({ .access_before = wis::ResourceAccess::CopyDest,
                                .access_after = wis::ResourceAccess::IndexBuffer },
                              index_buffer);
    gfx.context.Close();

    gfx.queue.ExecuteCommandList(gfx.context);
    gfx.WaitForGPU();

    gfx.context.Reset();

    gfx.context.Close();

    gfx.queue.ExecuteCommandList(gfx.context);
    gfx.WaitForGPU();
    vb = vertex_buffer.GetVertexBufferView(sizeof(Vertex));
}

Test::App::~App()
{
    gfx.WaitForGPU();
}

int Test::App::Start()
{
    while (true) {
        app.ProcessEvents();
        if (!wnd.visible())
            return 0;
        // Process Events
        if (wnd.resized())
            OnResize(wnd.width(), wnd.height());
        Frame();
    }
}

void Test::App::UpdateConstantBuffer()
{
    // glm is backwards... projection-view-model
    buffer.model_view_projection = gfx.projection * gfx.view * cube_transform;
    auto* mapped = reinterpret_cast<glm::mat4*>(mapped_buffer.data());
    *mapped = glm::transpose(buffer.model_view_projection);
}

void Test::App::Frame()
{
    gfx.context.Reset();

    auto back = gfx.swap.GetBackBuffer();

    cube_transform = glm::rotate(cube_transform, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 1.0f));
    UpdateConstantBuffer();

    gfx.context.TextureBarrier({
                                       .state_before = wis::TextureState::Undefined,
                                       .state_after = wis::TextureState::DepthWrite,
                                       .access_before = wis::ResourceAccess::Common,
                                       .access_after = wis::ResourceAccess::DepthWrite,
                               },
                               gfx.depth_buffers[0]);
    gfx.context.TextureBarrier({
                                       .state_before = wis::TextureState::Undefined,
                                       .state_after = wis::TextureState::DepthWrite,
                                       .access_before = wis::ResourceAccess::Common,
                                       .access_after = wis::ResourceAccess::DepthWrite,
                               },
                               gfx.depth_buffers[1]);

    gfx.context.TextureBarrier({ .state_before = wis::TextureState::Present,
                                 .state_after = wis::TextureState::RenderTarget,
                                 .access_before = wis::ResourceAccess::Common,
                                 .access_after = wis::ResourceAccess::RenderTarget },
                               back);

    constexpr wis::ColorClear color{ 0.0f, 0.2f, 0.4f, 1.0f };
    constexpr wis::ColorClear color2{ 1.0f, 0.2f, 0.4f, 1.0f };
    std::array rtvsx{
        std::pair{ gfx.rtvs[gfx.swap.GetNextIndex()], color },
        std::pair{ gfx.rtvs2[gfx.swap.GetNextIndex()], color2 }
    };

    gfx.context.SetGraphicsDescriptorSet(root, 0, constants_set);
    gfx.context.RSSetViewport({ float(wnd.width()), float(wnd.height()) });
    gfx.context.RSSetScissorRect({ long(wnd.width()), long(wnd.height()) });
    gfx.context.IASetPrimitiveTopology(wis::PrimitiveTopology::trianglelist);
    gfx.context.IASetVertexBuffers({ &vb, 1 });
    gfx.context.IASetIndexBuffer(index_buffer, sizeof(cube_indices), wis::IndexType::uint16);

    gfx.context.BeginRenderPass(gfx.render_pass, { rtvsx.data(), static_cast<unsigned int>(gfx.swap.StereoSupported()) + 1u }, std::pair{ gfx.dsv[gfx.swap.GetNextIndex()], wis::DepthClear{ 1.0f } });
    gfx.context.DrawIndexedInstanced(cube_indices.size());
    gfx.context.EndRenderPass();

    gfx.context.TextureBarrier({
                                       .state_before = wis::TextureState::RenderTarget,
                                       .state_after = wis::TextureState::Present,
                                       .access_before = wis::ResourceAccess::RenderTarget,
                                       .access_after = wis::ResourceAccess::Common,
                               },
                               back);
    gfx.context.Close();
    gfx.queue.ExecuteCommandList(gfx.context);

    gfx.swap.Present();
    gfx.WaitForGPU();
}

void Test::App::OnResize(uint32_t width, uint32_t height)
{
    gfx.OnResize(width, height);

    // update projection
    gfx.projection = glm::perspectiveFovLH_ZO(glm::radians(fov_degrees),
                                              (float)width, (float)height,
                                              near_plane, far_plane);
    UpdateConstantBuffer();

    // needs to be recreated for vulkan for now
    static constexpr std::array ia{
        wis::InputLayoutDesc{ 0, "POSITION", 0, wis::DataFormat::r32g32b32_float, 0, 0, wis::InputClassification::vertex, 0 },
        wis::InputLayoutDesc{ 1, "NORMAL", 0, wis::DataFormat::r32g32b32_float, 0, 12, wis::InputClassification::vertex, 0 },
        wis::InputLayoutDesc{ 2, "COLOR", 0, wis::DataFormat::r32g32b32a32_float, 0, 24, wis::InputClassification::vertex, 0 }
    };

    wis::GraphicsPipelineDesc desc{ root };
    desc.SetVS(vs);
    desc.SetPS(ps);
    desc.SetRenderPass(gfx.render_pass);
    pipeline = gfx.device.CreateGraphicsPipeline(desc, ia);
    gfx.context.SetPipeline(pipeline);
}
