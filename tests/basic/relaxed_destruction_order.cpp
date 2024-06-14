#include <catch2/catch_test_macros.hpp>
#include <wisdom/wisdom_platform.h>
#include <wisdom/wisdom.hpp>
#include <iostream>

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override {
        // std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    std::cout << message << std::endl;
    if (severity >= wis::Severity::Error) {
        auto& error = *static_cast<bool*>(user_data);
        error = true;
    }
}

TEST_CASE("basic_factory")
{
    bool error = false;
    auto [res, factory] = wis::CreateFactory(true);
    auto [res1, info] = factory.CreateDebugMessenger(&DebugCallback, &error);

    factory = {};
    REQUIRE_FALSE(factory);
    REQUIRE_FALSE(error);
}

TEST_CASE("basic_device")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    bool error = false;
    auto [res, factory] = wis::CreateFactory(true);
    auto [res1, info] = factory.CreateDebugMessenger(&DebugCallback, &error);

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

    SECTION("fence destruction")
    {
        fence = {};
        REQUIRE_FALSE(fence);
        REQUIRE_FALSE(error);
    }

    SECTION("reverse order destruction")
    {
        auto [res5, fence2] = device.CreateFence();

        factory = {};
        REQUIRE_FALSE(factory);
        REQUIRE_FALSE(error);

        device = {};
        REQUIRE_FALSE(device);
        REQUIRE_FALSE(error);

        queue = {};
        REQUIRE_FALSE(queue);
        REQUIRE_FALSE(error);

        fence2 = {};
        REQUIRE_FALSE(fence2);
        REQUIRE_FALSE(error);

        allocator = {};
        REQUIRE_FALSE(allocator);
        REQUIRE_FALSE(error);
    }

    SECTION("command list replacement")
    {
        wis::CommandList a;
        wis::CommandList cmd_list2 = std::move(a);
        REQUIRE_FALSE(cmd_list2);
        REQUIRE_FALSE(error);

        REQUIRE_FALSE(a);
        a = {};
        REQUIRE_FALSE(error);
        REQUIRE_FALSE(a);
    }
}
