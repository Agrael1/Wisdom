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

    // Query important device features
    WisDeviceMemoryProperties memory_properties = {
        .property_type = WisQueryPropertyTypeDeviceMemoryProperties
    };
    WisDeviceCommandQueuesProperties command_queues_properties = {
        .property_type = WisQueryPropertyTypeDeviceCommandQueueProperties,
        .next_in_chain = &memory_properties
    };
    WisDeviceDescriptorHeapProperties descriptor_heap_properties = {
        .property_type = WisQueryPropertyTypeDeviceDescriptorHeapProperties,
        .next_in_chain = &command_queues_properties
    };
    wisDeviceQueryProperties(&device, &descriptor_heap_properties);
    printf("Device supports the following queue types:\n");
    for (int i = 0; i < 5; ++i) {
        if (command_queues_properties.supported_queues[i]) {
            printf("- Queue type %d with max priority %d\n", i, command_queues_properties.max_queue_priority[i]);
        }
    }

    printf("Device descriptor heap properties:\n");
    printf("- Max descriptor heap size: %zu\n", descriptor_heap_properties.max_descriptor_heap_size);
    printf("- Max sampler heap size: %zu\n", descriptor_heap_properties.max_sampler_heap_size);
    printf("- Max sampler heap size with embedded samplers: %zu\n", descriptor_heap_properties.max_sampler_heap_size_with_embedded);
    printf("- Descriptor increment size: %zu\n", descriptor_heap_properties.descriptor_increment_size);
    printf("- Sampler increment size: %zu\n", descriptor_heap_properties.sampler_increment_size);

    printf("Device memory properties:\n");
    printf("- GPU upload supported: %s\n", memory_properties.gpu_upload_supported ? "Yes" : "No");
    printf("- Host image copy supported: %s\n", memory_properties.host_image_copy_supported ? "Yes" : "No");

    // Create CommandQueue
    WisCommandQueue command_queue = { 0 };

    result = wisDeviceCreateCommandQueue(&device, WisCommandQueueTypeGraphics, &command_queue);
    printf("CreateCommandQueue result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisFence fence = { 0 };
    result         = wisDeviceCreateFence(&device, 0, &fence);
    printf("CreateFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisResourceAllocator allocator = { 0 };
    result                         = wisDeviceGetResourceAllocator(&device, &allocator);

    WisCommandAllocator command_allocator = { 0 };
    result                                = wisDeviceCreateCommandAllocator(&device, WisCommandQueueTypeGraphics, &command_allocator);

    WisCommandList command_list = { 0 };
    result                      = wisCommandAllocatorCreateCommandList(&command_allocator, &command_list);

    WisCommandListView command_list_view = wisGetView(&command_list);

    WisDescriptorHeapDesc descriptor_heap_desc = {
        .type             = WisDescriptorHeapTypeDescriptor,
        .memory_type      = WisDescriptorMemoryTypeShaderVisible,
        .descriptor_count = 100,
    };
    WisDescriptorHeap descriptor_heap = { 0 };
    result                            = wisDeviceCreateDescriptorHeap(&device, &descriptor_heap_desc, &descriptor_heap);

    WisBufferDesc buffer_desc = {
        .size_bytes   = 1024,
        .usage_flags  = WisBufferUsageFlagsCopySrc,
        .memory_type  = WisMemoryTypeUpload,
        .memory_flags = WisMemoryFlagsMapped,
    };
    WisBuffer buffer = { 0 };
    result           = wisResourceAllocatorCreateBuffer(&allocator, &buffer_desc, &buffer);
    printf("CreateBuffer result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    void* mapped_ptr = wisBufferMap(&buffer);

    WisTextureDesc texture_desc = {
        .width               = 256,
        .height              = 256,
        .depth_or_array_size = 1,
        .mip_levels          = 1,
        .format              = WisDataFormatRGBA8Unorm,
        .sample_count        = WisSampleCountS1,
        .layout              = WisTextureLayoutTexture2D,
        .usage_flags         = WisTextureUsageFlagsCopyDst | WisTextureUsageFlagsShaderResource,
        .memory_type         = WisMemoryTypeDeviceLocal,
        .memory_flags        = WisMemoryFlagsNone,
    };

    WisTexture texture = { 0 };
    result             = wisResourceAllocatorCreateTexture(&allocator, &texture_desc, &texture);
    printf("CreateTexture result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    // Enqueue fence signal on command queue
    result = wisCommandQueueSignalFence(&command_queue, wisGetView(&fence), 1);

    wisFenceWait(&fence, 1, UINT64_MAX);

    // Out of order destruction must still work
    wisDestroyDevice(&device);
    wisDestroyCommandQueue(&command_queue);
    wisDestroyFence(&fence);
    wisDestroyResourceAllocator(&allocator);
    wisDestroyCommandList(&command_list);
    wisDestroyDescriptorHeap(&descriptor_heap);
    wisDestroyBuffer(&buffer);
    wisDestroyTexture(&texture);
    wisDestroyCommandAllocator(&command_allocator);
    return 0;
}
