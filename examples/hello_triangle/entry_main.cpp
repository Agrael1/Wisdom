#include <wisdom/wisdom.hpp>

#include <SDL3/SDL.h>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include <sdl_backend_cpp.h>

#define FRAMES_IN_FLIGHT 2
#define SWAPCHAIN_FRAMES 3

struct FrameContext {
    wis::CommandAllocator command_allocator{};
    wis::CommandList command_list{};
    uint64_t fence_value = 0;
};

struct HelloTriangleApp {
    wis::Device device{};
    wis::CommandQueue queue{};
    wis::Fence fence{};
    uint64_t next_fence_value = 1;

    wis::Swapchain swapchain{};
    wis::Texture swapchain_textures[SWAPCHAIN_FRAMES]{};
    wis::ViewHeap rtv_heap{};
    wis::DataFormat swapchain_format = wis::DataFormat::BGRA8Unorm;

    wis::RootSignature root_signature{};
    wis::Pipeline pipeline{};

    FrameContext frames[FRAMES_IN_FLIGHT]{};
    uint32_t frame_index = 0;

    uint32_t width = 800;
    uint32_t height = 600;
};

static void log_callback(wis::Severity severity, const char* message, uint64_t, void*)
{
    const char* severity_str = "UNKNOWN";
    switch (severity) {
    case wis::Severity::Verbose:
        severity_str = "VERBOSE";
        break;
    case wis::Severity::Info:
        severity_str = "INFO";
        break;
    case wis::Severity::Warning:
        severity_str = "WARNING";
        break;
    case wis::Severity::Error:
        severity_str = "ERROR";
        break;
    case wis::Severity::Fatal:
        severity_str = "FATAL";
        break;
    default:
        break;
    }

    std::printf("[%s] %s\n", severity_str, message ? message : "");
}

static bool check_result(wis::Result result, const char* where)
{
    if (result.status == wis::Status::Ok) {
        return true;
    }

    std::printf(
        "%s failed: %d, platform_code: %d, error: %s\n",
        where,
        static_cast<int>(result.status),
        result.platform_code,
        result.error ? result.error : "None"
    );
    return false;
}

static wis::Shader create_shader(const wis::Device* device, std::string_view basename)
{
    wis::Shader shader{};

    constexpr const char* extension = wis::shader_intermediate == wis::ShaderIntermediate::SPIRV ? ".spv" : ".cso";

    std::string filename;
    filename.reserve(basename.size() + 8);
    filename.append(basename);
    filename.append(extension);

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return shader;
    }

    std::vector<uint8_t> bytes{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
    if (bytes.empty()) {
        return shader;
    }

    wis::Result result;
    shader = device->CreateShader({bytes.data(), bytes.size()}, result);
    if (result.status == wis::Status::Ok) {
        return shader;
    }

    printf(
        "Failed to create shader from file %s: %d, platform_code: %d, error: %s\n",
        filename.c_str(),
        result.status,
        result.platform_code,
        result.error ? result.error : "None"
    );

    return shader;
}

static bool init_device(HelloTriangleApp* app, wis::Instance* instance, wis::SurfaceView surface)
{
    wis::Result result;
    wis::AdapterQuery adapters = instance->QueryAdapters(wis::AdapterPreference::Performance, result);
    if (!check_result(result, "QueryAdapters")) {
        return false;
    }

    wis::CommandQueueDesc queue_descs[] = {
        {wis::CommandQueueType::Graphics, wis::CommandQueuePriority::High},
    };

    wis::DeviceRequirements requirements{};
    requirements.queue_descs = {queue_descs, 1};

    const size_t adapter_count = adapters.GetAdapterCount();
    for (size_t i = 0; i < adapter_count; ++i) {
        if (!adapters.GetSurfaceSupport(i, surface)) {
            continue;
        }

        app->device = adapters.CreateDevice(i, requirements, result);
        if (result.status == wis::Status::Ok) {
            return true;
        }
    }

    return false;
}

static bool refresh_swapchain_targets(HelloTriangleApp* app)
{
    auto result = app->swapchain.GetTextures({app->swapchain_textures, SWAPCHAIN_FRAMES});
    if (!check_result(result, "Swapchain::GetTextures")) {
        return false;
    }

    for (uint32_t i = 0; i < SWAPCHAIN_FRAMES; ++i) {
        wis::RenderTargetDesc rtv_desc = {
            .format = app->swapchain_format,
            .layout = wis::TextureLayout::Texture2D,
            .array_layer_count = 1,
        };
        std::ignore = app->rtv_heap.WriteRenderTarget(app->swapchain_textures[i], rtv_desc, i);
    }

    return true;
}

static bool init_app(HelloTriangleApp* app, SDL_Window* window)
{
    SDLPlatformCpp platform;
    if (!platform.Init()) {
        return false;
    }

    wis::DebugDesc debug_desc = {
        .enable_debug_layer = true,
        .callback = log_callback,
    };

    wis::InstanceExtensionHeader* extensions[] = {platform.Extension()};
    wis::Result result;
    wis::Instance instance = wis::CreateInstance(&debug_desc, wis::span{extensions}, result);
    if (!check_result(result, "CreateInstance")) {
        return false;
    }

    wis::Surface surface = platform.CreateWindowSurface(window);
    if (!init_device(app, &instance, surface.GetView())) {
        return false;
    }

    app->queue = app->device.CreateCommandQueue(wis::CommandQueueType::Graphics, result);
    if (!check_result(result, "Device::CreateCommandQueue")) {
        return false;
    }

    app->swapchain_format = app->device.GetFormatPresentationSupport(surface.GetView(), wis::DataFormat::RGB10A2Unorm)
                              ? wis::DataFormat::RGB10A2Unorm
                              : wis::DataFormat::BGRA8Unorm;

    wis::SwapchainDesc swapchain_desc = {
        .width = app->width,
        .height = app->height,
        .image_count = SWAPCHAIN_FRAMES,
        .texture_usage_flags = wis::TextureUsageFlags::RenderTarget,
        .format = app->swapchain_format,
        .scaling = wis::SwapchainScaling::None,
        .flags = wis::SwapchainFlags::VSync,
        .composite_alpha = wis::CompositeAlpha::Opaque,
    };

    app->swapchain = app->device.CreateSwapchain(surface, app->queue, swapchain_desc, result);
    if (!check_result(result, "Device::CreateSwapchain")) {
        return false;
    }

    app->fence = app->device.CreateFence(0, result);
    if (!check_result(result, "Device::CreateFence")) {
        return false;
    }

    app->rtv_heap = app->device.CreateViewHeap(wis::ViewHeapType::RenderTarget, SWAPCHAIN_FRAMES, {}, result);
    if (!check_result(result, "Device::CreateViewHeap")) {
        return false;
    }

    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
        app->frames[i].command_allocator = app->device.CreateCommandAllocator(wis::CommandQueueType::Graphics, result);
        if (!check_result(result, "Device::CreateCommandAllocator")) {
            return false;
        }

        app->frames[i].command_list = app->frames[i].command_allocator.CreateCommandList(result);
        if (!check_result(result, "CommandAllocator::CreateCommandList")) {
            return false;
        }
    }

    if (!refresh_swapchain_targets(app)) {
        return false;
    }

    wis::PushConstant push_constant{};
    push_constant.visibility = wis::ShaderVisibility::All;
    push_constant.bind_register = 0;
    push_constant.bind_space = 0;
    push_constant.size_bytes = 16;

    wis::RootSignatureDesc root_signature_desc{};
    root_signature_desc.push_constants = {&push_constant, 1};

    app->root_signature = app->device.CreateRootSignature(root_signature_desc, result);
    if (!check_result(result, "Device::CreateRootSignature")) {
        return false;
    }

    wis::Shader vertex_shader = create_shader(&app->device, "hello_triangle.vs.hlsl");
    wis::Shader pixel_shader = create_shader(&app->device, "hello_triangle.ps.hlsl");

    wis::RenderAttachmentsDesc attachments = {};
    attachments.attachment_formats[0] = app->swapchain_format;
    attachments.attachments_count = 1;

    wis::RasterizerDesc rasterizer = {
        .fill_mode = wis::FillMode::Solid,
        .cull_mode = wis::CullMode::None,
        .front_face = wis::WindingOrder::CounterClockwise,
        .depth_clip_enable = true,
    };

    wis::GraphicsPipelineDesc pipeline_desc = {
        .root_signature = app->root_signature.GetView(),
        .vertex_shader = vertex_shader.GetView(),
        .pixel_shader = pixel_shader.GetView(),
        .render_attachments = attachments,
        .topology_type = wis::TopologyType::Triangle,
        .rasterizer_desc = &rasterizer,
        .flags = wis::PipelineFlags::None,
    };

    app->pipeline = app->device.CreateGraphicsPipeline(pipeline_desc, result);

    return check_result(result, "Device::CreateGraphicsPipeline");
}

static void wait_idle(HelloTriangleApp* app)
{
    auto result = app->queue.SignalFence(app->fence.GetView(), ++app->next_fence_value);
    if (result.status == wis::Status::Ok) {
        result = app->fence.Wait(app->next_fence_value, UINT64_MAX);
    }
}

static void destroy_app(HelloTriangleApp* app)
{
    wait_idle(app);

    app->pipeline = {};
    app->root_signature = {};
    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
        app->frames[i].command_list = {};
        app->frames[i].command_allocator = {};
    }
    app->rtv_heap = {};
    for (auto& tex : app->swapchain_textures) {
        tex = {};
    }
    app->swapchain = {};
    app->fence = {};
    app->queue = {};
    app->device = {};
}

static void draw_frame(HelloTriangleApp* app, float angle)
{
    wis::Result result;
    FrameContext* frame = &app->frames[app->frame_index];
    if (frame->fence_value > 0 && app->fence.GetCompletedValue() < frame->fence_value) {
        result = app->fence.Wait(frame->fence_value, UINT64_MAX);
    }

    result = frame->command_allocator.Reset();

    uint32_t swapchain_index = app->swapchain.GetCurrentIndex(result);
    if (result.status != wis::Status::Ok) {
        return;
    }

    wis::Texture& target_texture = app->swapchain_textures[swapchain_index];
    uint64_t target_rtv = app->rtv_heap.GetViewAddress(swapchain_index);

    wis::TextureBarrier barriers[2]{};
    barriers[0].sync_before = wis::BarrierSync::None;
    barriers[0].sync_after = wis::BarrierSync::RenderTarget;
    barriers[0].access_before = wis::ResourceAccess::None;
    barriers[0].access_after = wis::ResourceAccess::RenderTarget;
    barriers[0].state_before = wis::TextureState::Undefined;
    barriers[0].state_after = wis::TextureState::RenderTarget;
    barriers[0].texture = target_texture.GetView();
    barriers[0].subresource_range = {0, 1, 0, 1, 0, 1};
    barriers[0].queue_type_before = wis::CommandQueueType::Graphics;
    barriers[0].queue_type_after = wis::CommandQueueType::Graphics;

    barriers[1].sync_before = wis::BarrierSync::RenderTarget;
    barriers[1].sync_after = wis::BarrierSync::None;
    barriers[1].access_before = wis::ResourceAccess::RenderTarget;
    barriers[1].access_after = wis::ResourceAccess::None;
    barriers[1].state_before = wis::TextureState::RenderTarget;
    barriers[1].state_after = wis::TextureState::Present;
    barriers[1].texture = target_texture.GetView();
    barriers[1].subresource_range = {0, 1, 0, 1, 0, 1};
    barriers[1].queue_type_before = wis::CommandQueueType::Graphics;
    barriers[1].queue_type_after = wis::CommandQueueType::Graphics;

    wis::BarrierGroup pre_barrier{};
    pre_barrier.texture_barriers = {barriers, 1};
    wis::BarrierGroup post_barrier{};
    post_barrier.texture_barriers = {barriers + 1, 1};

    wis::Viewport viewport = {
        .width = (float)app->width,
        .height = (float)app->height,
        .min_depth = 0.0f,
        .max_depth = 1.0f,
    };

    wis::Rect scissor = {
        .x = 0,
        .y = 0,
        .width = app->width,
        .height = app->height,
    };

    const float push_data[4] = {std::cos(angle), std::sin(angle), 0.0f, 0.0f};
    wis::PushConstantDataDesc push_constant = {
        .pipeline = wis::PipelineType::Graphics,
        .root_index = 0,
        .data = push_data,
        .data_size = 16,
        .push_offset = 0,
    };

    wis::RenderPassDesc render_pass{};
    render_pass.render_targets[0] = {
        .target = target_rtv,
        .load_op = wis::LoadOp::Clear,
        .store_op = wis::StoreOp::Store,
        .clear_value = {0.1f, 0.1f, 0.15f, 1.0f},
    };
    render_pass.render_target_count = 1;
    render_pass.flags = wis::RenderPassFlags::None;

    result = frame->command_list.Begin();
    frame->command_list.InsertBarriers(pre_barrier);
    frame->command_list.SetRootSignature(app->root_signature.GetView(), wis::PipelineType::Graphics);
    frame->command_list.SetPipeline(app->pipeline.GetView(), wis::PipelineType::Graphics);
    frame->command_list.SetPushConstants(push_constant);
    frame->command_list.SetViewports({&viewport, 1});
    frame->command_list.SetScissors({&scissor, 1});
    frame->command_list.SetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
    frame->command_list.BeginRenderPass(render_pass);
    frame->command_list.Draw(3, 1, 0, 0);
    frame->command_list.EndRenderPass();
    frame->command_list.InsertBarriers(post_barrier);
    result = frame->command_list.End();

    wis::CommandListView list_view = frame->command_list.GetView();
    result = app->queue.Submit({&list_view, 1});
    result = app->swapchain.Present(wis::PresentFlags::None, {});

    frame->fence_value = app->next_fence_value;
    result = app->queue.SignalFence(app->fence.GetView(), app->next_fence_value);
    app->next_fence_value++;
    app->frame_index = (app->frame_index + 1) % FRAMES_IN_FLIGHT;
}

static void handle_resize(HelloTriangleApp* app, uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0) {
        return;
    }

    wait_idle(app);

    for (auto& tex : app->swapchain_textures) {
        tex = {};
    }

    wis::SwapchainUpdateDesc update_desc = {
        .width = width,
        .height = height,
    };

    if (app->swapchain.Update(update_desc).status == wis::Status::Ok) {
        app->width = width;
        app->height = height;
        refresh_swapchain_targets(app);
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Wisdom Hello Triangle C++", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        return 1;
    }

    HelloTriangleApp app{};
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
