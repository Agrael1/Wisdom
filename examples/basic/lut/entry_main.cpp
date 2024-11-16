#include <wis_helper.h>
#include <wis_swapchain.h>
#include <window.h>
#include <iostream>

#include "lut_loader.h"

#include <glm/vec3.hpp>

// This example shows how to use LUT (Look-Up Table) to apply color correction to the image.
// LUT is a 3D texture, where each pixel is a color correction value.
// In this example, we will use a 3D texture with size 16x16x16, which is a common size for LUTs.
// The LUT texture will be applied to the image, and the color correction will be visible.
// The LUT texture is loaded from a file, and the image is rendered to the screen.
// This example uses descriptor buffers, which are available on NVidia>=16xx.

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

struct ImageData {
    std::vector<uint8_t> data;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
};

ImageData LoadPNGImage(std::filesystem::path path)
{
    ImageData data;
    auto path_str = path.string();
    auto* idata = stbi_load(path_str.c_str(), (int*)&data.width, (int*)&data.height, (int*)&data.channels, 4);

    std::copy(idata, idata + data.width * data.height * 4, std::back_inserter(data.data));
    stbi_image_free(idata);

    return data;
}

class App
{
    // Standard setup
    ex::Window window;
    ex::ExampleSetup setup;
    ex::Swapchain swap;
    ex::FramedCommandList cmd_list;

    // Resources
    // First pass resources
    wis::RootSignature root; // root signature for first stage
    wis::PipelineState pipeline; // pipeline for first stage
    wis::Shader vs; // vertex shader
    wis::Shader ps; // pixel shader

    wis::Texture texture; // texture for rendering
    wis::Texture texture_lut; // 3D texture for LUT (Look-Up Table)
    wis::ShaderResource srv; // shader resource view for texture
    wis::ShaderResource srv_lut; // shader resource view for LUT
    wis::Sampler sampler; // sampler for texture (linear)
    wis::Sampler sampler_lut; // sampler for LUT (point)

    // Descriptor buffers
    wis::DescriptorBufferExtension desc_ext; // descriptor buffer extension for shader resources
    wis::DescriptorBuffer desc_buffer; // descriptor buffer for shader resources
    wis::DescriptorBuffer sampler_buffer; // descriptor buffer for samplers

public:
    App()
        : window("LUT", 800, 600)
    {
        wis::DeviceExtension* exts[] = { &desc_ext };
        setup.InitDefault(window.GetPlatformExtension(), exts);
        auto [w, h] = window.PixelSize();
        auto swapx = window.CreateSwapchain(setup);
        std::construct_at(&swap, setup.device, std::move(swapx), w, h);
        cmd_list = setup.CreateLists();

        // Only a single descriptor table with 1 descriptor
        uint32_t desc_increment = desc_ext.GetDescriptorSize(wis::DescriptorHeapType::Descriptor);
        desc_buffer = ex::Unwrap(desc_ext.CreateDescriptorBuffer(wis::DescriptorHeapType::Descriptor, wis::DescriptorMemory::ShaderVisible, 2 * desc_increment));

        // No need for multiple samplers
        uint32_t samp_increment = desc_ext.GetDescriptorSize(wis::DescriptorHeapType::Sampler);
        sampler_buffer = ex::Unwrap(desc_ext.CreateDescriptorBuffer(wis::DescriptorHeapType::Sampler, wis::DescriptorMemory::ShaderVisible, 2 * samp_increment));
    }

public:
    void Run()
    {
        CreateResources();
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
        // ------------------------------
        // Only pass
        auto& cmd2 = cmd_list[frame_index];
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
        ex::CheckResult(cmd2.Reset(pipeline));

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
        cmd2.SetRootSignature(root); // always set root signature before binding resources

        // Bind resources
        desc_ext.SetDescriptorBuffers(cmd2, desc_buffer, sampler_buffer);
        desc_ext.SetDescriptorTableOffset(cmd2, root, 0, desc_buffer, 0);
        desc_ext.SetDescriptorTableOffset(cmd2, root, 1, sampler_buffer, 0);

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

        wis::CommandListView lists[] = { cmd2 };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        swap.Present(setup.queue);
    }

    // Create resources
    void CreateResources()
    {
        using namespace wis; // for flag OR operations
        auto& device = setup.device;
        auto& cmd = cmd_list[0];
        ex::CheckResult(cmd.Reset());

        // Create shaders
        {
            auto xvs = ex::LoadShader("shaders/lut.vs");
            auto xps = ex::LoadShader("shaders/lut.ps");
            vs = ex::Unwrap(setup.device.CreateShader(xvs.data(), uint32_t(xvs.size())));
            ps = ex::Unwrap(setup.device.CreateShader(xps.data(), uint32_t(xps.size())));
        }

        // Create root signature with 2 textures and 2 samplers (pair for lut and pair for image)
        {
            wis::DescriptorTableEntry entries[]{
                { .type = wis::DescriptorType::ShaderResource, // LUT texture (3D cube)
                  .bind_register = 0,
                  .binding = 0,
                  .count = 1 },
                { .type = wis::DescriptorType::ShaderResource, // Image texture (2D)
                  .bind_register = 1,
                  .binding = 1,
                  .count = 1 },
                { .type = wis::DescriptorType::Sampler, // LUT sampler (point)
                  .bind_register = 0,
                  .binding = 0,
                  .count = 1 },
                { .type = wis::DescriptorType::Sampler, // image sampler (linear)
                  .bind_register = 1,
                  .binding = 1,
                  .count = 1 }
            };

            wis::DescriptorTable tables[]{
                { .type = wis::DescriptorHeapType::Descriptor,
                  .entries = entries, // first entries
                  .entry_count = 2,
                  .stage = wis::ShaderStages::Pixel },
                { .type = wis::DescriptorHeapType::Sampler,
                  .entries = entries + 2, // skip first 2 entries
                  .entry_count = 2,
                  .stage = wis::ShaderStages::Pixel }
            };
            root = ex::Unwrap(desc_ext.CreateRootSignature(nullptr, 0, tables, std::size(tables)));
        }

        // Create pipeline
        {
            wis::DataFormat attachment_formats[] = { ex::swapchain_format };
            wis::GraphicsPipelineDesc desc{
                .root_signature = root,
                .shaders = { .vertex = vs, .pixel = ps },
                .attachments = {
                        .attachment_formats = attachment_formats,
                        .attachments_count = 1,
                },
                .flags = wis::PipelineFlags::DescriptorBuffer, // use descriptor buffer for root signature
            };
            pipeline = ex::Unwrap(setup.device.CreateGraphicsPipeline(&desc));
        }

        // Load LUT (using uploaded buffer)
        // There is a way to load Textures directly to GPU memory, but it's a topic for another example ;)

        // Load LUT from file and copy it to UploadBuffer
        auto lut_data = LutLoader::LoadLut("lut.cube");
        auto [res2, lut_data_buffer] = setup.allocator.CreateUploadBuffer(lut_data.stride * lut_data.stride * lut_data.stride * 4 * sizeof(float));
        std::copy(lut_data.data.get(), lut_data.data.get() + lut_data.stride * lut_data.stride * lut_data.stride * 4, lut_data_buffer.Map<float>());
        lut_data_buffer.Unmap();

        {
            using namespace wis;

            // Create 3D texture for LUT
            wis::TextureDesc desc{
                .format = wis::DataFormat::RGBA32Float,
                .size = { uint32_t(lut_data.stride), (uint32_t)lut_data.stride, (uint32_t)lut_data.stride },
                .mip_levels = 1,
                .layout = wis::TextureLayout::Texture3D,
                .usage = wis::TextureUsage::CopyDst | wis::TextureUsage::ShaderResource
            };
            texture_lut = ex::Unwrap(setup.allocator.CreateTexture(desc));

            // Copy LUT data to the texture
            wis::BufferTextureCopyRegion region{
                .texture = {
                        .size = { (uint32_t)lut_data.stride, (uint32_t)lut_data.stride, (uint32_t)lut_data.stride },
                        .format = wis::DataFormat::RGBA32Float,
                }
            };
            // Insert barrier for the LUT texture
            cmd.TextureBarrier(
                    { .sync_before = wis::BarrierSync::All,
                      .sync_after = wis::BarrierSync::All,
                      .access_before = wis::ResourceAccess::NoAccess,
                      .access_after = wis::ResourceAccess::CopyDest,
                      .state_before = wis::TextureState::Undefined,
                      .state_after = wis::TextureState::CopyDest },
                    texture_lut);
            cmd.CopyBufferToTexture(lut_data_buffer, texture_lut, &region, 1);
            // Insert barrier for the LUT texture
            cmd.TextureBarrier(
                    { .sync_before = wis::BarrierSync::All,
                      .sync_after = wis::BarrierSync::All,
                      .access_before = wis::ResourceAccess::CopyDest,
                      .access_after = wis::ResourceAccess::ShaderResource,
                      .state_before = wis::TextureState::CopyDest,
                      .state_after = wis::TextureState::ShaderResource },
                    texture_lut);

            // Create shader resource view for LUT
            wis::ShaderResourceDesc srv_desc{
                .format = wis::DataFormat::RGBA32Float,
                .view_type = wis::TextureViewType::Texture3D,
                .subresource_range = {
                        .base_mip_level = 0,
                        .level_count = 1, // no need to specify array layers. It's a 3D texture
                },
            };
            srv_lut = ex::Unwrap(device.CreateShaderResource(texture_lut, srv_desc));

            // Write LUT SRV to the descriptor buffer
            // desc_buffer.WriteShaderResource(0, 0, 0, 0, root, srv_lut);
            desc_buffer.WriteShaderResource(0, 0, srv_lut);

            // Create sampler for LUT
            wis::SamplerDesc sample_desc{
                .min_filter = wis::Filter::Point,
                .mag_filter = wis::Filter::Point,
                .mip_filter = wis::Filter::Point,
                .anisotropic = false,
                .max_anisotropy = 16,
                .address_u = wis::AddressMode::ClampToEdge,
                .address_v = wis::AddressMode::ClampToEdge,
                .address_w = wis::AddressMode::ClampToEdge,
                .min_lod = 0,
                .max_lod = 1.0f,
                .mip_lod_bias = 0.0f,
                .comparison_op = wis::Compare::None,
            };
            sampler_lut = ex::Unwrap(device.CreateSampler(&sample_desc));

            // Write LUT sampler to the descriptor buffer
            sampler_buffer.WriteSampler(0, 0, sampler_lut);
        }

        // Load Image
        auto png_data = LoadPNGImage("image.png");
        auto [res, img_data_buffer] = setup.allocator.CreateUploadBuffer(png_data.data.size());
        std::copy(png_data.data.begin(), png_data.data.end(), img_data_buffer.Map<uint8_t>());
        img_data_buffer.Unmap();

        {
            // Create Texture
            using namespace wis;
            wis::TextureDesc desc{
                .format = wis::DataFormat::RGBA8Unorm,
                .size = { png_data.width, png_data.height, 1 },
                .mip_levels = 1,
                .usage = wis::TextureUsage::CopyDst | wis::TextureUsage::ShaderResource
            };
            texture = ex::Unwrap(setup.allocator.CreateTexture(desc));

            // Copy image data to the texture
            wis::BufferTextureCopyRegion region{
                .texture = {
                        .size = { png_data.width, png_data.height, 1 },
                        .format = wis::DataFormat::RGBA8Unorm,
                }
            };
            // Insert barrier for the image texture
            cmd.TextureBarrier(
                    { .sync_before = wis::BarrierSync::All,
                      .sync_after = wis::BarrierSync::All,
                      .access_before = wis::ResourceAccess::NoAccess,
                      .access_after = wis::ResourceAccess::CopyDest,
                      .state_before = wis::TextureState::Undefined,
                      .state_after = wis::TextureState::CopyDest },
                    texture);
            cmd.CopyBufferToTexture(img_data_buffer, texture, &region, 1);
            // Insert barrier for the image texture
            cmd.TextureBarrier(
                    { .sync_before = wis::BarrierSync::All,
                      .sync_after = wis::BarrierSync::All,
                      .access_before = wis::ResourceAccess::CopyDest,
                      .access_after = wis::ResourceAccess::ShaderResource,
                      .state_before = wis::TextureState::CopyDest,
                      .state_after = wis::TextureState::ShaderResource },
                    texture);

            // Create shader resource view for image
            wis::ShaderResourceDesc srv_desc{
                .format = wis::DataFormat::RGBA8Unorm,
                .view_type = wis::TextureViewType::Texture2D,
                .subresource_range = {
                        .base_mip_level = 0,
                        .level_count = 1, // no need to specify array layers. It's a 2D texture without array layers
                },
            };
            srv = ex::Unwrap(device.CreateShaderResource(texture, srv_desc));

            // Write image SRV to the descriptor buffer
            desc_buffer.WriteShaderResource(0, 1, srv);

            // Create sampler for image
            wis::SamplerDesc sample_desc{
                .min_filter = wis::Filter::Linear,
                .mag_filter = wis::Filter::Linear,
                .mip_filter = wis::Filter::Linear,
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
            sampler = ex::Unwrap(device.CreateSampler(&sample_desc));

            // Write image sampler to the descriptor buffer
            sampler_buffer.WriteSampler(0, 1, sampler);
        }

        cmd.Close();
        wis::CommandListView lists[] = { cmd };
        setup.queue.ExecuteCommandLists(lists, std::size(lists));
        setup.WaitForGPU();
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
