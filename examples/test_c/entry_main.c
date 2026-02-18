#include <wisdom/wisdom.h>
#include <stdio.h>

void log_callback(WisSeverity severity, const char* message, uint64_t device, void* user_data)
{
    const char* severity_str = "";
    switch (severity) {
    case WisSeverityVerbose:
        severity_str = "VERBOSE";
        break;
    case WisSeverityInfo:
        severity_str = "INFO";
        break;
    case WisSeverityWarning:
        severity_str = "WARNING";
        break;
    case WisSeverityError:
        severity_str = "ERROR";
        break;
    case WisSeverityFatal:
        severity_str = "FATAL";
        break;
    default:
        severity_str = "UNKNOWN";
        break;
    }
    printf("[%s] %s\n", severity_str, message);
}

// Entry point for testing
int main()
{
    WisDebugDesc debug_desc       = { 0 };
    debug_desc.enable_debug_layer = true;
    debug_desc.callback           = log_callback;
    debug_desc.user_data          = NULL;

    WisInstance instance = { 0 };
    WisResult   result   = wisCreateInstance(&debug_desc, NULL, 0, &instance);
    printf("CreateInstance result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisAdapterQuery adapter_query = { 0 };
    result                        = wisInstanceQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapter_query);
    printf("QueryAdapters result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    // Destroy instance as we no longer need it
    wisDestroyInstance(&instance);

    WisCommandQueueDesc queue_descs[] = {
        { WisCommandQueueTypeGraphics,   WisCommandQueuePriorityHigh },
        {  WisCommandQueueTypeCompute, WisCommandQueuePriorityNormal },
    };

    WisDeviceRequirements device_requirements = {
        .queue_descs      = queue_descs,
        .queue_desc_count = sizeof(queue_descs) / sizeof(queue_descs[0]),
    };

    WisDevice device        = { 0 };
    size_t    adapter_count = wisAdapterQueryGetAdapterCount(&adapter_query);
    printf("Adapter count: %zu\n", adapter_count);

    for (size_t i = 0; i < adapter_count; ++i) {
        WisAdapterDesc desc = { 0 };
        result              = wisAdapterQueryGetAdapterDesc(&adapter_query, i, &desc);
        printf("GetAdapterDesc result for adapter %zu: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");
        if (result.status == WisStatusOk) {
            printf("Adapter %zu: Name: %s, VendorID: %u, DeviceID: %u, Flags: %u\n", i, desc.description, desc.vendor_id, desc.device_id, desc.flags);
        }

        result = wisAdapterQueryCreateDevice(&adapter_query, i, &device_requirements, &device);
        printf("CreateDevice result for adapter %zu: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");
        if (result.status == WisStatusOk) {
            printf("Device created successfully for adapter %zu.\n", i);
            break; // Successfully created a device, exit loop
        }
    }
    wisDestroyAdapterQuery(&adapter_query);

    // Create CommandQueue
    WisCommandQueue command_queue = { 0 };

    result = wisDeviceCreateCommandQueue(&device, WisCommandQueueTypeGraphics, &command_queue);
    printf("CreateCommandQueue result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisFence fence = { 0 };
    result         = wisDeviceCreateFence(&device, 0, &fence);
    printf("CreateFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisResourceAllocator allocator = { 0 };
    result                         = wisDeviceCreateResourceAllocator(&device, &allocator);

    WisCommandList command_list = { 0 };
    result                      = wisDeviceCreateCommandList(&device, WisCommandQueueTypeGraphics, &command_list);

    // Out of order destruction must still work
    wisDestroyDevice(&device);
    wisDestroyCommandQueue(&command_queue);
    wisDestroyFence(&fence);
    wisDestroyResourceAllocator(&allocator);
    wisDestroyCommandList(&command_list);
    return 0;
}
