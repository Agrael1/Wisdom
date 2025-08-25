#include <catch2/catch_test_macros.hpp>
#include <wisdom/wisdom_platform.hpp>
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.hpp>
#include <iostream>

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
    {
        std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

static void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    std::cout << message << std::endl;
    if (severity >= wis::Severity::Error) {
        auto& error = *static_cast<bool*>(user_data);
        error = true;
    }
}

TEST_CASE("move_fence_dx12")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    auto [res, factory] = wis::CreateFactory(true, exts.data(), 1);
    auto [res1, info] = ext.CreateDebugMessenger(&DebugCallback, &error);

    wis::Device device;

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

    auto [res2, queue] = device.CreateCommandQueue(wis::QueueType::Graphics);
    auto [res3, fence] = device.CreateFence();
    auto [res4, cmd_list] = device.CreateCommandList(wis::QueueType::Graphics);
    auto [res5, allocator] = device.CreateAllocator();

    REQUIRE(device);
    REQUIRE(queue);
    REQUIRE(fence);
    REQUIRE(cmd_list);

    auto fence2 = std::move(fence);
    REQUIRE(fence2);

#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
    auto& internal = fence.GetInternal();
    REQUIRE(internal.fence.get() == nullptr);
    REQUIRE(internal.fence_event.get() == 0);
#endif
}
