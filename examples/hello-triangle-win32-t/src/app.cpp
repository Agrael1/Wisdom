#include <example/app.h>
#include <iostream>
#include <wisdom/platform/win32.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <filesystem>
#include <fstream>

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
    {
        std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

Test::App::App(uint32_t width, uint32_t height)
    : wnd(width, height, "VTest")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    auto [result, factory] = wis::CreateFactory(true);

    auto [resx, hinfo] = factory.CreateDebugMessenger(DebugCallback, &std::cout);
    info = std::move(hinfo);

    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            auto [res, hdevice] = wis::CreateDevice(std::move(adapter));
            if (res.status == wis::Status::Ok) {
                device = std::move(hdevice);
                break;
            };

        } else {
            break;
        }
    }

    auto [res2, hqueue] = device.CreateCommandQueue(wis::QueueType::Graphics);
    queue = std::move(hqueue);

    {
        wis::SwapchainDesc desc{
            .size = { uint32_t(wnd.GetWidth()), uint32_t(wnd.GetHeight()) },
            .format = wis::DataFormat::BGRA8Unorm,
            .buffer_count = 2,
            .stereo = true,
            .vsync = true,
        };

        auto [res3, hswap] = wis::CreateSwapchainWin32(device, queue, &desc,
                                                       wnd.GetHandle());
        swap = std::move(hswap);
        back_buffers = swap.GetBufferSpan();

        wis::RenderTargetDesc rt_desc{
            .format = wis::DataFormat::BGRA8Unorm,
            .layout = wis::TextureLayout::Texture2D,
            .mip = 0,
            .base_array_layer = 0,
            .layer_count = 1,
        };
        for (size_t i = 0; i < render_targets.size(); i++) {
            auto [res, hrt] = device.CreateRenderTarget(back_buffers[i], rt_desc);
            render_targets[i] = std::move(hrt);
        }
    }

    auto [res4, hfence] = device.CreateFence();
    fence = std::move(hfence);

    auto [res5, hcmd_list] = device.CreateCommandList(wis::QueueType::Graphics);
    cmd_list = std::move(hcmd_list);

    CreateResources();
}

int Test::App::Start()
{
    while (true) {
        if (const auto a = wnd.ProcessMessages())
            return (int)a.value();

        for (auto e : wnd.GetEvents())
            ProcessEvent(e);

        Frame();
    }
}

std::string LoadShader(std::filesystem::path p)
{
    if constexpr (wis::shader_intermediate == wis::ShaderIntermediate::DXIL)
        p += u".cso";
    else
        p += u".spv";

    std::ifstream t{ p, std::ios::binary };
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string ret;
    ret.resize(size);
    t.seekg(0);
    t.read(ret.data(), size);
    return ret;
}

void Test::App::CreateResources()
{
    struct Vertex {
        glm::vec3 pos;
        glm::vec4 col;
    };
    auto aspect_ratio = float(wnd.GetWidth()) / float(wnd.GetHeight());
    Vertex triangleVertices[] = {
        { { 0.0f, 0.25f * aspect_ratio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f * aspect_ratio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f * aspect_ratio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    {
        auto [res, alloc] = device.CreateAllocator();
        allocator = std::move(alloc);
    }
    auto [resx, ubuf] = allocator.CreateUploadBuffer(sizeof(triangleVertices));

    {
        auto [res, vbuf] = allocator.CreateCommitedBuffer(sizeof(triangleVertices), wis::BufferFlags::VertexBuffer);
        vertex_buffer = std::move(vbuf);
    }

    // Upload vertex data to a buffer
    {
        auto memory = ubuf.Map<Vertex>();
        std::copy(std::begin(triangleVertices), std::end(triangleVertices), memory);
        ubuf.Unmap();

        cmd_list.CopyBuffer(ubuf, vertex_buffer, { .size_bytes = sizeof(triangleVertices) });
        cmd_list.BufferBarrier({
                                       .sync_before = wis::BarrierSync::All,
                                       .sync_after = wis::BarrierSync::Draw,
                                       .access_before = wis::ResourceAccess::Common,
                                       .access_after = wis::ResourceAccess::VertexBuffer,
                               },
                               vertex_buffer);
        cmd_list.Close();

        wis::CommandListView cmd_lists[] = { cmd_list };
        queue.ExecuteCommandLists(cmd_lists, 1);
    }

    {
        auto s1 = LoadShader(SHADER_DIR "/example.vs");
        auto s2 = LoadShader(SHADER_DIR "/example.ps");
        auto [result, vs] = device.CreateShader(s1.data(), uint32_t(s1.size()));
        auto [result2, ps] = device.CreateShader(s2.data(), uint32_t(s2.size()));

        vertex_shader = std::move(vs);
        pixel_shader = std::move(ps);
    }

    {
        auto [result, hroot] = device.CreateRootSignature();
        root = std::move(hroot);

        wis::InputSlotDesc input_slots[] = {
            { .slot = 0, .stride_bytes = sizeof(Vertex), .input_class = wis::InputClass::PerVertex },
        };
        wis::InputAttribute input_attributes[] = {
            { .input_slot = 0, .semantic_name = "POSITION", .semantic_index = 0, .location = 0, .format = wis::DataFormat::RGB32Float, .offset_bytes = 0 },
            { .input_slot = 0, .semantic_name = "COLOR", .semantic_index = 0, .location = 1, .format = wis::DataFormat::RGBA32Float, .offset_bytes = 12 },
        };
        wis::DataFormat attachment_formats[] = { wis::DataFormat::BGRA8Unorm };

        wis::GraphicsPipelineDesc desc{
            .root_signature = root,
            .input_layout = {
                    .slots = input_slots,
                    .slot_count = 1,
                    .attributes = input_attributes,
                    .attribute_count = 2,
            },
            .shaders = { .vertex = vertex_shader, .pixel = pixel_shader },
            .attachments = {
                    .attachment_formats = attachment_formats,
                    .attachments_count = 1,
            }
        };
        auto [res2, hpipeline] = device.CreateGraphicsPipeline(&desc);
        pipeline = std::move(hpipeline);
    }

    WaitForGPU();
}

void Test::App::ProcessEvent(Event e)
{
    switch (e) {
    case Event::Resize:
        return OnResize(wnd.GetWidth(), wnd.GetHeight());
    }
}

void Test::App::OnResize(uint32_t width, uint32_t height)
{
    auto result = swap.Resize(width, height);
    if (result.status != wis::Status::Ok)
        throw std::runtime_error("Failed to resize swapchain");

    wis::RenderTargetDesc rt_desc{
        .format = wis::DataFormat::BGRA8Unorm,
        .layout = wis::TextureLayout::Texture2D,
        .mip = 0,
        .base_array_layer = 0,
        .layer_count = 1,
    };

    back_buffers = swap.GetBufferSpan();
    for (size_t i = 0; i < render_targets.size(); i++) {
        auto [res, hrt] = device.CreateRenderTarget(back_buffers[i], rt_desc);
        render_targets[i] = std::move(hrt);
    }
}

void Test::App::Frame()
{
    auto res = cmd_list.Reset(pipeline);

    cmd_list.TextureBarrier({
                                    .sync_before = wis::BarrierSync::All,
                                    .sync_after = wis::BarrierSync::Draw,
                                    .access_before = wis::ResourceAccess::Common,
                                    .access_after = wis::ResourceAccess::RenderTarget,
                                    .state_before = wis::TextureState::Present,
                                    .state_after = wis::TextureState::RenderTarget,
                                    .subresource_range = {
                                            .base_mip_level = 0,
                                            .level_count = 1,
                                            .base_array_layer = 0,
                                            .layer_count = 1,
                                    },
                            },
                            back_buffers[swap.GetCurrentIndex()]);

    wis::RenderPassRenderTargetDesc targets{
        .target = render_targets[swap.GetCurrentIndex()],
        .load_op = wis::LoadOperation::Clear,
        .store_op = wis::StoreOperation::Store,
        .clear_value = { 0.0f, 0.2f, 0.4f, 1.0f },
    };
    wis::RenderPassDesc rp{
        .targets = &targets,
        .target_count = 1,
        .flags = wis::RenderPassFlags::None,
    };
    cmd_list.BeginRenderPass(&rp);
    cmd_list.SetRootSignature(root);
    cmd_list.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
    cmd_list.RSSetViewport({ 0, 0, float(wnd.GetWidth()), float(wnd.GetHeight()), 0, 1 });
    cmd_list.RSSetScissor({ 0, 0, wnd.GetWidth(), wnd.GetHeight() });
    cmd_list.EndRenderPass();


    cmd_list.TextureBarrier({
                                    .sync_before = wis::BarrierSync::Draw,
                                    .sync_after = wis::BarrierSync::All,
                                    .access_before = wis::ResourceAccess::RenderTarget,
                                    .access_after = wis::ResourceAccess::Common,
                                    .state_before = wis::TextureState::RenderTarget,
                                    .state_after = wis::TextureState::Present,
                                    .subresource_range = {
                                            .base_mip_level = 0,
                                            .level_count = 1,
                                            .base_array_layer = 0,
                                            .layer_count = 1,
                                    },
                            },
                            back_buffers[swap.GetCurrentIndex()]);

    cmd_list.Close();

    wis::CommandListView lists[] = { cmd_list };
    queue.ExecuteCommandLists(lists, 1);

    auto result = swap.Present();
    if (result.status != wis::Status::Ok && result.status != wis::Status::Occluded)
        throw std::runtime_error("Failed to present swapchain");

    WaitForGPU();
}

void Test::App::WaitForGPU()
{
    const uint64_t vfence = fence_value;
    queue.SignalQueue(fence, vfence);
    fence_value++;
    fence.Wait(vfence);
}