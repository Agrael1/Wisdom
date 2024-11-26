#include <catch2/catch_test_macros.hpp>
#include <wisdom/wisdom_platform.h>
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.h>
#include <wisdom/wisdom_extended_allocation.h>
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

TEST_CASE("basic_copy_region")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    wis::ExtendedAllocation ext_alloc;
    wis::DeviceExtension* exts_d[] = { &ext_alloc };

    auto [res, factory] = wis::CreateFactory(true, exts.data(), 1);
    auto [res1, info] = ext.CreateDebugMessenger(&DebugCallback, &error);

    wis::Device device;

    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            auto [res, hdevice] = wis::CreateDevice(std::move(adapter), exts_d, 1);
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
    auto [res6, buffer] = allocator.CreateReadbackBuffer(1024);

    using namespace wis;
    wis::TextureDesc desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = { 4, 4, 1 },
        .usage = wis::TextureUsage::CopySrc | wis::TextureUsage::HostCopy
    };
    auto [extres, texture] = ext_alloc.CreateGPUUploadTexture(allocator, desc);
    REQUIRE(extres.status == wis::Status::Ok);

    // clang-format off
    constexpr uint32_t data[16] = {
        0,0,0,0,
        0,0,0,0,
        0,0,-1,-1,
        0,0,-1,-1
    };
    // clang-format on

    extres = ext_alloc.WriteMemoryToSubresourceDirect(data,
                                                      texture,
                                                      wis::TextureState::Common,
                                                      wis::TextureRegion{
                                                              .size = { 4, 4, 1 },
                                                              .format = wis::DataFormat::RGBA8Unorm,
                                                      });

    REQUIRE(extres.status == wis::Status::Ok);

    wis::BufferTextureCopyRegion region{
        .buffer_offset = 4,
        .texture = {
                .offset = { 3, 3, 0 },
                .size = { 1, 1, 1 },
                .format = wis::DataFormat::RGBA8Unorm,
        }
    };
    cmd_list.CopyTextureToBuffer(texture, buffer, &region, 1);
    cmd_list.Close();

    wis::CommandListView cmd_list_view{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view, 1);
    queue.SignalQueue(fence, 1);
    fence.Wait(1);

    auto* data_ptr = buffer.Map<uint32_t>();
    constexpr uint32_t data_expected[] = { 0, -1,-1,-1,-1 };

    for (size_t i = 0; i < 5; i++) {
        REQUIRE(data_ptr[i] == data_expected[i]);
    }
}
