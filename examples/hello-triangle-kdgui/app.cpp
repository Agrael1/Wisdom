#include "app.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/vector_float2.hpp>

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

std::vector<uint8_t> GenerateTextureData(size_t width, size_t height, size_t texel_size)
{
    const size_t rowPitch = width * texel_size;
    const size_t cellPitch = rowPitch >> 3; // The width of a cell in the checkerboard texture.
    const size_t cellHeight = width >> 3; // The height of a cell in the checkerboard texture.
    const size_t textureSize = rowPitch * height;

    std::vector<uint8_t> data(textureSize);
    uint8_t* pData = &data[0];

    for (size_t n = 0; n < textureSize; n += texel_size) {
        size_t x = n % rowPitch;
        size_t y = n / rowPitch;
        size_t i = x / cellPitch;
        size_t j = y / cellHeight;

        if (i % 2 == j % 2) {
            pData[n] = 0x00; // R
            pData[n + 1] = 0x00; // G
            pData[n + 2] = 0x00; // B
            pData[n + 3] = 0xff; // A
        } else {
            pData[n] = 0xff; // R
            pData[n + 1] = 0xff; // G
            pData[n + 2] = 0xff; // B
            pData[n + 3] = 0xff; // A
        }
    }

    return data;
}

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
std::span<std::byte> RawView(T& data)
{
    return { (std::byte*)&data, sizeof(T) };
}

Test::App::App(uint32_t width, uint32_t height)
    : wnd(app.createWindow(width, height))
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    factory.emplace(app_info);

    for (auto&& a : factory->EnumerateAdapters(wis::AdapterPreference::Performance)) {
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

    vs = device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.tex.vs"), wis::ShaderType::vertex);
    ps = device.CreateShader(LoadShader<wis::Shader>(SHADER_DIR "/example.tex.ps"), wis::ShaderType::pixel);

    srv_heap = device.CreateDescriptorHeap(1, wis::PoolType::CBV_SRV_UAV);
    sampler_heap = device.CreateDescriptorHeap(1, wis::PoolType::SAMPLER);

    wis::BindingDescriptor srv_bind{
        .binding = 0,
        .stages = wis::ShaderStage::all,
    };
    wis::BindingDescriptor splr_bind{
        .binding = 0,
        .stages = wis::ShaderStage::all,
        .type = wis::BindingType::SAMPLER,
    };

    wis::DescriptorSetLayout dsl_srv = device.CreateDescriptorSetLayout({ &srv_bind, 1 });
    wis::DescriptorSetLayout dsl_splr = device.CreateDescriptorSetLayout({ &splr_bind, 1 });
    srv_set = srv_heap.AllocateDescriptorSet(dsl_srv);
    sampler_set = sampler_heap.AllocateDescriptorSet(dsl_splr);

    std::array a{ dsl_srv, dsl_splr };
    root = device.CreateRootSignature(a); // empty

    OnResize(width, height);

    struct Vertex {
        glm::vec3 pos;
        glm::vec2 tc;
    };
    auto aspect_ratio = float(width) / float(height);
    Vertex triangleVertices[] = {
        { { 0.0f, 0.25f * aspect_ratio, 0.0f }, { 0.5f, 1.0f } },
        { { 0.25f, -0.25f * aspect_ratio, 0.0f }, { 0.0f, 0.0f } },
        { { -0.25f, -0.25f * aspect_ratio, 0.0f }, { 1.0f, 0.0f } }
    };

    vertex_buffer = allocator.CreatePersistentBuffer(sizeof(triangleVertices), wis::BufferFlags::VertexBuffer);

    auto upl_vbuf = allocator.CreateUploadBuffer(sizeof(triangleVertices));
    upl_vbuf.UpdateSubresource(RawView(triangleVertices));

    texture = allocator.CreateTexture(wis::TextureDescriptor{
            .width = 128,
            .height = 128,
            .format = wis::DataFormat::r8g8b8a8_unorm });
    auto data = GenerateTextureData(128, 128, 4);
    auto upl_tbuf = allocator.CreateUploadBuffer(data.size());
    upl_tbuf.UpdateSubresource({ (std::byte*)data.data(), data.size() });

    sampler = device.CreateSampler();

    context.Reset();
    context.CopyBuffer(upl_vbuf, vertex_buffer, sizeof(triangleVertices));
    context.BufferBarrier({ .access_before = wis::ResourceAccess::CopyDest,
                            .access_after = wis::ResourceAccess::VertexBuffer },
                          vertex_buffer);

    context.TextureBarrier({ .state_before = wis::TextureState::Undefined,
                             .state_after = wis::TextureState::CopyDst,
                             .access_before = wis::ResourceAccess::Common,
                             .access_after = wis::ResourceAccess::CopyDest },
                           texture);

    context.CopyTexture(upl_tbuf, texture, wis::TextureRange{ 0, 0, wis::Size3D{ 128, 128, 1 } });

    context.TextureBarrier({ .state_before = wis::TextureState::CopyDst,
                             .state_after = wis::TextureState::ShaderResource,
                             .access_before = wis::ResourceAccess::CopyDest,
                             .access_after = wis::ResourceAccess::ShaderResource },
                           texture);
    context.Close();

    queue.ExecuteCommandList(context);
    WaitForGPU();

    srv = device.CreateShaderResourceView(texture, { 0, 1, 0, 1 });
    vb = vertex_buffer.GetVertexBufferView(sizeof(Vertex));
    device.WriteShaderResourceView(srv, srv_set, dsl_srv, 0);
    device.WriteSampler(sampler, sampler_set, dsl_splr, 0);

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

    //context.SetGraphicsDescriptorSet(root);
    context.SetGraphicsDescriptorSet(root,0, srv_set);
    context.SetGraphicsDescriptorSet(root,1, sampler_set);

    context.RSSetViewport({ float(wnd.width()), float(wnd.height()) });
    context.RSSetScissorRect({ long(wnd.width()), long(wnd.height()) });
    context.IASetPrimitiveTopology(wis::PrimitiveTopology::trianglelist);
    context.IASetVertexBuffers({ &vb, 1 });

    context.BeginRenderPass(render_pass, { rtvsx.data(), static_cast<unsigned int>(swap.StereoSupported()) + 1u });
    context.DrawInstanced(3);
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
    render_pass = device.CreateRenderPass({ width, height }, { cas2.data(), swap.StereoSupported() + 1u });

    // needs to be recreated for vulkan for now
    static constexpr std::array<wis::InputLayoutDesc, 2> ia{
        wis::InputLayoutDesc{ 0, "POSITION", 0, wis::DataFormat::r32g32b32_float, 0, 0, wis::InputClassification::vertex, 0 },
        wis::InputLayoutDesc{ 1, "TEXCOORD0", 0, wis::DataFormat::r32g32_float, 0, 12, wis::InputClassification::vertex, 0 }
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
