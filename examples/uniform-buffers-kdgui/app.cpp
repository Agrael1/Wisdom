#include "app.h"
#include "cube.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
    {
        std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

constexpr wis::ApplicationInfo app_info{
    .application_name = "example",
    .engine_name = "none",
};

// not WinRT Compatible
template<class ShaderTy>
auto LoadShader(std::filesystem::path p)
{
    if constexpr (ShaderTy::language == wis::ShaderLang::dxil)
        p += ".cso";
    else if constexpr (ShaderTy::language == wis::ShaderLang::spirv)
        p += ".spv";

    std::ifstream t{ p, std::ios::binary };
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    wis::shared_blob ret{ size };
    t.seekg(0);
    t.read(ret.data<char>(), size);
    return ret;
}

template<class T>
std::span<std::byte> RawView(T &data)
{
    return { (std::byte *)&data, sizeof(T) };
}

Test::App::App(uint32_t width, uint32_t height)
    : wnd(app.createWindow(width, height))
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    factory.emplace(app_info);

    for (auto &&a : factory->EnumerateAdapters(wis::AdapterPreference::Performance)) {
        auto desc = a.GetDesc();

        if (desc.IsSoftware())
            wis::lib_warn("Loading WARP adapter");

        std::cout << desc.to_string();

        if (device.Initialize(a)) {
            allocator = wis::ResourceAllocator{ device, a };
            break;
        }
    }

    queue = device.CreateCommandQueue();

    swap = device.CreateSwapchain(queue, wis::SwapchainOptions{ uint32_t(width), uint32_t(height), wis::SwapchainOptions::default_frames, wis::SwapchainOptions::default_format, true }, wnd.GetSurfaceOptions());

    fence = device.CreateFence();
    context = device.CreateCommandList(wis::QueueType::direct);

    vs = device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.cb.transform.vs"), wis::ShaderType::vertex);
    ps = device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.ps"), wis::ShaderType::pixel);

    constants_heap = device.CreateDescriptorHeap(1, wis::PoolType::CBV_SRV_UAV);
    constant_buffer = allocator.CreateConstantBuffer(sizeof(SceneConstantBuffer));

    // model, view, and projection
    constexpr auto cube_position = glm::vec3(0.0f, 0.0f, -10.0f);
    cube_transform = glm::translate(cube_transform, cube_position);

    constexpr auto camera_position = glm::vec3(0.0f, 0.2f, 0.0f);
    view = glm::lookAt(camera_position, cube_position, glm::vec3(0.0f, 1.0f, 0.0f));

    constexpr auto near_plane = 0.1f;
    constexpr auto far_plane = 100.0f;
    constexpr auto fov_degrees = 45.0f;
    projection = glm::perspective(glm::radians(fov_degrees),
                                  (float)width / (float)height,
                                  near_plane, far_plane);
    buffer.model_view_projection = cube_transform * view * projection;

    // upload to gpu
    constant_buffer.UpdateSubresource(RawView(buffer));
    mapped_buffer = constant_buffer.MapMemory();

    std::array<wis::BindingDescriptor, 1> bindings{
        wis::BindingDescriptor{
                .binding = 0,
                .stages = wis::ShaderStage::vertex,
                .type = wis::BindingType::CBV,
        }
    };
    wis::DescriptorSetLayout constants_layout = device.CreateDescriptorSetLayout(bindings);
    constants_set = constants_heap.AllocateDescriptorSet(constants_layout);
    device.CreateConstantBufferView(constant_buffer, sizeof(SceneConstantBuffer), constants_set, constants_layout, 0);

    root = device.CreateRootSignature({ &constants_layout, 1u }); // empty

    OnResize(width, height);

    vertex_buffer = allocator.CreatePersistentBuffer(sizeof(cube_vertices), wis::BufferFlags::VertexBuffer);

    auto upl_vbuf = allocator.CreateUploadBuffer(sizeof(cube_vertices));
    upl_vbuf.UpdateSubresource(RawView(cube_vertices));

    context.Reset();
    context.CopyBuffer(upl_vbuf, vertex_buffer, sizeof(cube_vertices));
    context.BufferBarrier({ .access_before = wis::ResourceAccess::CopyDest,
                            .access_after = wis::ResourceAccess::VertexBuffer },
                          vertex_buffer);
    context.Close();

    queue.ExecuteCommandList(context);
    WaitForGPU();

    vb = vertex_buffer.GetVertexBufferView(sizeof(Vertex));
    context.SetPipeline(pipeline);
}

Test::App::~App()
{
    WaitForGPU();
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

void Test::App::Frame()
{
    context.Reset();
    auto back = swap.GetBackBuffer();

    cube_transform = glm::rotate(cube_transform, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    buffer.model_view_projection = cube_transform * view * projection;
    float *mapped = reinterpret_cast<float *>(mapped_buffer.data());
    std::memcpy(mapped, &buffer.model_view_projection, sizeof(glm::mat4));

    context.TextureBarrier({ .state_before = wis::TextureState::Present,
                             .state_after = wis::TextureState::RenderTarget,
                             .access_before = wis::ResourceAccess::Common,
                             .access_after = wis::ResourceAccess::RenderTarget },
                           back);

    constexpr wis::ColorClear color{ 0.0f, 0.2f, 0.4f, 1.0f };
    constexpr wis::ColorClear color2{ 1.0f, 0.2f, 0.4f, 1.0f };
    std::array rtvsx{
        std::pair{ rtvs[swap.GetNextIndex()], color },
        std::pair{ rtvs2[swap.GetNextIndex()], color2 }
    };

    context.SetGraphicsDescriptorSet(root, 0, constants_set);
    context.RSSetViewport({ float(wnd.width()), float(wnd.height()) });
    context.RSSetScissorRect({ long(wnd.width()), long(wnd.height()) });
    context.IASetPrimitiveTopology(wis::PrimitiveTopology::trianglelist);
    context.IASetVertexBuffers({ &vb, 1 });

    context.BeginRenderPass(render_pass, { rtvsx.data(), static_cast<unsigned int>(swap.StereoSupported()) + 1u });
    context.DrawInstanced(cube_vertices.size());
    context.EndRenderPass();

    context.TextureBarrier({
                                   .state_before = wis::TextureState::RenderTarget,
                                   .state_after = wis::TextureState::Present,
                                   .access_before = wis::ResourceAccess::RenderTarget,
                                   .access_after = wis::ResourceAccess::Common,
                           },
                           back);
    context.Close();
    queue.ExecuteCommandList(context);

    swap.Present();

    WaitForGPU();
}

void Test::App::WaitForGPU()
{
    const uint64_t vfence = fence_value;
    queue.Signal(fence, vfence);
    fence_value++;
    fence.Wait(vfence);
}

void Test::App::OnResize(uint32_t width, uint32_t height)
{
    if (!swap.Resize(width, height))
        return;

    std::array cas2{
        wis::ColorAttachment{
                .format = wis::SwapchainOptions::default_format,
                .load = wis::PassLoadOperation::clear },
        wis::ColorAttachment{
                .format = wis::SwapchainOptions::default_format,
                .load = wis::PassLoadOperation::clear }
    };

    // needs to be recreated for vulkan for now
    render_pass = device.CreateRenderPass({ width, height }, { cas2.data(), static_cast<unsigned int>(swap.StereoSupported()) + 1u });

    // needs to be recreated for vulkan for now
    static constexpr std::array<wis::InputLayoutDesc, 2> ia{
        wis::InputLayoutDesc{ 0, "POSITION", 0, wis::DataFormat::r32g32b32_float, 0, 0, wis::InputClassification::vertex, 0 },
        wis::InputLayoutDesc{ 1, "COLOR", 0, wis::DataFormat::r32g32b32a32_float, 0, 12, wis::InputClassification::vertex, 0 }
    };

    wis::GraphicsPipelineDesc desc{ root };
    desc.SetVS(vs);
    desc.SetPS(ps);
    desc.SetRenderPass(render_pass);
    pipeline = device.CreateGraphicsPipeline(desc, ia);
    context.SetPipeline(pipeline);

    auto x = swap.GetRenderTargets();
    for (size_t i = 0; i < x.size(); i++) {
        rtvs[i] = device.CreateRenderTargetView(x[i]);
        if (swap.StereoSupported())
            rtvs2[i] = device.CreateRenderTargetView(x[i], { .base_layer = 1 });
    }
}
