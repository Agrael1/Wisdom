#include "sdl_platform.h"
#include <stdio.h>
#include <stdlib.h>

#define FRAMES_IN_FLIGHT 2
#define TEST_FRAME_COUNT 120
#define PARTICLE_COUNT   256

typedef struct FrameContext {
    WisCommandAllocator command_allocator;
    WisCommandList      command_list;
    uint64_t            fence_value;
} FrameContext;

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

//------------------------------------------------------------------------------
WisShader CreateShader(const WisDevice* device, const char* filename)
{
    WisShader shader = { 0 };
    char      shader_name[256];
    size_t    filename_len = strlen(filename);
    strncpy(shader_name, filename, filename_len);

#ifdef WIS_SHADER_INTERMEDIATE_SPIRV
    // append .spv to the shader name
    strncpy(shader_name + filename_len, ".spv", 5);
#else
    // append .cso to the shader name
    strncpy(shader_name + filename_len, ".cso", 5);
#endif
    shader_name[filename_len + 5] = '\0'; // Ensure null termination

    FILE* file = fopen(shader_name, "rb");

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t code_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file contents
    uint8_t* shader_code = (uint8_t*)malloc(code_size);
    if (!shader_code) {
        printf("Failed to allocate memory for shader code.\n");
        fclose(file);
        return shader;
    }

    fread(shader_code, 1, code_size, file);
    WisResult result = wisDeviceCreateShader(device, shader_code, code_size, &shader);
    printf("CreateShader result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    free(shader_code);
    fclose(file);
    return shader;
}

//------------------------------------------------------------------------------
WisPipelineCache CreatePipelineCache(const WisDevice* device, const char* filename)
{
    WisPipelineCache pipeline_cache = { 0 };
    FILE*            file           = fopen(filename, "rb");
    if (!file) {
        printf("Pipeline cache file not found: %s. Creating an empty pipeline cache.\n", filename);
        WisResult result = wisDeviceCreatePipelineCache(device, NULL, 0, &pipeline_cache);
        printf("CreatePipelineCache result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
        return pipeline_cache;
    }

    fseek(file, 0, SEEK_END);
    size_t data_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t* cache_data = (uint8_t*)malloc(data_size);
    if (!cache_data) {
        printf("Failed to allocate memory for pipeline cache data.\n");
        fclose(file);
        return pipeline_cache;
    }

    fread(cache_data, 1, data_size, file);

    WisResult result = wisDeviceCreatePipelineCache(device, cache_data, data_size, &pipeline_cache);
    printf("CreatePipelineCache result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    if (result.status != WisStatusOk) {
        printf("Failed to create pipeline cache from file. Creating an empty pipeline cache instead.\n");
        result = wisDeviceCreatePipelineCache(device, NULL, 0, &pipeline_cache);
        printf("CreatePipelineCache (empty) result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    }

    free(cache_data);
    fclose(file);
    return pipeline_cache;
}

//------------------------------------------------------------------------------
void SavePipelineCache(const WisPipelineCache* cache, const char* filename)
{
    // Store pipeline cache in the file
    size_t cache_size = wisPipelineCacheGetSerializedSize(cache);
    if (!cache_size) {
        printf("Pipeline cache serialization failed or cache is empty.\n");
        return;
    }

    uint8_t* cache_data = (uint8_t*)malloc(cache_size);
    if (!cache_data) {
        return;
    }

    WisResult result = wisPipelineCacheSerialize(cache, cache_data, cache_size);
    printf("SerializePipelineCache result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    FILE* file = fopen("pipeline_cache.bin", "wb");
    if (file) {
        fwrite(cache_data, 1, cache_size, file);
        fclose(file);
        free(cache_data);
    } else {
        printf("Failed to open file for writing pipeline cache.\n");
        free(cache_data);
    }
}

typedef struct BasicRenderer {
    WisDevice device;

    // Command submission
    WisCommandQueue gfx_queue;
    WisFence        fence;
    FrameContext    frames[FRAMES_IN_FLIGHT];
    uint32_t        frame_index;
    uint64_t        next_fence_value;

    // Resources
    WisResourceAllocator allocator;
    WisDescriptorHeap    descriptor_heap;
    WisDescriptorHeap    sampler_heap;
} BasicRenderer;

typedef struct ResourceContainer {
    WisBuffer particle_buffer;
    WisBuffer frame_constants;
} ResourceContainer;

typedef struct BasicRenderTask {
    WisRootSignature root_signature;
    WisRootSignature compute_signature;
    WisPipeline      compute_pipeline;
    WisPipeline      graphics_pipeline;
    WisViewHeap      rtv_heap;
} BasicRenderTask;

//------------------------------------------------------------------------------
WisDevice CreateDevice(SDL_Window* window, const WisInstance* instance, WisSurfaceView surface)
{
    WisAdapterQuery adapter_query = { 0 };
    WisResult       result        = wisInstanceQueryAdapters(instance, WisAdapterPreferencePerformance, &adapter_query);
    printf("QueryAdapters result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

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

        // Check if adapter supports presentation to the surface
        if (wisAdapterQueryGetSurfaceSupport(&adapter_query, i, surface)) {
            printf("Adapter %zu supports presentation to the surface.\n", i);
        } else {
            printf("Adapter %zu does NOT support presentation to the surface. Skipping.\n", i);
            continue; // Skip this adapter and try the next one
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
void InitRenderer(BasicRenderer* renderer, SDL_Window* window)
{
    WisDebugDesc debug_desc       = { 0 };
    debug_desc.enable_debug_layer = true;
    debug_desc.callback           = LogCallback;
    debug_desc.user_data          = NULL;

    SDLPlatform                 platform     = CreatePlatform();
    WisInstanceExtensionHeader* extensions[] = {
        platform.platform_extension
    };
    WisInstance instance = { 0 };
    WisResult   result   = wisCreateInstance(&debug_desc, extensions, sizeof(extensions) / sizeof(WisInstanceExtensionHeader*), &instance);
    printf("CreateInstance result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisSurface surface = CreateSurface(&platform, window);

    renderer->device           = CreateDevice(window, &instance, wisGetSurfaceView(&surface));
    renderer->frame_index      = 0;
    renderer->next_fence_value = 1;

    // TODO: Create swapchain

    // Destroy instance as we no longer need it
    wisDestroySurface(&surface);
    wisDestroyInstance(&instance);
    DestroyPlatform(&platform);

    result = wisDeviceCreateCommandQueue(&renderer->device, WisCommandQueueTypeGraphics, &renderer->gfx_queue);
    printf("CreateCommandQueue result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDeviceGetResourceAllocator(&renderer->device, &renderer->allocator);
    printf("GetResourceAllocator result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisDeviceCreateFence(&renderer->device, 0, &renderer->fence);
    printf("CreateFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
        result = wisDeviceCreateCommandAllocator(&renderer->device, WisCommandQueueTypeGraphics, &renderer->frames[i].command_allocator);
        printf("CreateCommandAllocator[%u] result: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");

        result = wisCommandAllocatorCreateCommandList(&renderer->frames[i].command_allocator, &renderer->frames[i].command_list);
        printf("CreateCommandList[%u] result: %d, platform_code: %d, error: %s\n", i, result.status, result.platform_code, result.error ? result.error : "None");

        renderer->frames[i].fence_value = 0;
    }

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
    if (renderer->next_fence_value > 0) {
        WisResult result = wisCommandQueueSignalFence(&renderer->gfx_queue, wisGetFenceView(&renderer->fence), renderer->next_fence_value);
        printf("Flush SignalFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

        result = wisFenceWait(&renderer->fence, renderer->next_fence_value, UINT64_MAX);
        printf("Flush FenceWait result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    }

    wisDestroyDescriptorHeap(&renderer->descriptor_heap);
    wisDestroyDescriptorHeap(&renderer->sampler_heap);

    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
        wisDestroyCommandList(&renderer->frames[i].command_list);
        wisDestroyCommandAllocator(&renderer->frames[i].command_allocator);
    }

    wisDestroyFence(&renderer->fence);
    wisDestroyCommandQueue(&renderer->gfx_queue);
    wisDestroyResourceAllocator(&renderer->allocator);
    wisDestroyDevice(&renderer->device);
}

//------------------------------------------------------------------------------
void WaitForFinish(BasicRenderer* renderer)
{
    WisResult result = wisCommandQueueSignalFence(&renderer->gfx_queue, wisGetFenceView(&renderer->fence), renderer->next_fence_value);
    printf("WaitForFinish SignalFence result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
    result = wisFenceWait(&renderer->fence, renderer->next_fence_value, UINT64_MAX);
    printf("WaitForFinish FenceWait result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void InitRenderTask(BasicRenderTask* task, BasicRenderer* renderer)
{
    // Compute root signature
    WisPushConstant compute_push_constant = {
        .visibility    = WisShaderVisibilityAll,
        .bind_register = 0,
        .bind_space    = 0,
        .size_bytes    = 16,
    };
    WisPushDescriptor compute_push_descriptor = {
        .visibility    = WisShaderVisibilityAll,
        .type          = WisDescriptorTypeRWBuffer,
        .bind_register = 0,
        .bind_space    = 0,
    };
    WisRootSignatureDesc compute_root_signature_desc = {
        .push_constants        = &compute_push_constant,
        .push_constant_count   = 1,
        .push_descriptors      = &compute_push_descriptor,
        .push_descriptor_count = 1,
    };
    WisResult result = wisDeviceCreateRootSignature(&renderer->device, &compute_root_signature_desc, &task->compute_signature);
    printf("CreateRootSignature for ComputeShader result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    // Graphics root signature
    WisPushConstant push_constant = {
        .visibility    = WisShaderVisibilityAll,
        .bind_register = 0,
        .bind_space    = 0,
        .size_bytes    = 16,
    };
    WisPushDescriptor push_descriptor = {
        .visibility    = WisShaderVisibilityAll,
        .type          = WisDescriptorTypeBuffer,
        .bind_register = 0,
        .bind_space    = 0,
    };
    WisRootSignatureDesc root_signature_desc = {
        .push_constants        = &push_constant,
        .push_constant_count   = 1,
        .push_descriptors      = &push_descriptor,
        .push_descriptor_count = 1,
    };
    result = wisDeviceCreateRootSignature(&renderer->device, &root_signature_desc, &task->root_signature);
    printf("CreateRootSignature for RenderTask result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisPipelineCache pipeline_cache = CreatePipelineCache(&renderer->device, "pipeline_cache.bin");

    WisShader vertex_shader  = CreateShader(&renderer->device, "basic.vs.hlsl");
    WisShader pixel_shader   = CreateShader(&renderer->device, "basic.ps.hlsl");
    WisShader compute_shader = CreateShader(&renderer->device, "basic.cs.hlsl");

    WisComputePipelineDesc compute_pipeline_desc = {
        .root_signature = wisGetRootSignatureView(&task->compute_signature),
        .compute_shader = wisGetShaderView(&compute_shader),
        .cache          = wisGetPipelineCacheView(&pipeline_cache),
        .flags          = WisPipelineFlagsNone,
    };
    result = wisDeviceCreateComputePipeline(&renderer->device, &compute_pipeline_desc, &task->compute_pipeline);
    printf("CreateComputePipeline result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisRenderAttachmentsDesc attachments = { 0 };
    attachments.attachment_formats[0]    = WisDataFormatRGBA8Unorm;
    attachments.attachments_count        = 1;
    attachments.depth_attachment         = WisDataFormatD24UnormS8Uint;

    WisRasterizerDesc rasterizer = {
        .fill_mode         = WisFillModeSolid,
        .cull_mode         = WisCullModeBack,
        .front_face        = WisWindingOrderClockwise,
        .depth_clip_enable = true,
    };
    WisDepthStencilDesc depth_stencil = {
        .depth_enable       = true,
        .depth_write_enable = true,
        .depth_comp         = WisCompareOpLessEqual,
        .stencil_enable     = false,
    };
    WisBlendAttachmentDesc blend_attachment = {
        .blend_enable     = true,
        .src_color_blend  = WisBlendFactorSrcAlpha,
        .dst_color_blend  = WisBlendFactorInvSrcAlpha,
        .color_blend_op   = WisBlendOpAdd,
        .src_alpha_blend  = WisBlendFactorOne,
        .dst_alpha_blend  = WisBlendFactorZero,
        .alpha_blend_op   = WisBlendOpAdd,
        .color_write_mask = WisColorComponentsAll,
    };

    WisGraphicsPipelineDesc graphics_pipeline_desc = {
        .root_signature     = wisGetRootSignatureView(&task->root_signature),
        .vertex_shader      = wisGetShaderView(&vertex_shader),
        .pixel_shader       = wisGetShaderView(&pixel_shader),
        .render_attachments = attachments,
        .topology_type      = WisTopologyTypeTriangle,
        .rasterizer_desc    = &rasterizer,
        .depth_stencil_desc = &depth_stencil,
        .cache              = wisGetPipelineCacheView(&pipeline_cache),
        .flags              = WisPipelineFlagsNone,
    };
    result = wisDeviceCreateGraphicsPipeline(&renderer->device, &graphics_pipeline_desc, &task->graphics_pipeline);
    printf("CreateGraphicsPipeline result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    SavePipelineCache(&pipeline_cache, "pipeline_cache.bin");

    wisDestroyPipelineCache(&pipeline_cache);
    wisDestroyShader(&vertex_shader);
    wisDestroyShader(&pixel_shader);
    wisDestroyShader(&compute_shader);

    result = wisDeviceCreateViewHeap(&renderer->device, WisViewHeapTypeRenderTarget, 10, &task->rtv_heap);
    printf("CreateViewHeap result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void DestroyRenderTask(BasicRenderTask* task)
{
    wisDestroyRootSignature(&task->root_signature);
    wisDestroyRootSignature(&task->compute_signature);
    wisDestroyPipeline(&task->compute_pipeline);
    wisDestroyPipeline(&task->graphics_pipeline);
    wisDestroyViewHeap(&task->rtv_heap);
}

//------------------------------------------------------------------------------
void InitResourceContainer(ResourceContainer* container, BasicRenderer* renderer)
{
    WisBufferDesc particle_buffer_desc = {
        .size_bytes  = PARTICLE_COUNT * sizeof(float) * 8,
        .usage_flags = WisBufferUsageFlagsStorageBuffer,
        .memory_type = WisMemoryTypeDeviceLocal,
    };
    WisResult result = wisResourceAllocatorCreateBuffer(&renderer->allocator, &particle_buffer_desc, &container->particle_buffer);
    printf("Create ParticleBuffer result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");

    WisBufferDesc constants_desc = {
        .size_bytes   = 256,
        .usage_flags  = WisBufferUsageFlagsCopyDst | WisBufferUsageFlagsConstantBuffer,
        .memory_type  = WisMemoryTypeUpload,
        .memory_flags = WisMemoryFlagsMapped,
    };
    result = wisResourceAllocatorCreateBuffer(&renderer->allocator, &constants_desc, &container->frame_constants);
    printf("CreateFrameConstants result: %d, platform_code: %d, error: %s\n", result.status, result.platform_code, result.error ? result.error : "None");
}

//------------------------------------------------------------------------------
void DestroyResourceContainer(ResourceContainer* container)
{
    wisDestroyBuffer(&container->particle_buffer);
    wisDestroyBuffer(&container->frame_constants);
}

//------------------------------------------------------------------------------
void GetDeviceProperties(const WisDevice* device)
{
    // Query important device features
    WisDeviceMemoryProperties memory_properties = {
        .property_type = WisQueryPropertyTypeDeviceMemoryProperties
    };
    WisDeviceCommandQueueProperties command_queues_properties = {
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
    (void)renderer;
    (void)resources;
    // Placeholder for future descriptor table writes (textures/materials/samplers).
    // Current particle skeleton uses push descriptors and push constants only.
}

//------------------------------------------------------------------------------
void Render(BasicRenderer* renderer, const ResourceContainer* resources, const BasicRenderTask* task)
{
    FrameContext* frame = &renderer->frames[renderer->frame_index];

    if (frame->fence_value > 0) {
        uint64_t completed = wisFenceGetCompletedValue(&renderer->fence);
        if (completed < frame->fence_value) {
            WisResult result = wisFenceWait(&renderer->fence, frame->fence_value, UINT64_MAX);
            printf("Frame[%u] FenceWait result: %d, platform_code: %d, error: %s\n", renderer->frame_index, result.status, result.platform_code, result.error ? result.error : "None");
        }
    }

    WisResult result = wisCommandAllocatorReset(&frame->command_allocator);
    printf("Frame[%u] CommandAllocatorReset result: %d, platform_code: %d, error: %s\n", renderer->frame_index, result.status, result.platform_code, result.error ? result.error : "None");

    WisCommandListView command_list_view = wisGetCommandListView(&frame->command_list);

    // Record frame skeleton: compute updates particles, graphics draws them.
    uint32_t frame_number = (uint32_t)(renderer->next_fence_value - 1);

    uint32_t                compute_params[4]      = { PARTICLE_COUNT, frame_number, 16, 0 };
    WisPushConstantDataDesc compute_constants_desc = {
        .pipeline    = WisPipelineTypeCompute,
        .root_index  = 0,
        .data        = compute_params,
        .data_size   = 16,
        .push_offset = 0,
    };
    WisPushDescriptorDataDesc compute_push_descriptor_desc = {
        .pipeline        = WisPipelineTypeCompute,
        .root_index      = 1,
        .descriptor_type = WisDescriptorTypeRWBuffer,
        .buffer_address  = wisBufferGetGPUAddress(&resources->particle_buffer),
    };

    uint32_t                push_data[4]            = { frame_number, PARTICLE_COUNT, 3, 0 };
    WisPushConstantDataDesc push_constant_data_desc = {
        .pipeline    = WisPipelineTypeGraphics,
        .root_index  = 0,
        .data        = push_data,
        .data_size   = 16,
        .push_offset = 0,
    };
    WisPushDescriptorDataDesc push_descriptor_data_desc = {
        .pipeline        = WisPipelineTypeGraphics,
        .root_index      = 1,
        .descriptor_type = WisDescriptorTypeBuffer,
        .buffer_address  = wisBufferGetGPUAddress(&resources->particle_buffer),
    };

    WisBufferBarrier particle_barrier = {
        .sync_before       = WisBarrierSyncCompute,
        .sync_after        = WisBarrierSyncAllShading,
        .access_before     = WisResourceAccessNone,
        .access_after      = WisResourceAccessShaderResource,
        .buffer            = wisGetBufferView(&resources->particle_buffer),
        .offset            = 0,
        .size              = WIS_WHOLE_SIZE,
        .queue_type_before = WisCommandQueueTypeGraphics,
        .queue_type_after  = WisCommandQueueTypeGraphics,
    };
    WisBarrierGroup barrier_group = {
        .buffer_barriers      = &particle_barrier,
        .buffer_barrier_count = 1
    };

    WisViewport viewport = { .width = 800.0f, .height = 600.0f, .min_depth = 0.0f, .max_depth = 1.0f };
    WisScissor  scissor  = { .left = 0, .top = 0, .right = 800, .bottom = 600 };

    result = wisCommandListBegin(&frame->command_list);

    wisCommandListSetRootSignature(&frame->command_list, wisGetRootSignatureView(&task->compute_signature), WisPipelineTypeCompute);
    wisCommandListSetPipeline(&frame->command_list, wisGetPipelineView(&task->compute_pipeline), WisPipelineTypeCompute);
    wisCommandListSetPushConstants(&frame->command_list, &compute_constants_desc);
    wisCommandListSetPushDescriptor(&frame->command_list, &compute_push_descriptor_desc);
    wisCommandListDispatch(&frame->command_list, PARTICLE_COUNT, 1, 1);

    wisCommandListInsertBarriers(&frame->command_list, &barrier_group);

    wisCommandListSetRootSignature(&frame->command_list, wisGetRootSignatureView(&task->root_signature), WisPipelineTypeGraphics);
    wisCommandListSetPipeline(&frame->command_list, wisGetPipelineView(&task->graphics_pipeline), WisPipelineTypeGraphics);
    wisCommandListSetPushConstants(&frame->command_list, &push_constant_data_desc);
    wisCommandListSetPushDescriptor(&frame->command_list, &push_descriptor_data_desc);

    wisCommandListSetViewports(&frame->command_list, &viewport, 1);
    wisCommandListSetScissors(&frame->command_list, &scissor, 1);
    wisCommandListSetPrimitiveTopology(&frame->command_list, WisPrimitiveTopologyTriangleList);
    // TODO: Begin render pass with swapchain color target + depth attachment.
    // TODO: Issue draw call for PARTICLE_COUNT * 3 vertices (triangle per particle).
    // TODO: End render pass and present the swapchain image.

    result = wisCommandListEnd(&frame->command_list);

    result = wisCommandQueueSubmit(&renderer->gfx_queue, &command_list_view, 1);
    printf("Frame[%u] QueueSubmit result: %d, platform_code: %d, error: %s\n", renderer->frame_index, result.status, result.platform_code, result.error ? result.error : "None");

    frame->fence_value = renderer->next_fence_value;
    result             = wisCommandQueueSignalFence(&renderer->gfx_queue, wisGetFenceView(&renderer->fence), renderer->next_fence_value);
    printf("Frame[%u] SignalFence result: %d, platform_code: %d, error: %s\n", renderer->frame_index, result.status, result.platform_code, result.error ? result.error : "None");
    renderer->next_fence_value++;

    renderer->frame_index = (renderer->frame_index + 1) % FRAMES_IN_FLIGHT;
}

// Entry point for testing
int main()
{
    // init SDL and create a window here
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Wisdom Renderer Test", 800, 600, SDL_WINDOW_RESIZABLE);

    BasicRenderer renderer = { 0 };
    InitRenderer(&renderer, window);

    // Query and print device properties
    GetDeviceProperties(&renderer.device);

    BasicRenderTask render_task = { 0 };
    InitRenderTask(&render_task, &renderer);

    ResourceContainer resources = { 0 };
    InitResourceContainer(&resources, &renderer);

    // Bind resources to descriptor heaps
    BindResources(&renderer, &resources);

    // Execute render task
    for (uint32_t i = 0; i < TEST_FRAME_COUNT; ++i) {
        Render(&renderer, &resources, &render_task);
    }
    // Wait for GPU to finish before exiting
    WaitForFinish(&renderer);

    // Cleanup
    DestroyRenderTask(&render_task);
    DestroyResourceContainer(&resources);
    DestoyRenderer(&renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
