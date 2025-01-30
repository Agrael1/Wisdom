#include "app.h"
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>
#include <wisdom/wisdom_debug.hpp>
#include <iostream>
#include <stb_image_write.h>

//--------------------------------------------------------------------------------------------------

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
    {
        std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    std::cout << message << "\n";
}

//--------------------------------------------------------------------------------------------------

App::App(uint32_t width, uint32_t height)
    : wnd(width, height, "Lut Test"), width(width), height(height)
{
    CreateDevices();
}
App::~App()
{
    transfer.input_buffer.Unmap();
}

void App::CreateDevices()
{
    // Create factory
    wis::DebugExtension debug_ext;
    wis::platform::WindowsExtension windows_ext;

    wis::FactoryExtension* extensions[] = { &windows_ext, &debug_ext };
    auto [result, factory] = wis::CreateFactory(false, extensions, std::size(extensions));
    if (result.status != wis::Status::Ok) {
        throw std::runtime_error("Failed to create factory");
    }

    // Create debug messenger
    auto [result2, hinfo] = debug_ext.CreateDebugMessenger(DebugCallback, nullptr);
    if (result2.status != wis::Status::Ok) {
        throw std::runtime_error("Failed to create debug messenger");
    }

    info = std::move(hinfo);

    // Create devices
    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            if (!transfer.transfer_device) {
                std::cout << wis::format("Work adapter: {}\n", desc.description.data());
                auto result = CreateTransferNode(std::move(adapter));
                if (result) {
                    transfer = std::move(result.value());
                }
                continue;
            }
            if (!work.work_device) {
                std::cout << wis::format("Work adapter: {}\n", desc.description.data());
                auto result = CreateWorkNode(std::move(adapter));
                if (result) {
                    work = std::move(result.value());
                }
            }
        } else {
            if (!work.work_device) {
                throw std::runtime_error("Failed to create work device");
            }
            if (!transfer.transfer_device) {
                throw std::runtime_error("Failed to create transfer device");
            }
            break;
        }
    }

    CreateSwapChain(windows_ext);
}

uint8_t* buffer;

void App::CreateSwapChain(const wis::platform::WindowsExtension& platform)
{
    wis::SwapchainDesc desc{
        .size = { uint32_t(wnd.GetWidth()), uint32_t(wnd.GetHeight()) },
        .format = wis::DataFormat::RGBA8Unorm,
        .buffer_count = TransferNode::kFrameCount,
        .stereo = false,
        .vsync = false,
        .tearing = false,
    };
    auto [result, swap] = platform.CreateSwapchain(transfer.transfer_device, transfer.queue, desc, wnd.GetHandle());
    if (result.status != wis::Status::Ok) {
        throw std::runtime_error("Failed to create swapchain");
    }

    transfer.InitSwapchain(std::move(swap));
    work.CreateOutputTexture(desc.size);

    transfer.VKCreateInputBuffer(desc.size);

    // buffer = (uint8_t*)_aligned_malloc(desc.size.width * desc.size.height * 4, 4096);
    //  Make Shared memory between work and transfer
    work.ImportExternalBuffer(transfer.input_buffer.Map(), desc.size.width * desc.size.height * 4);
}

int App::Start()
{
    uint32_t frame_count = 0;
    long long elapsed = 0;
    while (frame_count != 100) {
        if (const auto a = wnd.ProcessMessages()) {
            return (int)a.value();
        }

        for (auto e : wnd.GetEvents()) {
            switch (e) {
            case Event::Resize:
                OnResize(wnd.GetWidth(), wnd.GetHeight());
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        Frame();
        auto end = std::chrono::high_resolution_clock::now();
        elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        frame_count++;
    }

    std::cout << "Average frame time: " << elapsed / 100 << "ms\n";
    return 0;
}

void App::OnResize(uint32_t width, uint32_t height)
{
    if (width == this->width && height == this->height) {
        return;
    }

    transfer.input_buffer.Unmap();
    work.CreateOutputTexture({ width, height });

    transfer.VKCreateInputBuffer({ width, height });
    transfer.Resize(width, height);

    //_aligned_free(buffer);
    // buffer = (uint8_t*)_aligned_malloc(width * height * 4,4096 );
    work.ImportExternalBuffer(transfer.input_buffer.Map(), width * height * 4);

    this->width = width;
    this->height = height;
}

void App::Frame()
{
    work.Frame();
    transfer.Frame();
}
