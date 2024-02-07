// #include "app.h"
#include <example/window.h>
//#define WISDOM_FORCE_VULKAN
//#undef WISDOM_FORCE_VULKAN
//#include <wisdom/wisdom.hpp>
//#include <wisdom/platform/win32.h>
#include <wisdom/xdx12/dx12_factory.h>
#include <iostream>

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}
void DebugCallback2(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

int main()
{
    Window window(1920, 1080, "Example");

    auto [res, factory] = wis::DX12CreateFactory(false, &DebugCallback, &std::cout);
    /*{
        auto [res, factory] = wis::CreateFactory(false, &DebugCallback, &std::cout);

        wis::Device device;

        for (size_t i = 0;; i++) {
            auto [res, adapter] = factory.GetAdapter(i);
            if (res.status == wis::Status::Ok) {
                wis::AdapterDesc desc;
                adapter.GetDesc(&desc);
                std::cout << "Adapter: " << desc.description.data() << "\n";

                auto [res, xdevice] = wis::CreateDevice(factory, adapter);
                if (res.status == wis::Status::Ok) {
                    device = std::move(xdevice);
                    break;
                };

            } else {
                break;
            }
        }

        auto fence = device.CreateFence();
        auto allocator = device.CreateAllocator();
        auto root_signature = device.CreateRootSignature();
        auto command_queue = device.CreateCommandQueue(wis::QueueType::Graphics);
        auto command_list = device.CreateCommandList(wis::QueueType::Graphics);

        wis::SwapchainDesc desc{
            .size = { uint32_t(window.GetWidth()), uint32_t(window.GetHeight()) },
            .format = wis::DataFormat::BGRA8Unorm,
            .buffer_count = 2,
            .stereo = true,
            .vsync = true,
        };
        auto swap = wis::CreateSwapchainWin32(device, command_queue.second, &desc, window.GetHandle());
    }*/

    return 0;

    // Test::App app(1920, 1080);
    // app.Start();
}