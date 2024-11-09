#include "wis_helper.h"
#include <iostream>
#include <fstream>

void ex::ExampleSetup::DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

wis::Factory ex::ExampleSetup::InitDefaultFactory(wis::FactoryExtension* platform_ext)
{
    using namespace wis;
    wis::DebugExtension debug_ext; // no need to store it, it will be destroyed when it goes out of scope

    wis::FactoryExtension* xfactory_exts[] = { &debug_ext, platform_ext };
    wis::Factory factory = Unwrap(wis::CreateFactory(true, xfactory_exts, std::size(xfactory_exts)));
    info = Unwrap(debug_ext.CreateDebugMessenger(&DebugCallback, &std::cout));
    return std::move(factory);
}

void ex::ExampleSetup::InitDefaultDevice(const wis::Factory& factory)
{
    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status != wis::Status::Ok)
            break;

        wis::AdapterDesc desc;
        res = adapter.GetDesc(&desc);
        std::cout << "Adapter: " << desc.description.data() << "\n";

        auto [res2, hdevice] = wis::CreateDevice(std::move(adapter));
        if (res2.status == wis::Status::Ok) {
            device = std::move(hdevice);
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

void ex::ExampleSetup::InitDefault(wis::FactoryExtension* platform_ext)
{
    wis::Factory factory = InitDefaultFactory(platform_ext);
    InitDefaultDevice(factory);
    InitDefaultQueue();
}

void ex::ExampleSetup::WaitForGPU()
{
    CheckResult(queue.SignalQueue(fence, ++fence_value));
    CheckResult(fence.Wait(fence_value));
}

std::string ex::LoadShader(std::filesystem::path p)
{
    if constexpr (wis::shader_intermediate == wis::ShaderIntermediate::DXIL)
        p += u".cso";
    else
        p += u".spv";

    if (!std::filesystem::exists(p))
        throw Exception(wis::format("Shader file not found: {}", p.string()));

    std::ifstream t{ p, std::ios::binary };
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string ret;
    ret.resize(size);
    t.seekg(0);
    t.read(ret.data(), size);
    return ret;
}
