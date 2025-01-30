#include <wis_helper.h>
#include <wis_swapchain.h>
#include <window.h>
#include <iostream>

#include <glm/vec3.hpp>
#include <optional>

class App
{
    static constexpr uint32_t kMonCount = 2;
    ex::PlatformExtension platform;
    std::optional<ex::Window> window[kMonCount];
    ex::ExampleSetup setup;
    wis::Size3D cover_size{};
    wis::Texture cover_texture;
    wis::RenderTarget cover_target;

    wis::Size3D size_swap[kMonCount]{};
    wis::Size3D offset_swap[kMonCount]{};

    // Swapchains
    wis::SwapChain swap[kMonCount];
    wis::Fence fence;
    uint64_t fence_value = 1;

    std::span<const wis::Texture> textures[kMonCount];
    std::array<wis::RenderTarget, ex::swap_buffer_count> render_targets[kMonCount];

    ex::FramedCommandList cmd_list;

    // Resources
    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::Shader vs; // vertex shader
    wis::Shader ps; // pixel shader

    wis::Buffer vertex_buffer; // vertex buffer for triangle
    wis::Buffer constant_buffersx[ex::flight_frames]; // constant buffer for triangle
    wis::Buffer constant_buffersy[ex::flight_frames]; // constant buffer for triangle

    float* constant_datax[ex::flight_frames]; // constant buffer data
    float* constant_datay[ex::flight_frames]; // constant buffer data
    float offsetx = 0.0f; // x offset for the triangle
    float offsety = 0.0f; // y offset for the triangle

    // Descriptor buffers
    wis::DescriptorStorage desc_storage;

public:
    App()
    {
        using namespace wis;

        InitWindows();
        wis::Result result = wis::success;

        setup.InitDefault(platform.get());
        for (size_t i = 0; i < kMonCount; i++) {
            auto [w, h] = window[i]->PixelSize();
            swap[i] = window[i]->CreateSwapchain(result, platform, setup);
            textures[i] = swap[i].GetBufferSpan();

            for (size_t j = 0; j < ex::swap_buffer_count; j++) {
                render_targets[i][j] = setup.device.CreateRenderTarget(result, textures[i][j], { .format = ex::swapchain_format });
            }
        }
        fence = setup.device.CreateFence(result);
        cmd_list = setup.CreateLists();

        wis::DescriptorBindingDesc bindings[] = {
            { .binding_type = wis::DescriptorType::ConstantBuffer, .binding_space = 1, .binding_count = ex::flight_frames * 2 },
        };
        desc_storage = setup.device.CreateDescriptorStorage(result, bindings, std::size(bindings));

        wis::TextureDesc cover_desc{
            .format = ex::swapchain_format,
            .size = cover_size,
            .usage = wis::TextureUsage::RenderTarget | wis::TextureUsage::CopySrc,
        };
        cover_texture = setup.allocator.CreateTexture(result, cover_desc);

        cover_target = setup.device.CreateRenderTarget(result, cover_texture, { .format = ex::swapchain_format });

        auto& cmd = cmd_list[0];
        auto& queue = setup.queue;

        cmd.TextureBarrier(
                { .sync_before = wis::BarrierSync::None,
                  .sync_after = wis::BarrierSync::None,
                  .access_before = wis::ResourceAccess::NoAccess,
                  .access_after = wis::ResourceAccess::NoAccess,
                  .state_before = wis::TextureState::Undefined,
                  .state_after = wis::TextureState::CopySource },
                cover_texture);

        cmd.Close();

        wis::CommandListView lists[] = { cmd };
        queue.ExecuteCommandLists(lists, 1);
        queue.SignalQueue(fence, fence_value);
        fence.Wait(fence_value++);
    }

public:
    void Run()
    {
        CreateResources();
        while (true) {
            if (!ProcessEvents()) {
                break;
            }

            Frame();
        }
    }
    // Process window events
    bool ProcessEvents()
    {
        // Window events
        while (true) {
            auto event = window[0]->PollEvents();
            switch (event) {
            case ex::WindowEvent::Resize: {
                /*auto [w, h] = window[0]->PixelSize();
                swap.Resize(setup.device, w, h);
                break;*/
            }
            case ex::WindowEvent::Quit:
                fence.Wait(fence_value - 1);

                for (size_t i = 0; i < ex::flight_frames; i++) {
                    constant_buffersx[i].Unmap();
                    constant_buffersy[i].Unmap();
                }
                return false;
            case ex::WindowEvent::NoEvent:
                return true;
            default:
                break;
            }
        }
        // Mouse events
        // Keyboard events
    }

    // Render a frame
    void Frame()
    {
        uint32_t frame_index[kMonCount];
        for (size_t i = 0; i < kMonCount; i++) {
            frame_index[i] = swap[i].GetCurrentIndex();
        }
        auto& queue = setup.queue;
        auto& cmd2 = cmd_list[0];

        offsety += 0.01f;
        if (offsety > 1.0f) {
            offsety = -1.0f;
        }
        offsetx = 0.0f;

        // ------------------------------
        // Second pass
        wis::RenderPassRenderTargetDesc targets2[]{
            { .target = cover_target,
              .load_op = wis::LoadOperation::Clear,
              .store_op = wis::StoreOperation::Store,
              .clear_value = { 0.5f, 0.5f, 0.5f, 1.0f } }
        };
        wis::RenderPassDesc rp2{
            .target_count = 1,
            .targets = targets2,
        };

        constant_datax[0][0] = offsetx;
        constant_datay[0][0] = offsety;

        // Begin recording
        ex::CheckResult(cmd2.Reset(pipeline));

        // Insert barriers for the swapchain render target
        cmd2.TextureBarrier(
                { .sync_before = wis::BarrierSync::None,
                  .sync_after = wis::BarrierSync::RenderTarget,
                  .access_before = wis::ResourceAccess::NoAccess,
                  .access_after = wis::ResourceAccess::RenderTarget,
                  .state_before = wis::TextureState::CopySource,
                  .state_after = wis::TextureState::RenderTarget },
                cover_texture);

        cmd2.BeginRenderPass(&rp2);
        cmd2.SetRootSignature(root); // always set root signature before binding resources

        // Bind descriptor storage
        cmd2.SetDescriptorStorage(desc_storage);

        uint32_t root_constants[] = { 0, ex::flight_frames }; // frame index and frame count to get offset to the second cbuffer
        cmd2.SetPushConstants(root_constants, std::size(root_constants), 0, wis::ShaderStages::All);

        cmd2.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
        wis::VertexBufferBinding vertex_binding{
            .buffer = vertex_buffer,
            .size = sizeof(glm::vec3) * 3,
            .stride = sizeof(glm::vec3),
        };
        cmd2.IASetVertexBuffers(&vertex_binding, 1);

        auto [w, h, d] = cover_size;
        cmd2.RSSetViewport({ 0, 0, float(w), float(h), 0, 1 });
        cmd2.RSSetScissor({ 0, 0, int(w), int(h) });
        cmd2.DrawInstanced(3);
        cmd2.EndRenderPass();

        // Insert barriers for the swapchain render target
        cmd2.TextureBarrier(
                { .sync_before = wis::BarrierSync::Draw,
                  .sync_after = wis::BarrierSync::Copy,
                  .access_before = wis::ResourceAccess::RenderTarget,
                  .access_after = wis::ResourceAccess::CopySource,
                  .state_before = wis::TextureState::RenderTarget,
                  .state_after = wis::TextureState::CopySource },
                cover_texture);

        for (size_t i = 0; i < kMonCount; i++) {
            cmd2.TextureBarrier(
                    { .sync_before = wis::BarrierSync::None,
                      .sync_after = wis::BarrierSync::Copy,
                      .access_before = wis::ResourceAccess::NoAccess,
                      .access_after = wis::ResourceAccess::CopyDest,
                      .state_before = wis::TextureState::Present,
                      .state_after = wis::TextureState::CopyDest },
                    textures[i][frame_index[i]]);
        }

        // copy the cover texture to the swapchain render target
        for (size_t i = 0; i < kMonCount; i++) {
            wis::TextureCopyRegion region{
                .src = {
                        .offset = offset_swap[i],
                        .size = size_swap[i],
                        .format = ex::swapchain_format,
                },
                .dst = {
                        .size = size_swap[i],
                        .format = ex::swapchain_format,
                },
            };
            cmd2.CopyTexture(cover_texture, textures[i][frame_index[i]], &region, 1);
        }

        for (size_t i = 0; i < kMonCount; i++) {
            cmd2.TextureBarrier(
                    { .sync_before = wis::BarrierSync::Copy,
                      .sync_after = wis::BarrierSync::Draw,
                      .access_before = wis::ResourceAccess::CopyDest,
                      .access_after = wis::ResourceAccess::Common,
                      .state_before = wis::TextureState::CopyDest,
                      .state_after = wis::TextureState::Present },
                    textures[i][frame_index[i]]);
        }

        // End recording
        cmd2.Close();

        wis::CommandListView lists[] = { cmd2 };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        for (size_t i = 0; i < kMonCount; i++) {
            swap[i].Present();
        }
        for (size_t i = 0; i < kMonCount; i++) {
            swap[i].WaitForPresent();
        }
        setup.queue.SignalQueue(fence, fence_value);

        fence.Wait(fence_value++);
    }
    void CreateResources()
    {
        // Load shaders
        auto vs_code = ex::LoadShader("shaders/desc_storage.vs");
        auto ps_code = ex::LoadShader("shaders/desc_storage.ps");
        vs = ex::Unwrap(setup.device.CreateShader(vs_code.data(), vs_code.size()));
        ps = ex::Unwrap(setup.device.CreateShader(ps_code.data(), ps_code.size()));

        // Create root for storage (it is bindless, so no reason to use tables anymore)
        wis::PushConstant root_constants[]{
            { .stage = wis::ShaderStages::All, .size_bytes = 2 * sizeof(uint32_t) }
        };
        wis::DescriptorBindingDesc bindings[] = {
            { .binding_type = wis::DescriptorType::ConstantBuffer, .binding_space = 1, .space_overlap_count = 2, .binding_count = ex::flight_frames * 2 },
        };
        root = ex::Unwrap(setup.device.CreateRootSignature(root_constants, std::size(root_constants), nullptr, 0, bindings, std::size(bindings)));

        // Create pipeline
        {
            wis::InputSlotDesc input_slots[] = {
                { .slot = 0, .stride_bytes = sizeof(glm::vec3), .input_class = wis::InputClass::PerVertex },
            };
            wis::InputAttribute input_attributes[] = {
                { .input_slot = 0, .semantic_name = "POSITION", .semantic_index = 0, .location = 0, .format = wis::DataFormat::RGB32Float, .offset_bytes = 0 }
            };
            wis::GraphicsPipelineDesc desc{
                .root_signature = root,
                .input_layout = {
                        .slots = input_slots,
                        .slot_count = 1,
                        .attributes = input_attributes,
                        .attribute_count = 1,
                },
                .shaders = { .vertex = vs, .pixel = ps },
                .attachments = {
                        .attachment_formats = { ex::swapchain_format },
                        .attachments_count = 1,
                },
            };
            pipeline = ex::Unwrap(setup.device.CreateGraphicsPipeline(desc));
        }

        // Create vertex buffer
        {
            glm::vec3 triangle_vertices[] = {
                { 0.0f, 0.5f, 0.0f },
                { 0.5f, -0.5f, 0.0f },
                { -0.5f, -0.5f, 0.0f }
            };
            vertex_buffer = setup.CreateAndUploadBuffer(std::span<glm::vec3>{ triangle_vertices }, wis::BufferUsage::VertexBuffer);
        }

        // Create constant buffer
        {
            using namespace wis;
            for (size_t i = 0; i < ex::flight_frames; i++) {
                constant_buffersx[i] = ex::Unwrap(setup.allocator.CreateBuffer(sizeof(float), wis::BufferUsage::CopySrc | wis::BufferUsage::ConstantBuffer, wis::MemoryType::Upload, wis::MemoryFlags::Mapped));
                constant_buffersy[i] = ex::Unwrap(setup.allocator.CreateBuffer(sizeof(float), wis::BufferUsage::CopySrc | wis::BufferUsage::ConstantBuffer, wis::MemoryType::Upload, wis::MemoryFlags::Mapped));
                desc_storage.WriteConstantBuffer(0, i, constant_buffersx[i], sizeof(float));
                desc_storage.WriteConstantBuffer(0, ex::flight_frames + i, constant_buffersy[i], sizeof(float));
                constant_datax[i] = static_cast<float*>(constant_buffersx[i].Map());
                constant_datax[i][0] = 0.0f;

                constant_datay[i] = static_cast<float*>(constant_buffersy[i].Map());
                constant_datay[i][0] = 0.0f;
            }
        }
    }

private:
    void InitWindows()
    {
        int num_displays = 0;
        SDL_DisplayID* displays = SDL_GetDisplays(&num_displays);

        std::unique_ptr<SDL_Rect[]> bounds = std::make_unique<SDL_Rect[]>(num_displays);
        for (int i = 0; i < num_displays && i < kMonCount; i++) {
            SDL_GetDisplayBounds(displays[i], &bounds[i]);
        }
        std::span<SDL_Rect> bounds_span{ bounds.get(), size_t(num_displays) };

        // Sort bounds by x coordinate
        std::sort(bounds_span.begin(), bounds_span.end(), [](const SDL_Rect& a, const SDL_Rect& b) {
            return a.x < b.x;
        });

        for (size_t i = 0; i < kMonCount; i++) {
            window[i].emplace("multimon", bounds_span[i].x, bounds_span[i].y, bounds_span[i].w, bounds_span[i].h, SDL_WINDOW_FULLSCREEN);
        }

        // Set cover size to the size of all displays
        for (size_t i = 0; i < kMonCount; i++) {
            cover_size.width += bounds_span[i].w;
            cover_size.height = std::max(cover_size.height, uint32_t(bounds_span[i].h));
            size_swap[i].height = bounds_span[i].h;
            size_swap[i].width = bounds_span[i].w;
            size_swap[i].depth_or_layers = 1;

            if (i > 0) {
                offset_swap[i].width = bounds_span[i - 1].w;
                offset_swap[i].height = 0;
                offset_swap[i].depth_or_layers = 0;
            }
        }

        cover_size.depth_or_layers = 1;

        SDL_free(displays);
    }
};

int main(int argc, char** argv)
{
    try {
        ex::Instance inst;
        App{}.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
