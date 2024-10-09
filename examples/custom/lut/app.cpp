#include "app.h"
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "lut_loader.h"

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

struct LogProvider : public wis::LogLayer {
    virtual void Log(wis::Severity sev, std::string message, wis::source_location sl = wis::source_location::current()) override
    {
        std::cout << wis::format("[{}]: {}\n", wis::severity_strings[+sev], message);
    };
};

void DebugCallback(wis::Severity severity, const char* message, void* user_data)
{
    auto stream = reinterpret_cast<std::ostream*>(user_data);
    *stream << message << "\n";
}

App::App(uint32_t width, uint32_t height)
    : width(width), height(height)
{
    // wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    auto [result, factory] = wis::CreateFactory(false);

    // auto [resx, hinfo] = factory.CreateDebugMessenger(DebugCallback, &std::cout);
    // info = std::move(hinfo);

    for (uint32_t i = 0;; i++) {
        auto [res, adapter] = factory.GetAdapter(i);
        if (res.status == wis::Status::Ok) {
            wis::AdapterDesc desc;
            res = adapter.GetDesc(&desc);
            std::cout << "Adapter: " << desc.description.data() << "\n";

            auto [res, hdevice] = wis::CreateDevice(std::move(adapter));
            if (res.status == wis::Status::Ok) {
                device = std::move(hdevice);
                break;
            };

        } else {
            break;
        }
    }

    auto [res2, hqueue] = device.CreateCommandQueue(wis::QueueType::Graphics);
    queue = std::move(hqueue);

    {
        auto [res, alloc] = device.CreateAllocator();
        allocator = std::move(alloc);
    }

    {
        using namespace wis;
        wis::TextureDesc desc{
            .format = wis::DataFormat::RGBA8Unorm,
            .size = { width, height, 1 },
            .mip_levels = 1,
            .usage = wis::TextureUsage::RenderTarget | wis::TextureUsage::CopySrc
        };
        auto [res, htexture] = allocator.CreateTexture(desc);
        out_texture = std::move(htexture);

        wis::RenderTargetDesc rt_desc{
            .format = wis::DataFormat::RGBA8Unorm,
            .layout = wis::TextureLayout::Texture2D,
            .mip = 0,
            .base_array_layer = 0,
            .layer_count = 1,
        };
        auto [res2, hrt] = device.CreateRenderTarget(out_texture, rt_desc);
        render_target = std::move(hrt);
    }

    auto [res4, hfence] = device.CreateFence();
    fence = std::move(hfence);

    auto [res5, hcmd_list] = device.CreateCommandList(wis::QueueType::Graphics);
    cmd_list = std::move(hcmd_list);

    CreateResources();
}

int App::Start()
{
    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    // Point filter
    for (size_t i = 0; i < 1000; i++) {
        Frame();
    }

    auto end = clock.now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    DumpFrame("out_point.png");

    // Linear filter
    pipeline_c = &pipeline2;
    start = clock.now();
    for (size_t i = 0; i < 1000; i++) {
        Frame();
    }
    end = clock.now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    DumpFrame("out_linear.png");

    // Tetra + Point filter
    pipeline_c = &pipeline2;
    sampler_buffer.WriteSampler(0, 0, sampler);

    start = clock.now();
    for (size_t i = 0; i < 1000; i++) {
        Frame();
    }
    end = clock.now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    DumpFrame("out_tetra.png");

    return 0;
}

std::string LoadShader(std::filesystem::path p)
{
    if constexpr (wis::shader_intermediate == wis::ShaderIntermediate::DXIL)
        p += u".cso";
    else
        p += u".spv";

    std::ifstream t{ p, std::ios::binary };
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string ret;
    ret.resize(size);
    t.seekg(0);
    t.read(ret.data(), size);
    return ret;
}

void App::CreateResources()
{
    // Create descriptor buffer
    {
        auto desc_increment = device.GetDescriptorBufferUnitSize(wis::DescriptorHeapType::Descriptor);
        auto [res, hdesc] = device.CreateDescriptorBuffer(wis::DescriptorHeapType::Descriptor, wis::DescriptorMemory::ShaderVisible, 2 * desc_increment);
        desc_buffer = std::move(hdesc);

        auto sampler_increment = device.GetDescriptorBufferUnitSize(wis::DescriptorHeapType::Sampler);
        auto [res2, hdesc2] = device.CreateDescriptorBuffer(wis::DescriptorHeapType::Sampler, wis::DescriptorMemory::ShaderVisible, 2 * sampler_increment);
        sampler_buffer = std::move(hdesc2);
    }

    // Upload lut data to a buffer
    {
        auto png_data = LoadPNGImage("image.png");
        auto [res, img_data_buffer] = allocator.CreateUploadBuffer(png_data.data.size());
        std::copy(png_data.data.begin(), png_data.data.end(), img_data_buffer.Map<uint8_t>());
        img_data_buffer.Unmap();

        // Create Texture
        {
            using namespace wis;
            wis::TextureDesc desc{
                .format = wis::DataFormat::RGBA8Unorm,
                .size = { png_data.width, png_data.height, 1 },
                .mip_levels = 1,
                .usage = wis::TextureUsage::CopyDst | wis::TextureUsage::ShaderResource
            };
            auto [res, htexture] = allocator.CreateTexture(desc);
            texture = std::move(htexture);
        }

        auto lut_data = LutLoader::LoadLut("lut.cube");
        auto [res2, lut_data_buffer] = allocator.CreateUploadBuffer(lut_data.stride * lut_data.stride * lut_data.stride * 4 * sizeof(float));
        std::copy(lut_data.data.get(), lut_data.data.get() + lut_data.stride * lut_data.stride * lut_data.stride * 4, lut_data_buffer.Map<float>());
        lut_data_buffer.Unmap();

        {
            wis::DescriptorTableEntry entries[] = {
                {
                    .type = wis::DescriptorType::ShaderResource,
                    .bind_register = 0,
                    .binding = 0,
                    .count = 1,
                },
                {
                    .type = wis::DescriptorType::ShaderResource,
                    .bind_register = 1,
                    .binding = 1,
                    .count = 1,
                },
                {
                    .type = wis::DescriptorType::Sampler,
                    .bind_register = 0,
                    .binding = 0,
                    .count = 1,
                },
                {
                    .type = wis::DescriptorType::Sampler,
                    .bind_register = 1,
                    .binding = 1,
                    .count = 1,
                },
            };

            wis::DescriptorTable tables[] = {
                {
                    .type = wis::DescriptorHeapType::Descriptor,
                    .entries = entries,
                    .entry_count = 2,
                    .stage = wis::ShaderStages::Pixel,
                },
                {
                    .type = wis::DescriptorHeapType::Sampler,
                    .entries = entries + 2,
                    .entry_count = 2,
                    .stage = wis::ShaderStages::Pixel,
                },

            };
            auto [result, hroot] = device.CreateRootSignature(nullptr, 0, tables, sizeof(tables) / sizeof(tables[0]));
            root = std::move(hroot);
        }

        // Create Texture
        {
            using namespace wis;
            wis::TextureDesc desc{
                .format = wis::DataFormat::RGBA32Float,
                .size = { uint32_t(lut_data.stride), (uint32_t)lut_data.stride, (uint32_t)lut_data.stride },
                .mip_levels = 1,
                .layout = wis::TextureLayout::Texture3D,
                .usage = wis::TextureUsage::CopyDst | wis::TextureUsage::ShaderResource
            };
            auto [res, htexture] = allocator.CreateTexture(desc);
            lut = std::move(htexture);
        }

        cmd_list.TextureBarrier({
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::All,
            .access_before = wis::ResourceAccess::NoAccess,
            .access_after = wis::ResourceAccess::CopyDest,
            .state_before = wis::TextureState::Undefined,
            .state_after = wis::TextureState::CopyDest,
            .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            },
        },
        lut);

        cmd_list.TextureBarrier({
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::All,
            .access_before = wis::ResourceAccess::NoAccess,
            .access_after = wis::ResourceAccess::CopyDest,
            .state_before = wis::TextureState::Undefined,
            .state_after = wis::TextureState::CopyDest,
            .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            },
        },
        texture);

        wis::BufferTextureCopyRegion region{
            .texture = {
                .size = { (uint32_t)lut_data.stride, (uint32_t)lut_data.stride, (uint32_t)lut_data.stride },
                .format = wis::DataFormat::RGBA32Float,
            }
        };
        cmd_list.CopyBufferToTexture(lut_data_buffer, lut, &region, 1);

        wis::BufferTextureCopyRegion region2{
            .texture = {
                .size = { png_data.width, png_data.height, 1 },
                .format = wis::DataFormat::RGBA8Unorm,
            }
        };
        cmd_list.CopyBufferToTexture(img_data_buffer, texture, &region2, 1);

        cmd_list.TextureBarrier(
        {
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::All,
            .access_before = wis::ResourceAccess::CopyDest,
            .access_after = wis::ResourceAccess::ShaderResource,
            .state_before = wis::TextureState::CopyDest,
            .state_after = wis::TextureState::ShaderResource,
            .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            },
        },
        texture);
        cmd_list.TextureBarrier(
        {
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::All,
            .access_before = wis::ResourceAccess::CopyDest,
            .access_after = wis::ResourceAccess::ShaderResource,
            .state_before = wis::TextureState::CopyDest,
            .state_after = wis::TextureState::ShaderResource,
            .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            },
        },
        lut);

        cmd_list.TextureBarrier({
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::All,
            .access_before = wis::ResourceAccess::NoAccess,
            .access_after = wis::ResourceAccess::RenderTarget,
            .state_before = wis::TextureState::Undefined,
            .state_after = wis::TextureState::RenderTarget,
            .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            },
        },
        out_texture);
        cmd_list.Close();

        wis::CommandListView cmd_lists[] = { cmd_list };
        queue.ExecuteCommandLists(cmd_lists, 1);

        WaitForGPU();

        auto [resz2, hsrv] = device.CreateShaderResource(texture, { .format = wis::DataFormat::RGBA8Unorm, .view_type = wis::TextureViewType::Texture2D, .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            }
        });
        auto [res2z3, hsrv2] = device.CreateShaderResource(lut, { .format = wis::DataFormat::RGBA32Float, .view_type = wis::TextureViewType::Texture3D, .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            }
        });
        srv_lut = std::move(hsrv2);
        srv = std::move(hsrv);
        auto offset = desc_buffer.WriteShaderResource(0, 0, 0, 0, root, srv_lut);
        desc_buffer.WriteShaderResource(offset, 0, 1, 0, root, srv);
    }

    {
        auto s1 = LoadShader("lut.vs");
        auto s2 = LoadShader("lut.ps");
        auto s3 = LoadShader("lut_tetra.ps");
        auto [result, vs] = device.CreateShader(s1.data(), uint32_t(s1.size()));
        auto [result2, ps] = device.CreateShader(s2.data(), uint32_t(s2.size()));
        auto [result3, ps2] = device.CreateShader(s3.data(), uint32_t(s3.size()));

        vertex_shader = std::move(vs);
        pixel_shader = std::move(ps);
        pixel_shader_tetra = std::move(ps2);
    }

    {
        wis::DataFormat attachment_formats[] = { wis::DataFormat::RGBA8Unorm };

        wis::GraphicsPipelineDesc desc{
            .root_signature = root,
            .shaders = { .vertex = vertex_shader, .pixel = pixel_shader },
            .attachments = {
                .attachment_formats = attachment_formats,
                .attachments_count = 1,
            }
        };
        auto [res2, hpipeline] = device.CreateGraphicsPipeline(&desc);
        pipeline = std::move(hpipeline);

        desc.shaders.pixel = pixel_shader_tetra;
        auto [res3, hpipeline2] = device.CreateGraphicsPipeline(&desc);
        pipeline2 = std::move(hpipeline2);
    }

    // Create Sampler
    {
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
        auto [res, hsampler] = device.CreateSampler(&sample_desc);
        sampler = std::move(hsampler);

        wis::SamplerDesc sample_desc2{
            .min_filter = wis::Filter::Linear,
            .mag_filter = wis::Filter::Linear,
            .mip_filter = wis::Filter::Linear,
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
        auto [res2, hsampler2] = device.CreateSampler(&sample_desc2);
        sampler_lut_linear = std::move(hsampler2);

        sampler_buffer.WriteSampler(0, 0, sampler);
        sampler_buffer.WriteSampler(0, 1, sampler);
    }
    pipeline_c = &pipeline;

    WaitForGPU();
}

void App::Frame()
{
    auto res = cmd_list.Reset(*pipeline_c);

    wis::RenderPassRenderTargetDesc targets{
        .target = render_target,
        .load_op = wis::LoadOperation::Clear,
        .store_op = wis::StoreOperation::Store,
        .clear_value = { 0.0f, 0.2f, 0.4f, 1.0f },
    };
    wis::RenderPassDesc rp{
        .flags = wis::RenderPassFlags::None,
        .target_count = 1,
        .targets = &targets,
    };

    cmd_list.BeginRenderPass(&rp);
    cmd_list.SetRootSignature(root);

    wis::DescriptorBufferView desc_buffers[] = { desc_buffer, sampler_buffer };

    cmd_list.SetDescriptorBuffers(desc_buffers, 2);
    cmd_list.SetDescriptorTableOffset(0, desc_buffer, 0);
    cmd_list.SetDescriptorTableOffset(1, sampler_buffer, 0);

    cmd_list.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);

    cmd_list.RSSetViewport({ 0, 0, float(width), float(height), 0, 1 });
    cmd_list.RSSetScissor({ 0, 0, int(width), int(height) });

    cmd_list.DrawInstanced(3);
    cmd_list.EndRenderPass();
    cmd_list.Close();

    wis::CommandListView lists[] = { cmd_list };
    queue.ExecuteCommandLists(lists, 1);

    WaitForGPU();
}

void App::DumpFrame(const char* name)
{
    auto [res, data] = allocator.CreateReadbackBuffer(width * height * 4);

    std::ignore = cmd_list.Reset();

    wis::TextureBarrier2 barriers[] = {
        { {
                .sync_before = wis::BarrierSync::All,
                .sync_after = wis::BarrierSync::All,
                .access_before = wis::ResourceAccess::RenderTarget,
                .access_after = wis::ResourceAccess::CopySource,
                .state_before = wis::TextureState::RenderTarget,
                .state_after = wis::TextureState::CopySource,
                .subresource_range = {
                    .base_mip_level = 0,
                    .level_count = 1,
                    .base_array_layer = 0,
                    .layer_count = 1,
                },
            },
            out_texture
        },
        { {
                .sync_before = wis::BarrierSync::All,
                .sync_after = wis::BarrierSync::All,
                .access_before = wis::ResourceAccess::CopySource,
                .access_after = wis::ResourceAccess::RenderTarget,
                .state_before = wis::TextureState::CopySource,
                .state_after = wis::TextureState::RenderTarget,
                .subresource_range = {
                    .base_mip_level = 0,
                    .level_count = 1,
                    .base_array_layer = 0,
                    .layer_count = 1,
                },
            },
            out_texture
        },

    };

    cmd_list.TextureBarriers(barriers, 1);

    wis::BufferTextureCopyRegion region{
        .texture = {
            .size = { width, height, 1 },
            .format = wis::DataFormat::RGBA8Unorm,
        }
    };

    cmd_list.CopyTextureToBuffer(out_texture, data, &region, 1);
    cmd_list.TextureBarriers(barriers + 1, 1);
    cmd_list.Close();

    wis::CommandListView lists[] = { cmd_list };
    queue.ExecuteCommandLists(lists, 1);

    WaitForGPU();

    auto* ptr = data.Map<uint8_t>();

    stbi_write_png(name, width, height, 4, ptr, width * 4);
    data.Unmap();
}

void App::WaitForGPU()
{
    const uint64_t vfence = fence_value;
    queue.SignalQueue(fence, vfence);
    fence_value++;
    std::ignore = fence.Wait(vfence);
}
