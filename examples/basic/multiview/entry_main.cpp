#include <wis_helper.h>
#include <wis_swapchain.h>
#include <window.h>
#include <iostream>

#include <glm/vec3.hpp>

// In order to render with multiview, we need to have texture with array layers.
// In case of stereo rendering, we need to have 2 array layers.
// Stereo example is not implemented here, but it is possible to implement it by
// creating a swapchain with stereo=true. Stereo rendering is not supported by all graphics cards.
// You can check if stereo is supported by calling swap.StereoSupported().

// In this example, we will render to the texture with 2 array layers, imitating stereo rendering.
// One layer will be rendered with red color, and the other with blue color.

class App
{
    // Standard setup
    ex::Window window;
    ex::ExampleSetup setup;
    ex::Swapchain swap;
    ex::FramedCommandList cmd_list; // for first pass
    ex::FramedCommandList cmd_list2; // for second pass

    // Resources
    // First pass resources
    wis::RootSignature root; // root signature for first stage
    wis::PipelineState pipeline; // pipeline for first stage
    wis::Shader vs; // vertex shader
    wis::Shader ps; // pixel shader

    wis::Texture textures[ex::flight_frames]; // textures for rendering, with 2 array layers
    wis::ShaderResource srvs[ex::flight_frames]; // shader resource view for texture
    wis::RenderTarget rts[ex::flight_frames]; // render target for texture

    wis::Buffer vertex_buffer; // vertex buffer for triangle

    // Second pass resources
    wis::RootSignature fullscreen_root; // root signature for second stage
    wis::PipelineState fullscreen_pipeline; // pipeline for second stage
    wis::Shader fullscreen_vs; // vertex shader for second stage
    wis::Shader fullscreen_ps; // pixel shader for second stage
    wis::Sampler sampler; // sampler for texture

    // Descriptor buffers
    ex::FramedDescriptorSetup desc_buffer; // descriptor buffer for shader resources
    wis::DescriptorBuffer sampler_buffer; // descriptor buffer for samplers

public:
    App()
        : window("Multiview", 800, 600)
    {
        setup.InitDefault(window.GetPlatformExtension());
        auto [w, h] = window.PixelSize();
        auto swapx = window.CreateSwapchain(setup);
        std::construct_at(&swap, setup.device, std::move(swapx), w, h);
        cmd_list = setup.CreateLists();
        cmd_list2 = setup.CreateLists();

        // Only a single descriptor table with 1 descriptor
        std::array<ex::DescTable, 1> desc_tables{
            ex::DescTable{ .descriptor_count = 1 }
        };
        std::construct_at(&desc_buffer, setup.device, wis::DescriptorHeapType::Descriptor, std::span{ desc_tables });

        // No need for multiple samplers
        uint32_t desc_increment = setup.device.GetDescriptorBufferUnitSize(wis::DescriptorHeapType::Sampler);
        sampler_buffer = ex::Unwrap(setup.device.CreateDescriptorBuffer(wis::DescriptorHeapType::Sampler, wis::DescriptorMemory::ShaderVisible, 1 * desc_increment));
    }

public:
    void Run()
    {
        CreateFirstPassResources();
        CreateSecondPassResources();
        while (true) {
            if (!ProcessEvents())
                break;

            Frame();
        }
    }
    // Process window events
    bool ProcessEvents()
    {
        // Window events
        while (true) {
            auto event = window.PollEvents();
            switch (event) {
            case ex::WindowEvent::Resize: {
                auto [w, h] = window.PixelSize();
                swap.Resize(setup.device, w, h);
                break;
            }
            case ex::WindowEvent::Quit:
                swap.Throttle(); // wait for GPU to finish, then exit
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
        uint32_t frame_index = swap.CurrentFrame();
        auto& cmd = cmd_list[frame_index];

        wis::RenderPassRenderTargetDesc targets[]{
            { .target = rts[frame_index],
              .load_op = wis::LoadOperation::Clear,
              .store_op = wis::StoreOperation::Store,
              .clear_value = { 0.1f, 0.1f, 0.1f, 1.0f } } // clear with gray color
        };
        wis::RenderPassDesc rp1{
            .flags = wis::RenderPassFlags::None,
            .view_mask = 0b11, // 2 array layers
            .target_count = 1,
            .targets = targets,
        };

        // Begin recording
        ex::CheckResult(cmd.Reset(pipeline));

        // Insert barriers for the texture
        cmd.TextureBarrier(
                { .sync_before = wis::BarrierSync::None,
                  .sync_after = wis::BarrierSync::Draw,
                  .access_before = wis::ResourceAccess::NoAccess,
                  .access_after = wis::ResourceAccess::RenderTarget,
                  .state_before = wis::TextureState::ShaderResource,
                  .state_after = wis::TextureState::RenderTarget },
                textures[frame_index]);

        // Record commands for the first pass
        // ...
        cmd.BeginRenderPass(&rp1);
        cmd.SetRootSignature(root); // always set root signature before binding resources
        cmd.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
        cmd.RSSetViewport({ 0, 0, 800, 800, 0, 1 });
        cmd.RSSetScissor({ 0, 0, 800, 800 });

        wis::VertexBufferBinding vertex_binding{
            .buffer = vertex_buffer,
            .size = sizeof(glm::vec3) * 3,
            .stride = sizeof(glm::vec3),
        };
        cmd.IASetVertexBuffers(&vertex_binding, 1);
        cmd.DrawInstanced(3);

        cmd.EndRenderPass();
        // Insert barriers for the texture
        cmd.TextureBarrier(
                { .sync_before = wis::BarrierSync::Draw,
                  .sync_after = wis::BarrierSync::All,
                  .access_before = wis::ResourceAccess::RenderTarget,
                  .access_after = wis::ResourceAccess::ShaderResource,
                  .state_before = wis::TextureState::RenderTarget,
                  .state_after = wis::TextureState::ShaderResource },
                textures[frame_index]);

        // End recording
        cmd.Close();

        // ------------------------------
        // Second pass
        auto& cmd2 = cmd_list2[frame_index];
        wis::RenderPassRenderTargetDesc targets2[]{
            { .target = swap.GetRenderTarget(frame_index),
              .load_op = wis::LoadOperation::Clear,
              .store_op = wis::StoreOperation::Store,
              .clear_value = { 0.0f, 0.5f, 0.5f, 1.0f } }
        };
        wis::RenderPassDesc rp2{
            .flags = wis::RenderPassFlags::None,
            .view_mask = 0, // no array layers
            .target_count = 1,
            .targets = targets2,
        };

        // Begin recording
        ex::CheckResult(cmd2.Reset(fullscreen_pipeline));

        // Insert barriers for the swapchain render target
        cmd2.TextureBarrier(
                { .sync_before = wis::BarrierSync::None,
                  .sync_after = wis::BarrierSync::Draw,
                  .access_before = wis::ResourceAccess::NoAccess,
                  .access_after = wis::ResourceAccess::RenderTarget,
                  .state_before = wis::TextureState::Present,
                  .state_after = wis::TextureState::RenderTarget },
                swap.GetTexture(frame_index));

        cmd2.BeginRenderPass(&rp2);
        cmd2.SetRootSignature(fullscreen_root); // always set root signature before binding resources

        wis::DescriptorBufferView desc_buffers[] = { desc_buffer.desc_buffer, sampler_buffer };
        cmd2.SetDescriptorBuffers(desc_buffers, 2);

        cmd2.SetDescriptorTableOffset(0, desc_buffer.desc_buffer, desc_buffer.offset_frame(frame_index));
        cmd2.SetDescriptorTableOffset(1, sampler_buffer, 0);
        cmd2.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);

        auto [w, h] = window.PixelSize();
        cmd2.RSSetViewport({ 0, 0, float(w), float(h), 0, 1 });
        cmd2.RSSetScissor({ 0, 0, w, h });
        cmd2.DrawInstanced(3);
        cmd2.EndRenderPass();

        // Insert barriers for the swapchain render target
        cmd2.TextureBarrier(
                { .sync_before = wis::BarrierSync::Draw,
                  .sync_after = wis::BarrierSync::Draw,
                  .access_before = wis::ResourceAccess::RenderTarget,
                  .access_after = wis::ResourceAccess::Common,
                  .state_before = wis::TextureState::RenderTarget,
                  .state_after = wis::TextureState::Present },
                swap.GetTexture(frame_index));

        // End recording
        cmd2.Close();

        wis::CommandListView lists[] = { cmd, cmd2 };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        swap.Present(setup.queue);
    }

    // Create resources
    void CreateFirstPassResources()
    {
        using namespace wis; // for flag OR operations
        auto& device = setup.device;
        auto& cmd = cmd_list[0];
        ex::CheckResult(cmd.Reset());

        // Create shaders
        {
            auto xvs = ex::LoadShader("shaders/multiview.vs");
            auto xps = ex::LoadShader("shaders/multiview.ps");
            vs = ex::Unwrap(setup.device.CreateShader(xvs.data(), uint32_t(xvs.size())));
            ps = ex::Unwrap(setup.device.CreateShader(xps.data(), uint32_t(xps.size())));
        }

        // Create texture with 2 array layers
        {
            wis::TextureDesc desc{
                .format = wis::DataFormat::BGRA8Unorm,
                .size = { 800, 800, 2 }, // 2 array layers
                .mip_levels = 1,
                .layout = wis::TextureLayout::Texture2DArray,
                .usage = wis::TextureUsage::RenderTarget | wis::TextureUsage::ShaderResource
            };

            // Create render target for texture for the first pass
            wis::RenderTargetDesc rt_desc{
                .format = wis::DataFormat::BGRA8Unorm,
                .layout = wis::TextureLayout::Texture2DArray,
                .mip = 0,
                .base_array_layer = 0,
                .layer_count = 2,
            };

            // Create shader resource view for texture for the second pass
            wis::ShaderResourceDesc srv_desc{
                .format = wis::DataFormat::BGRA8Unorm,
                .view_type = wis::TextureViewType::Texture2DArray,
                .subresource_range = {
                        .base_mip_level = 0,
                        .level_count = 1,
                        .base_array_layer = 0,
                        .layer_count = 2,
                },
            };

            for (size_t i = 0; i < ex::flight_frames; i++) {
                textures[i] = ex::Unwrap(setup.allocator.CreateTexture(desc));
                rts[i] = ex::Unwrap(device.CreateRenderTarget(textures[i], rt_desc));
                srvs[i] = ex::Unwrap(device.CreateShaderResource(textures[i], srv_desc));
                // Texture is in undefined state, so we need to transition it to render target state
                cmd.TextureBarrier(
                        { .sync_before = wis::BarrierSync::None,
                          .sync_after = wis::BarrierSync::None,
                          .access_before = wis::ResourceAccess::NoAccess,
                          .access_after = wis::ResourceAccess::NoAccess,
                          .state_before = wis::TextureState::Undefined,
                          .state_after = wis::TextureState::ShaderResource }, // we will change this later
                        textures[i]);
            }
        }

        // Create empty root signature
        root = ex::Unwrap(setup.device.CreateRootSignature(nullptr, 0, nullptr, 0));

        // Create pipeline
        {
            wis::InputSlotDesc input_slots[] = {
                { .slot = 0, .stride_bytes = sizeof(glm::vec3), .input_class = wis::InputClass::PerVertex },
            };
            wis::InputAttribute input_attributes[] = {
                { .input_slot = 0, .semantic_name = "POSITION", .semantic_index = 0, .location = 0, .format = wis::DataFormat::RGB32Float, .offset_bytes = 0 }
            };
            wis::DataFormat attachment_formats[] = { wis::DataFormat::BGRA8Unorm };
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
                        .attachment_formats = attachment_formats,
                        .attachments_count = 1,
                },
                .view_mask = 0b11, // 2 array layers
            };
            pipeline = ex::Unwrap(setup.device.CreateGraphicsPipeline(&desc));
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

        cmd.Close();
        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        setup.WaitForGPU();
    }

    void CreateSecondPassResources()
    {
        // Create shaders
        {
            auto xvs = ex::LoadShader("shaders/multiview_screen.vs");
            auto xps = ex::LoadShader("shaders/multiview_screen.ps");
            fullscreen_vs = ex::Unwrap(setup.device.CreateShader(xvs.data(), uint32_t(xvs.size())));
            fullscreen_ps = ex::Unwrap(setup.device.CreateShader(xps.data(), uint32_t(xps.size())));
        }

        // Create root signature with 1 texture and 1 sampler
        {
            wis::DescriptorTableEntry entries[]{
                { .type = wis::DescriptorType::ShaderResource,
                  .bind_register = 0,
                  .binding = 0,
                  .count = 1 },
                { .type = wis::DescriptorType::Sampler,
                  .bind_register = 0,
                  .binding = 0,
                  .count = 1 }
            };

            wis::DescriptorTable tables[]{
                { .type = wis::DescriptorHeapType::Descriptor,
                  .entries = entries, // first entry
                  .entry_count = 1,
                  .stage = wis::ShaderStages::Pixel },
                { .type = wis::DescriptorHeapType::Sampler,
                  .entries = entries + 1, // skip first entry
                  .entry_count = 1,
                  .stage = wis::ShaderStages::Pixel }
            };
            fullscreen_root = ex::Unwrap(setup.device.CreateRootSignature(nullptr, 0, tables, std::size(tables)));
        }

        // Create pipeline
        {
            wis::DataFormat attachment_formats[] = { ex::swapchain_format };
            wis::GraphicsPipelineDesc desc{
                .root_signature = fullscreen_root,
                .shaders = { .vertex = fullscreen_vs, .pixel = fullscreen_ps },
                .attachments = {
                        .attachment_formats = attachment_formats,
                        .attachments_count = 1,
                },
                // view mask is 0b00, because we will render to the back buffer
            };
            fullscreen_pipeline = ex::Unwrap(setup.device.CreateGraphicsPipeline(&desc));
        }

        // Create Sampler
        {
            wis::SamplerDesc sample_desc{
                .min_filter = wis::Filter::Point,
                .mag_filter = wis::Filter::Point,
                .mip_filter = wis::Filter::Point,
                .anisotropic = false,
                .max_anisotropy = 16,
                .address_u = wis::AddressMode::Repeat,
                .address_v = wis::AddressMode::Repeat,
                .address_w = wis::AddressMode::Repeat,
                .min_lod = 0,
                .max_lod = 1.0f,
                .mip_lod_bias = 0.0f,
                .comparison_op = wis::Compare::None,
            };
            sampler = ex::Unwrap(setup.device.CreateSampler(&sample_desc));
            sampler_buffer.WriteSampler(0, 0, sampler);
        }

        // fill desc buffer
        {
            for (uint32_t i = 0; i < ex::flight_frames; i++) {
                desc_buffer.desc_buffer.WriteShaderResource(desc_buffer.offsets[i], 0, 0, 0, fullscreen_root, srvs[i]);
            }
        }
    }
};

int main(int argc, char** argv)
{
    try {
        App{}.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}