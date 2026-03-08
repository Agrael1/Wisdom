#include <wisdom/wisdom.h>
#include <stdio.h>

void LogCallback(WisSeverity severity, const char* message, uint64_t device, void* user_data)
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

typedef struct BasicRenderer {
    WisDevice device;

    // Command submission
    WisCommandQueue     gfx_queue;
    WisCommandAllocator gfx_command_allocator;
    WisCommandList      gfx_command_list;
    WisFence            fence;

    // Resources
    WisResourceAllocator allocator;
    WisDescriptorHeap    descriptor_heap;
    WisDescriptorHeap    sampler_heap;
} BasicRenderer;

typedef struct ResourceContainer {
    WisBuffer  buffer;
    WisTexture texture;
} ResourceContainer;

typedef struct BasicRenderTask {
    WisRootSignature root_signature;

} BasicRenderTask;

//------------------------------------------------------------------------------
WisDevice CreateDevice()
{
    WisDebugDesc debug_desc       = { 0 };
    debug_desc.enable_debug_layer = true;
    debug_desc.callback           = LogCallback;
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
    return device;
}

//------------------------------------------------------------------------------
void InitRenderer(BasicRenderer* renderer)
{
    renderer->device = CreateDevice();
    WisResult result = wisDeviceCreateCommandQueue(&renderer->device, WisCommandQueueTypeGraphics, &renderer->gfx_queue);
    printf("CreateCommandQueue result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDeviceGetResourceAllocator(&renderer->device, &renderer->allocator);
    printf("GetResourceAllocator result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDeviceCreateFence(&renderer->device, 0, &renderer->fence);
    printf("CreateFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDeviceCreateCommandAllocator(&renderer->device, WisCommandQueueTypeGraphics, &renderer->gfx_command_allocator);
    printf("CreateCommandAllocator result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisCommandAllocatorCreateCommandList(&renderer->gfx_command_allocator, &renderer->gfx_command_list);
    printf("CreateCommandList result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

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
    result = wisDeviceCreateDescriptorHeap(&renderer->device, &descriptor_heap_desc, &renderer->descriptor_heap);
    printf("CreateDescriptorHeap result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDeviceCreateDescriptorHeap(&renderer->device, &sampler_heap_desc, &renderer->sampler_heap);
    printf("CreateSamplerHeap result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void DestoyRenderer(BasicRenderer* renderer)
{
    wisDestroyDescriptorHeap(&renderer->descriptor_heap);
    wisDestroyDescriptorHeap(&renderer->sampler_heap);
    wisDestroyCommandList(&renderer->gfx_command_list);
    wisDestroyCommandAllocator(&renderer->gfx_command_allocator);
    wisDestroyFence(&renderer->fence);
    wisDestroyCommandQueue(&renderer->gfx_queue);
    wisDestroyDevice(&renderer->device);
    wisDestroyResourceAllocator(&renderer->allocator);
}

//------------------------------------------------------------------------------
void InitRenderTask(BasicRenderTask* task, BasicRenderer* renderer)
{
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
    WisDescriptorTableEntry descriptor_table_entries[] = {
        {
         .type              = WisDescriptorTypeConstantBuffer,
         .bind_register     = 2,
         .bind_space        = 0,
         .count             = 1,
         .descriptor_offset = 0,
         },
        {
         .type              = WisDescriptorTypeTexture,
         .bind_register     = 3,
         .bind_space        = 0,
         .count             = 1,
         .descriptor_offset = WIS_DESCRIPTOR_OFFSET_APPEND,
         }
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
         .entries     = descriptor_table_entries,
         .entry_count = 2,
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
    WisResult result = wisDeviceCreateRootSignature(&renderer->device, &root_signature_desc, &task->root_signature);
    printf("CreateRootSignature for RenderTask result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void DestroyRenderTask(BasicRenderTask* task)
{
    wisDestroyRootSignature(&task->root_signature);
}

//------------------------------------------------------------------------------
void InitResourceContainer(ResourceContainer* container, BasicRenderer* renderer)
{
    WisBufferDesc buffer_desc = {
        .size_bytes   = 1024,
        .usage_flags  = WisBufferUsageFlagsCopyDst | WisBufferUsageFlagsConstantBuffer,
        .memory_type  = WisMemoryTypeUpload,
        .memory_flags = WisMemoryFlagsMapped,
    };
    WisResult result = wisResourceAllocatorCreateBuffer(&renderer->allocator, &buffer_desc, &container->buffer);
    printf("CreateBuffer result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
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
    result = wisResourceAllocatorCreateTexture(&renderer->allocator, &texture_desc, &container->texture);
    printf("CreateTexture result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void DestroyResourceContainer(ResourceContainer* container)
{
    wisDestroyBuffer(&container->buffer);
    wisDestroyTexture(&container->texture);
}

//------------------------------------------------------------------------------
void GetDeviceProperties(const WisDevice* device)
{
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
    wisDeviceQueryProperties(device, &descriptor_heap_properties);
    printf("Device supports the following queue types:\n");
    for (int i = 0; i < 5; ++i) {
        if (command_queues_properties.supported_queues[i]) {
            printf("- Queue type %d with max priority %d\n", i, command_queues_properties.max_queue_priority[i]);
        }
    }
    printf("Relaxed queue transition support: %s\n", command_queues_properties.relaxed_queue_transition ? "Yes" : "No");

    printf("Device descriptor heap properties:\n");
    printf("- Max descriptor heap size: %u\n", descriptor_heap_properties.max_descriptor_heap_size);
    printf("- Max sampler heap size: %u\n", descriptor_heap_properties.max_sampler_heap_size);
    printf("- Max sampler heap size with embedded samplers: %u\n", descriptor_heap_properties.max_sampler_heap_size_with_embedded);
    printf("- Descriptor increment size: %u\n", descriptor_heap_properties.descriptor_increment_size);
    printf("- Sampler increment size: %u\n", descriptor_heap_properties.sampler_increment_size);

    printf("Device memory properties:\n");
    printf("- GPU upload supported: %s\n", memory_properties.gpu_upload_supported ? "Yes" : "No");
    printf("- Host image copy supported: %s\n", memory_properties.host_image_copy_supported ? "Yes" : "No");
}

//------------------------------------------------------------------------------
void BindResources(const BasicRenderer* renderer, const ResourceContainer* resources)
{
    WisSamplerDesc sampler_desc = {
        .min_filter          = WisFilterLinear,
        .mag_filter          = WisFilterLinear,
        .mip_filter          = WisFilterLinear,
        .reduction_mode      = WisReductionModeStandard,
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
        .buffer_address = wisBufferGetGPUAddress(&resources->buffer),
        .size_bytes     = 1024,
    };
    WisTextureBinding texture_binding = {
        .format = WisDataFormatRGBA8Unorm,
        .layout = WisTextureLayoutTexture2D,
        .flags  = WisTextureBindingFlagsNone,
        .range  = {
                   .base_mip_level    = 0,
                   .mip_level_count   = 1,
                   .base_array_layer  = 0,
                   .array_layer_count = 1,
                   .plane_slice       = 0,
                   }
    };
    WisResult result = wisDescriptorHeapWriteConstantBuffer(&renderer->descriptor_heap, &cb_binding, 0);
    result           = wisDescriptorHeapWriteSampler(&renderer->sampler_heap, &sampler_desc, 0);
    printf("WriteConstantBuffer result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDescriptorHeapWriteTexture(&renderer->descriptor_heap, wisGetView(&resources->texture), &texture_binding, 1);
    printf("WriteTexture result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void Render(const BasicRenderer* renderer, const ResourceContainer* resources, const BasicRenderTask* task)
{
    WisCommandListView command_list_view = wisGetView(&renderer->gfx_command_list);

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
        .buffer_address  = wisBufferGetGPUAddress(&resources->buffer),
    };
    WisDescriptorTableDataDesc descriptor_table_data_desc = {
        .pipeline    = WisPipelineTypeGraphics,
        .root_index  = 2,
        .heap_type   = WisDescriptorHeapTypeDescriptor,
        .heap_offset = 0,
    };

    WisTextureBarrier texture_barrier = {
        .sync_before   = WisBarrierSyncNone,
        .sync_after    = WisBarrierSyncPixelShading,
        .access_before = WisResourceAccessNone,
        .access_after  = WisResourceAccessShaderResource,
        .state_before  = WisTextureStateUndefined,
        .state_after   = WisTextureStateShaderResource,
        .texture       = wisGetView(&resources->texture),
        .flags         = WisBarrierFlagsWholeRange | WisBarrierFlagsDiscardContent,
    };
    WisBarrierGroup barrier_group = {
        .texture_barriers = &texture_barrier,
        .texture_barrier_count = 1
    };

    wisCommandListBegin(&renderer->gfx_command_list);
    wisCommandListInsertBarriers(&renderer->gfx_command_list, &barrier_group);

    wisCommandListSetRootSignature(&renderer->gfx_command_list, wisGetView(&task->root_signature), WisPipelineTypeGraphics);
    wisCommandListSetPushConstants(&renderer->gfx_command_list, &push_constant_data_desc);
    wisCommandListSetPushDescriptor(&renderer->gfx_command_list, &push_descriptor_data_desc);
    wisCommandListSetDescriptorHeaps(&renderer->gfx_command_list, &renderer->descriptor_heap, &renderer->sampler_heap);
    wisCommandListSetDescriptorTable(&renderer->gfx_command_list, &descriptor_table_data_desc);
    wisCommandListEnd(&renderer->gfx_command_list);

    WisResult result = wisCommandQueueSubmit(&renderer->gfx_queue, &command_list_view, 1);

    // Enqueue fence signal on command queue
    result = wisCommandQueueSignalFence(&renderer->gfx_queue, wisGetView(&renderer->fence), 1);

    result = wisFenceWait(&renderer->fence, 1, UINT64_MAX);
}

// Entry point for testing
int main()
{
    BasicRenderer renderer = { 0 };
    InitRenderer(&renderer);

    // Query and print device properties
    GetDeviceProperties(&renderer.device);

    BasicRenderTask render_task = { 0 };
    InitRenderTask(&render_task, &renderer);

    ResourceContainer resources = { 0 };
    InitResourceContainer(&resources, &renderer);

    // Bind resources to descriptor heaps
    BindResources(&renderer, &resources);

    // Execute render task
    Render(&renderer, &resources, &render_task);

    // Cleanup
    DestoyRenderer(&renderer);
    DestroyRenderTask(&render_task);
    DestroyResourceContainer(&resources);

    return 0;
}
