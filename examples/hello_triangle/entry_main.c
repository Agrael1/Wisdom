#include <wisdom/wisdom.h>

#include <SDL3/SDL.h>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sdl_backend_c.h>

// Export the symbols for the Agility SDK.
// This is required when linking against the Agility SDK on Windows.
WISDOM_EXPORT_AGILITY_SYMBOLS();

#define FRAMES_IN_FLIGHT 2
#define SWAPCHAIN_FRAMES 3

typedef struct FrameContext {
    WisCommandAllocator command_allocator;
    WisCommandList command_list;
    uint64_t fence_value;
} FrameContext;

typedef struct HelloTriangleApp {
    WisDevice device;
    WisCommandQueue queue;
    WisFence fence;
    uint64_t next_fence_value;

    WisSwapchain swapchain;
    WisTexture swapchain_textures[SWAPCHAIN_FRAMES];
    WisViewHeap rtv_heap;
    WisDataFormat swapchain_format;

    WisRootSignature root_signature;
    WisPipeline pipeline;

    FrameContext frames[FRAMES_IN_FLIGHT];
    uint32_t frame_index;

    uint32_t width;
    uint32_t height;
} HelloTriangleApp;

static void log_callback(WisSeverity severity, const char* message, uint64_t device, void* user_data)
{
    (void)device;
    (void)user_data;

    const char* severity_str = "UNKNOWN";
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
        break;
    }

    printf("[%s] %s\n", severity_str, message ? message : "");
}

static bool check_result(WisResult result, const char* where)
{
    if (result.status == WisStatusOk) {
        return true;
    }

    printf(
        "%s failed: %d, platform_code: %d, error: %s\n",
        where,
        result.status,
        result.platform_code,
        result.error ? result.error : "None"
    );
    return false;
}

static WisShader create_shader(const WisDevice* device, const char* basename)
{
    WisShader shader = {0};

#if defined(WISDOM_FORCE_VULKAN)
    const char* extensions[2] = {".spv", ".cso"};
#else
    const char* extensions[2] = {".cso", ".spv"};
#endif

    for (uint32_t i = 0; i < 2; ++i) {
        char filename[260];
        SDL_snprintf(filename, sizeof(filename), "%s%s", basename, extensions[i]);

        SDL_IOStream* stream = SDL_IOFromFile(filename, "rb");
        if (!stream) {
            continue;
        }

        const Sint64 file_size = SDL_GetIOSize(stream);
        if (file_size <= 0) {
            SDL_CloseIO(stream);
            continue;
        }

        uint8_t* bytes = (uint8_t*)malloc((size_t)file_size);
        if (!bytes) {
            SDL_CloseIO(stream);
            continue;
        }

        const size_t read_bytes = SDL_ReadIO(stream, bytes, (size_t)file_size);
        SDL_CloseIO(stream);
        if (read_bytes != (size_t)file_size) {
            free(bytes);
            continue;
        }

        WisResult result = wisDeviceCreateShader(device, bytes, (size_t)file_size, &shader);
        free(bytes);

        if (result.status == WisStatusOk) {
            return shader;
        }
    }

    return shader;
}

static bool init_device(HelloTriangleApp* app, WisInstance* instance, WisSurfaceView surface)
{
    WisResult result;
    WisAdapterQuery adapters = {0};
    result = wisInstanceQueryAdapters(instance, WisAdapterPreferencePerformance, &adapters);
    if (!check_result(result, "wisInstanceQueryAdapters")) {
        return false;
    }

    WisCommandQueueDesc queue_descs[] = {
        {WisCommandQueueTypeGraphics, WisCommandQueuePriorityHigh},
    };

    WisDeviceRequirements requirements = {
        .queue_descs = queue_descs,
        .queue_desc_count = 1,
    };

    const size_t adapter_count = wisAdapterQueryGetAdapterCount(&adapters);
    for (size_t i = 0; i < adapter_count; ++i) {
        if (!wisAdapterQueryGetSurfaceSupport(&adapters, i, surface)) {
            continue;
        }

        result = wisAdapterQueryCreateDevice(&adapters, i, &requirements, &app->device);
        if (result.status == WisStatusOk) {
            wisDestroyAdapterQuery(&adapters);
            return true;
        }
    }

    wisDestroyAdapterQuery(&adapters);
    return false;
}

static bool refresh_swapchain_targets(HelloTriangleApp* app)
{
    WisResult result = wisSwapchainGetTextures(&app->swapchain, app->swapchain_textures, SWAPCHAIN_FRAMES);
    if (!check_result(result, "wisSwapchainGetTextures")) {
        return false;
    }

    for (uint32_t i = 0; i < SWAPCHAIN_FRAMES; ++i) {
        WisRenderTargetDesc rtv_desc = {
            .format = app->swapchain_format,
            .layout = WisTextureLayoutTexture2D,
            .array_layer_count = 1,
        };
        wisViewHeapWriteRenderTarget(&app->rtv_heap, &app->swapchain_textures[i], &rtv_desc, i);
    }

    return true;
}

static bool init_app(HelloTriangleApp* app, SDL_Window* window)
{
    app->width = 800;
    app->height = 600;
    app->frame_index = 0;
    app->next_fence_value = 1;

    SDLPlatform platform = CreatePlatform();
    if (platform.platform_extension == NULL) {
        return false;
    }

    WisDebugDesc debug_desc = {
        .enable_debug_layer = true,
        .callback = log_callback,
    };

    WisInstance instance = {0};
    WisInstanceExtensionHeader* extensions[] = {platform.platform_extension};
    WisResult result = wisCreateInstance(&debug_desc, extensions, 1, &instance);
    if (!check_result(result, "wisCreateInstance")) {
        DestroyPlatform(&platform);
        return false;
    }

    WisSurface surface = CreateSurface(&platform, window);
    if (!init_device(app, &instance, wisGetSurfaceView(&surface))) {
        wisDestroySurface(&surface);
        wisDestroyInstance(&instance);
        DestroyPlatform(&platform);
        return false;
    }

    result = wisDeviceCreateCommandQueue(&app->device, WisCommandQueueTypeGraphics, &app->queue);
    if (!check_result(result, "wisDeviceCreateCommandQueue")) {
        wisDestroySurface(&surface);
        wisDestroyInstance(&instance);
        DestroyPlatform(&platform);
        return false;
    }

    app->swapchain_format = wisDeviceGetFormatPresentationSupport(
                                &app->device,
                                wisGetSurfaceView(&surface),
                                WisDataFormatRGB10A2Unorm
                            )
                              ? WisDataFormatRGB10A2Unorm
                              : WisDataFormatBGRA8Unorm;

    WisSwapchainDesc swapchain_desc = {
        .width = app->width,
        .height = app->height,
        .image_count = SWAPCHAIN_FRAMES,
        .texture_usage_flags = WisTextureUsageFlagsRenderTarget,
        .format = app->swapchain_format,
        .scaling = WisSwapchainScalingNone,
        .flags = WisSwapchainFlagsVSync,
        .composite_alpha = WisCompositeAlphaOpaque,
    };

    result = wisDeviceCreateSwapchain(&app->device, &surface, &app->queue, &swapchain_desc, &app->swapchain);
    wisDestroySurface(&surface);
    wisDestroyInstance(&instance);
    DestroyPlatform(&platform);

    if (!check_result(result, "wisDeviceCreateSwapchain")) {
        return false;
    }

    result = wisDeviceCreateFence(&app->device, 0, &app->fence);
    if (!check_result(result, "wisDeviceCreateFence")) {
        return false;
    }

    result = wisDeviceCreateViewHeap(&app->device, WisViewHeapTypeRenderTarget, SWAPCHAIN_FRAMES, 0, &app->rtv_heap);
    if (!check_result(result, "wisDeviceCreateViewHeap")) {
        return false;
    }

    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
        result = wisDeviceCreateCommandAllocator(
            &app->device,
            WisCommandQueueTypeGraphics,
            &app->frames[i].command_allocator
        );
        if (!check_result(result, "wisDeviceCreateCommandAllocator")) {
            return false;
        }

        result = wisCommandAllocatorCreateCommandList(&app->frames[i].command_allocator, &app->frames[i].command_list);
        if (!check_result(result, "wisCommandAllocatorCreateCommandList")) {
            return false;
        }

        app->frames[i].fence_value = 0;
    }

    if (!refresh_swapchain_targets(app)) {
        return false;
    }

    WisPushConstant push_constant = {
        .visibility = WisShaderVisibilityAll,
        .bind_register = 0,
        .bind_space = 0,
        .size_bytes = 16,
    };

    WisRootSignatureDesc root_signature_desc = {
        .push_constants = &push_constant,
        .push_constant_count = 1,
    };

    result = wisDeviceCreateRootSignature(&app->device, &root_signature_desc, &app->root_signature);
    if (!check_result(result, "wisDeviceCreateRootSignature")) {
        return false;
    }

    WisShader vertex_shader = create_shader(&app->device, "hello_triangle.vs.hlsl");
    WisShader pixel_shader = create_shader(&app->device, "hello_triangle.ps.hlsl");

    WisRenderAttachmentsDesc attachments = {0};
    attachments.attachment_formats[0] = app->swapchain_format;
    attachments.attachments_count = 1;

    WisRasterizerDesc rasterizer = {
        .fill_mode = WisFillModeSolid,
        .cull_mode = WisCullModeNone,
        .front_face = WisWindingOrderCounterClockwise,
        .depth_clip_enable = true,
    };

    WisGraphicsPipelineDesc pipeline_desc = {
        .root_signature = wisGetRootSignatureView(&app->root_signature),
        .vertex_shader = wisGetShaderView(&vertex_shader),
        .pixel_shader = wisGetShaderView(&pixel_shader),
        .render_attachments = attachments,
        .topology_type = WisTopologyTypeTriangle,
        .rasterizer_desc = &rasterizer,
        .flags = WisPipelineFlagsNone,
    };

    result = wisDeviceCreateGraphicsPipeline(&app->device, &pipeline_desc, &app->pipeline);

    wisDestroyShader(&vertex_shader);
    wisDestroyShader(&pixel_shader);

    return check_result(result, "wisDeviceCreateGraphicsPipeline");
}

static void wait_idle(HelloTriangleApp* app)
{
    WisResult result = wisCommandQueueSignalFence(&app->queue, wisGetFenceView(&app->fence), ++app->next_fence_value);
    if (result.status == WisStatusOk) {
        wisFenceWait(&app->fence, app->next_fence_value, UINT64_MAX);
    }
}

static void destroy_app(HelloTriangleApp* app)
{
    wait_idle(app);

    wisDestroyPipeline(&app->pipeline);
    wisDestroyRootSignature(&app->root_signature);

    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
        wisDestroyCommandList(&app->frames[i].command_list);
        wisDestroyCommandAllocator(&app->frames[i].command_allocator);
    }

    wisDestroyViewHeap(&app->rtv_heap);

    for (uint32_t i = 0; i < SWAPCHAIN_FRAMES; ++i) {
        wisDestroyTexture(&app->swapchain_textures[i]);
    }

    wisDestroySwapchain(&app->swapchain);
    wisDestroyFence(&app->fence);
    wisDestroyCommandQueue(&app->queue);
    wisDestroyDevice(&app->device);
}

static void draw_frame(HelloTriangleApp* app, float angle)
{
    FrameContext* frame = &app->frames[app->frame_index];
    if (frame->fence_value > 0 && wisFenceGetCompletedValue(&app->fence) < frame->fence_value) {
        wisFenceWait(&app->fence, frame->fence_value, UINT64_MAX);
    }

    wisCommandAllocatorReset(&frame->command_allocator);

    uint32_t swapchain_index = 0;
    WisResult result = wisSwapchainGetCurrentIndex(&app->swapchain, &swapchain_index);
    if (result.status != WisStatusOk) {
        return;
    }

    WisTexture* target_texture = &app->swapchain_textures[swapchain_index];
    uint64_t target_rtv = wisViewHeapGetViewAddress(&app->rtv_heap, swapchain_index);

    WisTextureBarrier barriers[] = {
        {
            .sync_before = WisBarrierSyncNone,
            .sync_after = WisBarrierSyncRenderTarget,
            .access_before = WisResourceAccessNone,
            .access_after = WisResourceAccessRenderTarget,
            .state_before = WisTextureStateUndefined,
            .state_after = WisTextureStateRenderTarget,
            .texture = wisGetTextureView(target_texture),
            .subresource_range = {0, 1, 0, 1, 0, 1},
            .queue_type_before = WisCommandQueueTypeGraphics,
            .queue_type_after = WisCommandQueueTypeGraphics,
        },
        {
            .sync_before = WisBarrierSyncRenderTarget,
            .sync_after = WisBarrierSyncNone,
            .access_before = WisResourceAccessRenderTarget,
            .access_after = WisResourceAccessNone,
            .state_before = WisTextureStateRenderTarget,
            .state_after = WisTextureStatePresent,
            .texture = wisGetTextureView(target_texture),
            .subresource_range = {0, 1, 0, 1, 0, 1},
            .queue_type_before = WisCommandQueueTypeGraphics,
            .queue_type_after = WisCommandQueueTypeGraphics,
        },
    };

    WisBarrierGroup pre_barrier = {
        .texture_barriers = barriers,
        .texture_barrier_count = 1,
    };
    WisBarrierGroup post_barrier = {
        .texture_barriers = barriers + 1,
        .texture_barrier_count = 1,
    };

    WisViewport viewport = {
        .width = (float)app->width,
        .height = (float)app->height,
        .min_depth = 0.0f,
        .max_depth = 1.0f,
    };

    WisRect scissor = {
        .x = 0,
        .y = 0,
        .width = (int32_t)app->width,
        .height = (int32_t)app->height,
    };

    const float push_data[4] = {cosf(angle), sinf(angle), 0.0f, 0.0f};
    WisPushConstantDataDesc push_constant = {
        .pipeline = WisPipelineTypeGraphics,
        .root_index = 0,
        .data = push_data,
        .data_size = 16,
        .push_offset = 0,
    };

    WisRenderPassDesc render_pass = {
        .render_targets = {{
            .target = target_rtv,
            .load_op = WisLoadOpClear,
            .store_op = WisStoreOpStore,
            .clear_value = {0.1f, 0.1f, 0.15f, 1.0f},
        }},
        .render_target_count = 1,
    };

    wisCommandListBegin(&frame->command_list);
    wisCommandListInsertBarriers(&frame->command_list, &pre_barrier);
    wisCommandListSetRootSignature(
        &frame->command_list,
        wisGetRootSignatureView(&app->root_signature),
        WisPipelineTypeGraphics
    );
    wisCommandListSetPipeline(&frame->command_list, wisGetPipelineView(&app->pipeline), WisPipelineTypeGraphics);
    wisCommandListSetPushConstants(&frame->command_list, &push_constant);
    wisCommandListSetViewports(&frame->command_list, &viewport, 1);
    wisCommandListSetScissors(&frame->command_list, &scissor, 1);
    wisCommandListSetPrimitiveTopology(&frame->command_list, WisPrimitiveTopologyTriangleList);
    wisCommandListBeginRenderPass(&frame->command_list, &render_pass);
    wisCommandListDraw(&frame->command_list, 3, 1, 0, 0);
    wisCommandListEndRenderPass(&frame->command_list);
    wisCommandListInsertBarriers(&frame->command_list, &post_barrier);
    wisCommandListEnd(&frame->command_list);

    WisCommandListView list_view = wisGetCommandListView(&frame->command_list);
    wisCommandQueueSubmit(&app->queue, &list_view, 1);
    wisSwapchainPresent(&app->swapchain, WisPresentFlagsNone, NULL, 0);

    frame->fence_value = app->next_fence_value;
    wisCommandQueueSignalFence(&app->queue, wisGetFenceView(&app->fence), app->next_fence_value);
    app->next_fence_value++;
    app->frame_index = (app->frame_index + 1) % FRAMES_IN_FLIGHT;
}

static void handle_resize(HelloTriangleApp* app, uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0) {
        return;
    }

    wait_idle(app);

    for (uint32_t i = 0; i < SWAPCHAIN_FRAMES; ++i) {
        wisDestroyTexture(&app->swapchain_textures[i]);
    }

    WisSwapchainUpdateDesc update_desc = {
        .width = width,
        .height = height,
    };

    if (wisSwapchainUpdate(&app->swapchain, &update_desc).status == WisStatusOk) {
        app->width = width;
        app->height = height;
        refresh_swapchain_targets(app);
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Wisdom Hello Triangle C", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        return 1;
    }

    HelloTriangleApp app = {0};
    if (!init_app(&app, window)) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const uint64_t start_ticks = SDL_GetTicks();
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                handle_resize(&app, (uint32_t)e.window.data1, (uint32_t)e.window.data2);
            }
        }

        const float elapsed = (float)(SDL_GetTicks() - start_ticks) / 1000.0f;
        draw_frame(&app, elapsed);
    }

    destroy_app(&app);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
