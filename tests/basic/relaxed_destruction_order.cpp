#include <wisdom/wisdom.h>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

void log_callback(WisSeverity severity, const char* message, uint64_t device, void* user_data)
{
    const char* severity_str = "";
    switch (severity) {
    case WisSeverityError:
        severity_str = "ERROR";
        break;
    case WisSeverityFatal:
        severity_str = "FATAL";
        FAIL("Fatal message in log: " << (message ? message : ""));
        break;
    default:
        return;
    }
    printf("[%s] %s\n", severity_str, message);
}

TEST_CASE("relaxed_destruction_order")
{
    WisDebugDesc debug_desc = {0};
    debug_desc.enable_debug_layer = true;
    debug_desc.callback = log_callback;
    debug_desc.user_data = NULL;

    WisInstance instance = {0};
    WisResult result = wisCreateInstance(&debug_desc, NULL, 0, &instance);
    REQUIRE(result.status == WisStatusOk);

    WisAdapterQuery adapter_query = {0};
    result = wisInstanceQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapter_query);
    REQUIRE(result.status == WisStatusOk);

    // Destroy instance as we no longer need it
    wisDestroyInstance(&instance);
    WisCommandQueueDesc queue_descs[] = {
        {WisCommandQueueTypeGraphics, WisCommandQueuePriorityHigh},
        {WisCommandQueueTypeCompute, WisCommandQueuePriorityNormal},
    };

    WisDeviceRequirements device_requirements = {
        .queue_descs = queue_descs,
        .queue_desc_count = sizeof(queue_descs) / sizeof(queue_descs[0]),
    };

    WisDevice device = {0};
    size_t adapter_count = wisAdapterQueryGetAdapterCount(&adapter_query);
    REQUIRE(adapter_count > 0);

    bool created = false;
    for (size_t i = 0; i < adapter_count; ++i) {
        WisAdapterDesc desc = {0};
        result = wisAdapterQueryGetAdapterDesc(&adapter_query, i, &desc);
        if (result.status == WisStatusOk) {
            printf(
                "Adapter %zu: Name: %s, VendorID: %u, DeviceID: %u, Flags: %u\n",
                i,
                desc.description,
                desc.vendor_id,
                desc.device_id,
                desc.flags
            );
        }

        result = wisAdapterQueryCreateDevice(&adapter_query, i, &device_requirements, &device);
        printf(
            "CreateDevice result for adapter %zu: %d, platform_code: %d, error: %s\n",
            i,
            result.status,
            result.platform_code,
            result.error ? result.error : "None"
        );
        if (result.status == WisStatusOk) {
            printf("Device created successfully for adapter %zu.\n", i);
            created = true;
            break; // Successfully created a device, exit loop
        }
    }
    REQUIRE(created);

    wisDestroyAdapterQuery(&adapter_query);

    // Create CommandQueue
    WisCommandQueue command_queue = {0};
    result = wisDeviceCreateCommandQueue(&device, WisCommandQueueTypeGraphics, &command_queue);
    REQUIRE(result.status == WisStatusOk);

    WisFence fence = {0};
    result = wisDeviceCreateFence(&device, 0, &fence);
    REQUIRE(result.status == WisStatusOk);

    WisResourceAllocator allocator = {0};
    result = wisDeviceGetResourceAllocator(&device, &allocator);
    REQUIRE(result.status == WisStatusOk);

    WisCommandAllocator command_allocator = {0};
    result = wisDeviceCreateCommandAllocator(&device, WisCommandQueueTypeGraphics, &command_allocator);
    REQUIRE(result.status == WisStatusOk);

    WisCommandList command_list = {0};
    result = wisCommandAllocatorCreateCommandList(&command_allocator, &command_list);
    REQUIRE(result.status == WisStatusOk);

    SECTION("Out of order destruction")
    {
        // Destroy command queue before command list and fence
        wisDestroyDevice(&device);
        wisDestroyCommandQueue(&command_queue);
        wisDestroyFence(&fence);
        wisDestroyResourceAllocator(&allocator);
        wisDestroyCommandList(&command_list);
        wisDestroyCommandAllocator(&command_allocator);
    }

    SECTION("In order destruction")
    {
        wisDestroyCommandList(&command_list);
        wisDestroyCommandAllocator(&command_allocator);
        wisDestroyResourceAllocator(&allocator);
        wisDestroyFence(&fence);
        wisDestroyCommandQueue(&command_queue);
        wisDestroyDevice(&device);
    }
}
