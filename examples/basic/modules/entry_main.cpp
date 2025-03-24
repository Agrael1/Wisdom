#include <exception>
#include <iostream>
#include "window.h"

#ifdef WISDOM_FORCE_VULKAN
import wisdom.fvk;
import wisdom.debug.fvk;
import wisdom.descriptor_buffer.fvk;
#else
import wisdom;
import wisdom.debug;
import wisdom.descriptor_buffer;
#endif

class App
{
    static void DebugCallback(wis::Severity severity, const char* message, void* user_data)
    {
        std::cout << "Debug: " << message << std::endl;
    }

public:
    App()
        : window("Wisdom Module Example", 800, 600)
    {
        wis::Result result = wis::success;
        wis::DebugExtension debug_ext;

        wis::FactoryExtension* exts[] = { &debug_ext, platform_ext.get() };
        wis::Factory factory = wis::CreateFactory(result, true, exts, std::size(exts));
        if (result.status != wis::Status::Ok) {
            throw std::runtime_error("Failed to create factory");
        }

        info = debug_ext.CreateDebugMessenger(result, DebugCallback, nullptr);

        for (size_t i = 0;; i++) {
            wis::Adapter adapter = factory.GetAdapter(result, i);
            auto& a = adapter.GetInternal();
            if (result.status != wis::Status::Ok) {
                break;
            }

            wis::AdapterDesc desc;
            result = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            device = wis::CreateDevice(result, std::move(adapter));
            if (result.status == wis::Status::Ok) {
                std::cout << "Device created\n";
                break;
            }
        }
        if (result.status != wis::Status::Ok) {
            throw std::runtime_error("Failed to create device");
        }

        queue = device.CreateCommandQueue(result, wis::QueueType::Graphics);

        if (result.status != wis::Status::Ok) {
            throw std::runtime_error("Failed to create command queue");
        }

        swapchain = window.CreateSwapchain(result, platform_ext, device, queue);
    }

    int Run()
    {
        return 0;
    }

private:
    ex::Window window;
    ex::PlatformExtension platform_ext;

    wis::DebugMessenger info;

    wis::Device device;
    wis::CommandQueue queue;
    wis::SwapChain swapchain;
};

int main(int argc, char** argv)
{
    ex::Instance instance;

    try {
        return App{}.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
