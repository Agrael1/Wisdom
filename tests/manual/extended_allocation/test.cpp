#include <wisdom/wisdom.hpp>
#include <wisdom/wisdom_platform.h>
#include <wisdom/wisdom_debug.h>
#include <wisdom/wisdom_extended_allocation.h>
#include <wisdom/bridge/format.h>
#include <iostream>
#include <chrono>

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

struct Test {
    wis::Device Init()
    {
        wis::Device device;
        wis::DebugExtension global_debug;

        wis::FactoryExtension* exts_i[] = {
            &global_debug,
        };

        auto [result, factory] = wis::CreateFactory(true, exts_i, std::size(exts_i));
        auto [r2, m] = global_debug.CreateDebugMessenger(DebugCallback, &std::cout);
        global_messenger = std::move(m);

        for (size_t i = 0;; i++) {
            auto [res, adapter] = factory.GetAdapter(i);
            if (res.status == wis::Status::Ok) {
                wis::AdapterDesc desc;
                res = adapter.GetDesc(&desc);
                std::cout << "Adapter: " << desc.description.data() << "\n";

                wis::DeviceExtension* exts[] {
                    &global_interop,
                    &global_extended_allocation,
                };

                auto [res, hdevice] = wis::CreateDevice(std::move(adapter), exts, std::size(exts));
                if (res.status == wis::Status::Ok) {
                    device = std::move(hdevice);
                    break;
                };

            } else {
                break;
            }
        }
        return device;
    }

public:
    wis::DebugMessenger global_messenger;
    wis::platform::InteropDeviceExtension global_interop;
    wis::ExtendedAllocation global_extended_allocation;
};

int TestStagedCopy(const wis::ResourceAllocator& allocator, wis::Device& device, wis::Size2D img_size)
{

    auto [res1, cqueue] = device.CreateCommandQueue(wis::QueueType::Copy);
    if (res1.status != wis::Status::Ok) {
        std::cerr << "Failed to create command queue\n";
        return 1;
    }

    auto [res2, clist] = device.CreateCommandList(wis::QueueType::Copy);
    if (res2.status != wis::Status::Ok) {
        std::cerr << "Failed to create command list\n";
        return 1;
    }

    uint64_t fence_value = 1;
    auto [res3, fence] = device.CreateFence();
    if (res3.status != wis::Status::Ok) {
        std::cerr << "Failed to create fence\n";
        return 1;
    }

    auto [res4, ubuf] = allocator.CreateUploadBuffer(img_size.width * img_size.height * 4);
    if (res4.status != wis::Status::Ok) {
        std::cerr << "Failed to create upload buffer\n";
        return 1;
    }

    // Create texture
    wis::TextureDesc desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = { img_size.width, img_size.height, 1 },
        .mip_levels = 1,
        .usage = wis::TextureUsage::CopyDst,
    };
    auto [res5, img] = allocator.CreateTexture(desc);
    if (res5.status != wis::Status::Ok) {
        std::cerr << "Failed to create texture\n";
        return 1;
    }
    clist.Reset();
    clist.TextureBarrier({ .sync_before = wis::BarrierSync::None,
                           .sync_after = wis::BarrierSync::None,
                           .access_before = wis::ResourceAccess::NoAccess,
                           .access_after = wis::ResourceAccess::NoAccess,
                           .state_before = wis::TextureState::Undefined,
                           .state_after = wis::TextureState::CopyDest,
    .subresource_range = {
        .base_mip_level = 0,
        .level_count = 1,
        .base_array_layer = 0,
        .layer_count = 1
    } },
    img);
    clist.Close();
    wis::CommandListView clist_view{ clist };
    cqueue.ExecuteCommandLists(&clist_view, 1);

    auto wait_gpu = [&]() {
        const uint64_t vfence = fence_value;
        cqueue.SignalQueue(fence, vfence);
        fence_value++;
        std::ignore = fence.Wait(vfence);
    };
    wait_gpu();

    // Allocate data
    auto* data = malloc(img_size.width * img_size.height * 4);
    if (!data) {
        std::cerr << "Failed to allocate data\n";
        return 1;
    }

    auto* mapped = ubuf.Map();

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 100; i++) {
        std::memcpy(mapped, data, img_size.width * img_size.height * 4);
        clist.Reset();
        wis::BufferTextureCopyRegion region{
            .buffer_offset = 0,
            .texture = {
                .size = { img_size.width, img_size.height, 1 },
                .format = wis::DataFormat::RGBA8Unorm,
            }
        };
        clist.CopyBufferToTexture(ubuf, img, &region, 1);
        clist.Close();

        wis::CommandListView clist_view{ clist };
        cqueue.ExecuteCommandLists(&clist_view, 1);

        wait_gpu();
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    ubuf.Unmap();

    std::cout << "Elapsed time: " << elapsed.count() << "s\n";
    std::cout << "Per copy time: " << elapsed.count() / 1000 << "s\n";
    return 0;
}

int TestDirectWrite(const Test& test, const wis::Device& device, const wis::ResourceAllocator& alloc, wis::Size2D img_size)
{
    if (!test.global_extended_allocation.SupportedDirectGPUUpload(wis::DataFormat::RGBA8Unorm)) {
        std::cerr << "Direct GPU upload is not supported\n";
        return 1;
    }
    wis::TextureDesc descx{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = { img_size.width, img_size.height, 1 },
        .mip_levels = 1,
        .usage = wis::TextureUsage::CopyDst,
    };
    auto [res5, img] = alloc.CreateTexture(descx);
    if (res5.status != wis::Status::Ok) {
        std::cerr << "Failed to create texture\n";
        return 1;
    }

    using namespace wis;
    wis::TextureDesc desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = { img_size.width, img_size.height, 1 },
        .mip_levels = 1,
        .usage = wis::TextureUsage::HostCopy,
    };
    auto [res, tex] = test.global_extended_allocation.CreateGPUUploadTexture(alloc, desc, wis::TextureState::CopyDest);
    if (res.status != wis::Status::Ok) {
        std::cerr << "Failed to create texture\n";
        return 1;
    }

    auto* data = malloc(img_size.width * img_size.height * 4);
    if (!data) {
        std::cerr << "Failed to allocate data\n";
        return 1;
    }

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 100; i++) {
        wis::TextureRegion region{
            .size = { img_size.width, img_size.height, 1 },
            .format = wis::DataFormat::RGBA8Unorm,
        };
        test.global_extended_allocation.WriteMemoryToSubresourceDirect(data, tex, wis::TextureState::CopyDest, region);
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << "s\n";
    std::cout << "Per copy time: " << elapsed.count() / 1000 << "s\n";
    return 0;
}

int main()
{
    Test test;
    auto device = test.Init();
    if (!device) {
        std::cerr << "Failed to create device\n";
        return 1;
    }

    auto [res4, allocator] = device.CreateAllocator();
    if (res4.status != wis::Status::Ok) {
        std::cerr << "Failed to create allocator\n";
        return 1;
    }

    wis::Size2D img_size{ 8192, 8192 };

    auto batch = [&](wis::Size2D img_size) {
        std::cout << wis::format("\n\nTest image size {}x{}:\n", img_size.width, img_size.height);
        std::cout << "\nStaged copy:\n";
        TestStagedCopy(allocator, device, img_size);
        std::cout << "\n\nDirect copy:\n";
        TestDirectWrite(test, device, allocator, img_size);
    };

    batch({ 8192, 8192 });
    batch({ 4096, 4096 });
    batch({ 2048, 2048 });
    batch({ 1024, 1024 });
}
