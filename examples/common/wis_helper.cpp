#include "wis_helper.h"
#include <iostream>
#include <fstream>

#include <wisdom/bridge/format.h>

void ex::ExampleSetup::DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

wis::Factory ex::ExampleSetup::InitDefaultFactory(wis::FactoryExtension* platform_ext)
{
    using namespace wis;
    wis::DebugExtension debug_ext; // no need to store it, it will be destroyed when it goes out of scope

    wis::Result res;
    wis::FactoryExtension* xfactory_exts[] = { &debug_ext, platform_ext };
    wis::Factory factory = wis::CreateFactory(res, true, xfactory_exts, std::size(xfactory_exts));
    info = debug_ext.CreateDebugMessenger(res, &DebugCallback, &std::cout);
    return factory;
}

void ex::ExampleSetup::InitDefaultDevice(const wis::Factory& factory, std::span<wis::DeviceExtension*> device_exts)
{
    wis::Result result = wis::success;
    for (size_t i = 0;; i++) {
        auto adapter = factory.GetAdapter(result, i);
        if (result.status != wis::Status::Ok) {
            break;
        }

        wis::AdapterDesc desc;
        result = adapter.GetDesc(&desc);
        std::cout << "Adapter: " << desc.description.data() << "\n";

        device = wis::CreateDevice(result, std::move(adapter), device_exts.data(), device_exts.size());
        if (result.status == wis::Status::Ok) {
            return;
        }
    }
    throw Exception("No suitable adapter found");
}

void ex::ExampleSetup::InitDefaultQueue()
{
    queue = Unwrap(device.CreateCommandQueue(wis::QueueType::Graphics));
    fence = Unwrap(device.CreateFence(0));
    allocator = Unwrap(device.CreateAllocator());
}

void ex::ExampleSetup::InitDefault(wis::FactoryExtension* platform_ext, std::span<wis::DeviceExtension*> device_exts)
{
    wis::Factory factory = InitDefaultFactory(platform_ext);
    InitDefaultDevice(factory, device_exts);
    InitDefaultQueue();
}

void ex::ExampleSetup::WaitForGPU()
{
    CheckResult(queue.SignalQueue(fence, ++fence_value));
    CheckResult(fence.Wait(fence_value));
}

std::string ex::LoadShader(std::filesystem::path p)
{
    if constexpr (wis::shader_intermediate == wis::ShaderIntermediate::DXIL) {
        p += u".cso";
    } else {
        p += u".spv";
    }

    if (!std::filesystem::exists(p)) {
        throw Exception(wis::format("Shader file not found: {}", p.string()));
    }

    std::ifstream t{ p, std::ios::binary };
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string ret;
    ret.resize(size);
    t.seekg(0);
    t.read(ret.data(), size);
    return ret;
}
