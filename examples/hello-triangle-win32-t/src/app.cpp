#include <example/app.h>
#include <iostream>
#include <wisdom/platform/win32.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

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
        cmd_list.Close();

        wis::CommandListView cmd_lists[] = { cmd_list };
        queue.ExecuteCommandLists(cmd_lists, 1);
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
}

void Test::App::Frame()
{


    auto result = swap.Present();
    if (result.status != wis::Status::Ok)
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