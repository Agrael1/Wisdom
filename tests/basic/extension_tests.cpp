#include <catch2/catch_test_macros.hpp>
#include <wisdom/wisdom_platform.hpp>
#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_debug.hpp>
#include <wisdom/wisdom_extended_allocation.hpp>
#include <iostream>

struct LogProvider3 : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override {
        // std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
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
TEST_CASE("host_texture_write_ext")
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider3>());

    bool error = false;

    wis::DebugExtension ext;
    std::array<wis::FactoryExtension*, 1> exts = { &ext };

    auto [res, factory] = wis::CreateFactory(true, exts.data(), 1);
    auto [res1, info] = ext.CreateDebugMessenger(&DebugCallback, &error);

    wis::Device device;
    wis::ExtendedAllocation a;

    wis::DeviceExtension* exts2[] = { &a };

    for (size_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            auto [res, hdevice] = wis::CreateDevice(std::move(adapter), exts2, 1);
            if (res.status == wis::Status::Ok) {
                device = std::move(hdevice);
                break;
            };

        } else {
            break;
        }
    }

    REQUIRE(device);
    if (!a.Supported()) {
        return;
    }

    auto [res2, allocator] = device.CreateAllocator();
    REQUIRE(res2.status == wis::Status::Ok);

    uint32_t texture_data[4] = { 0 };
    texture_data[0] = 0xFF0000FF;
    texture_data[1] = 0x00FF00FF;
    texture_data[2] = 0x0000FFFF;
    texture_data[3] = 0xFFFF00FF;

    using namespace wis;

    bool support = a.SupportedDirectGPUUpload(wis::DataFormat::RGBA8Unorm);
    if (!support) {
        // Not supported for DX12, no need to use this test
        return;
    }

    // Create a texture with a size of 2x2 for host copy
    auto [res3, texture] = a.CreateGPUUploadTexture(allocator, wis::TextureDesc{ .format = wis::DataFormat::RGBA8Unorm, .size = { 2, 2, 1 }, .mip_levels = 1, .layout = wis::TextureLayout::Texture2D, .usage = wis::TextureUsage::HostCopy | wis::TextureUsage::ShaderResource });
    REQUIRE(res3.status == wis::Status::Ok);

    // Perform a copy from the host memory to the texture

    res3 = a.WriteMemoryToSubresourceDirect(texture_data, texture, wis::TextureState::Common, wis::TextureRegion{
                                                                                                      .size = { 2, 2, 1 },
                                                                                                      .format = wis::DataFormat::RGBA8Unorm,
                                                                                              });

    REQUIRE(res3.status == wis::Status::Ok);
}
