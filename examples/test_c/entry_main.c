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
    printf("- Max descriptor heap size: %u\n", descriptor_heap_properties.max_descriptor_heap_size);
    printf("- Max sampler heap size: %u\n", descriptor_heap_properties.max_sampler_heap_size);
    printf("- Max sampler heap size with embedded samplers: %u\n", descriptor_heap_properties.max_sampler_heap_size_with_embedded);
    printf("- Descriptor increment size: %u\n", descriptor_heap_properties.descriptor_increment_size);
    printf("- Sampler increment size: %u\n", descriptor_heap_properties.sampler_increment_size);

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
    WisDescriptorHeapDesc sampler_heap_desc = {
        .type             = WisDescriptorHeapTypeSampler,
        .memory_type      = WisDescriptorMemoryTypeShaderVisible,
        .descriptor_count = 100,
    };
    WisDescriptorHeap descriptor_heap = { 0 };
    WisDescriptorHeap sampler_heap    = { 0 };
    result                            = wisDeviceCreateDescriptorHeap(&device, &descriptor_heap_desc, &descriptor_heap);
    result                            = wisDeviceCreateDescriptorHeap(&device, &sampler_heap_desc, &sampler_heap);

    WisBufferDesc buffer_desc = {
        .size_bytes   = 1024,
        .usage_flags  = WisBufferUsageFlagsCopyDst | WisBufferUsageFlagsConstantBuffer,
        .memory_type  = WisMemoryTypeUpload,
        .memory_flags = WisMemoryFlagsMapped,
    };
    WisBuffer buffer = { 0 };
    result           = wisResourceAllocatorCreateBuffer(&allocator, &buffer_desc, &buffer);
    printf("CreateBuffer result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    uint64_t buffer_gpu_address = wisBufferGetGPUAddress(&buffer);
    void*    mapped_ptr         = wisBufferMap(&buffer);

    WisSamplerDesc sampler_desc = {
        .min_filter          = WisFilterLinear,
        .mag_filter          = WisFilterLinear,
        .mip_filter          = WisFilterLinear,
        .is_anisotropic      = false,
        .max_anisotropy      = 1,
        .address_u           = WisAddressModeRepeat,
        .address_v           = WisAddressModeRepeat,
        .address_w           = WisAddressModeRepeat,
        .min_lod             = 0.0f,
        .max_lod             = 1000.0f,
        .mip_lod_bias        = 0.0f,
        .comparison_op       = WisCompareOperationNever,
        .static_border_color = WisStaticBorderOpaqueBlack,
        .flags               = WisSamplerFlagsNone,
    };
    WisConstantBufferBinding cb_binding = {
        .buffer_address = buffer_gpu_address,
        .size_bytes     = 1024,
    };
    result = wisDescriptorHeapWriteConstantBuffer(&descriptor_heap, &cb_binding, 0);
    result = wisDescriptorHeapWriteSampler(&sampler_heap, &sampler_desc, 0);

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

    WisPushConstant push_constant = {
        .visibility    = WisShaderVisibilityAll,
        .bind_register = 0,
        .bind_space    = 0,
        .size_bytes    = 16,
    };
    WisPushDescriptor push_descriptor = {
        .visibility    = WisShaderVisibilityAll,
        .type          = WisDescriptorTypeConstantBuffer,
        .bind_register = 1,
        .bind_space    = 0,
    };
    WisDescriptorTableEntry descriptor_table_entry = {
        .type              = WisDescriptorTypeConstantBuffer,
        .bind_register     = 2,
        .bind_space        = 0,
        .count             = 1,
        .descriptor_offset = 0,
    };
    WisDescriptorTableEntry descriptor_table_entry2 = {
        .type              = WisDescriptorTypeSampler,
        .bind_register     = 0,
        .bind_space        = 0,
        .count             = 1,
        .descriptor_offset = 0,
    };
    WisDescriptorTable descriptor_tables[] = {
        {
         .visibility  = WisShaderVisibilityAll,
         .entries     = &descriptor_table_entry,
         .entry_count = 1,
         },
        {
         .visibility  = WisShaderVisibilityPixel,
         .entries     = &descriptor_table_entry2,
         .entry_count = 1,
         }
    };
    WisRootSignatureDesc root_signature_desc = {
        .push_constants         = &push_constant,
        .push_constant_count    = 1,
        .push_descriptors       = &push_descriptor,
        .push_descriptor_count  = 1,
        .descriptor_tables      = descriptor_tables,
        .descriptor_table_count = 2,
    };
    WisRootSignature root_signature = { 0 };
    result                          = wisDeviceCreateRootSignature(&device, &root_signature_desc, &root_signature);
    printf("CreateRootSignature result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    // Dummy command list

    uint32_t                push_data[4]            = { 1, 2, 3, 4 };
    WisPushConstantDataDesc push_constant_data_desc = {
        .pipeline   = WisPipelineTypeGraphics,
        .root_index = 0,
        .data       = push_data,
        .data_size  = 16,
    };
    WisPushDescriptorDataDesc push_descriptor_data_desc = {
        .pipeline        = WisPipelineTypeGraphics,
        .root_index      = 1,
        .descriptor_type = WisDescriptorTypeConstantBuffer,
        .buffer_address  = buffer_gpu_address
    };
    WisDescriptorTableDataDesc descriptor_table_data_desc = {
        .pipeline    = WisPipelineTypeGraphics,
        .root_index  = 2,
        .heap_type   = WisDescriptorHeapTypeDescriptor,
        .heap_offset = 0,
    };

    wisCommandListBegin(&command_list);
    wisCommandListSetRootSignature(&command_list, wisGetView(&root_signature), WisPipelineTypeGraphics);
    wisCommandListSetPushConstants(&command_list, &push_constant_data_desc);
    wisCommandListSetPushDescriptor(&command_list, &push_descriptor_data_desc);
    wisCommandListSetDescriptorHeaps(&command_list, &descriptor_heap, &sampler_heap);
    wisCommandListSetDescriptorTable(&command_list, &descriptor_table_data_desc);
    wisCommandListEnd(&command_list);

    wisCommandQueueSubmit(&command_queue, &command_list_view, 1);

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
    wisDestroyDescriptorHeap(&sampler_heap);
    wisDestroyBuffer(&buffer);
    wisDestroyTexture(&texture);
    wisDestroyCommandAllocator(&command_allocator);
    wisDestroyRootSignature(&root_signature);
    return 0;
}
