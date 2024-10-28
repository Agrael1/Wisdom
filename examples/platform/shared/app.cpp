#include "app.h"
#include <iostream>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>
#include <wisdom/wisdom_debug.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <filesystem>
#include <fstream>

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

Test::App::App(std::span<wis::FactoryExtension*> factory_exts)
{
    wis::LibLogger::SetLogLayer(std::make_shared<LogProvider>());

    wis::DebugExtension debug_ext;
    std::unique_ptr<wis::FactoryExtension*[]> xfactory_exts = wis::detail::make_unique_for_overwrite<wis::FactoryExtension*[]>(factory_exts.size() + 1);
    xfactory_exts[0] = &debug_ext;
    std::copy(factory_exts.begin(), factory_exts.end(), xfactory_exts.get() + 1);

    auto [result, factory] = wis::CreateFactory(true, xfactory_exts.get(), factory_exts.size() + 1);
    auto [resx, hinfo] = debug_ext.CreateDebugMessenger(DebugCallback, &std::cout);
    info = std::move(hinfo);

    for (size_t i = 0;; i++) {
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

    auto [res4, hfence] = device.CreateFence();
    fence = std::move(hfence);

    auto [res5, hcmd_list] = device.CreateCommandList(wis::QueueType::Graphics);
    cmd_list = std::move(hcmd_list);
}

void Test::App::SetSwapChain(wis::SwapChain hswap, uint32_t width, uint32_t height)
{
    swap = std::move(hswap);
    back_buffers = swap.GetBufferSpan();
    this->width = width;
    this->height = height;

    wis::RenderTargetDesc rt_desc{
        .format = wis::DataFormat::BGRA8Unorm,
        .layout = wis::TextureLayout::Texture2D,
        .mip = 0,
        .base_array_layer = 0,
        .layer_count = 1,
    };
    for (size_t i = 0; i < render_targets.size(); i++) {
        auto [res, hrt] = device.CreateRenderTarget(back_buffers[i], rt_desc);
        render_targets[i] = std::move(hrt);
    }
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

void Test::App::CreateResources()
{
    using namespace wis;
    CreateRootSignature();

    struct Vertex {
        glm::vec3 pos;
        glm::vec2 tc;
    };
    auto aspect_ratio = float(width) / float(height);
    Vertex triangleVertices[] = {
        { { 0.0f, 0.25f * aspect_ratio, 0.0f }, { 1, 1 } },
        { { 0.0f, -0.25f * aspect_ratio, 0.0f }, { 1.0f, 0.0f } },
        { { -0.25f, -0.25f * aspect_ratio, 0.0f }, { 0.0f, 0.0f } }
    };

    {
        auto [res, alloc] = device.CreateAllocator();
        allocator = std::move(alloc);
    }
    auto [resx, ubuf] = allocator.CreateUploadBuffer(sizeof(triangleVertices));
    auto [resx2, ubuf2] = allocator.CreateUploadBuffer(2 * 2 * 4);
    auto [resx3, ubuf3] = allocator.CreateUploadBuffer(sizeof(float) * 4);

    {
        auto [res, vbuf] = allocator.CreateBuffer(sizeof(triangleVertices), wis::BufferUsage::VertexBuffer | wis::BufferUsage::CopyDst);
        vertex_buffer = std::move(vbuf);

        vertex_binding = wis::VertexBufferBinding{
            .buffer = vertex_buffer,
            .size = sizeof(triangleVertices),
            .stride = sizeof(Vertex),
        };
    }
    // Create Texture
    {
        using namespace wis;
        wis::TextureDesc desc{
            .format = wis::DataFormat::BGRA8Unorm,
            .size = { 2, 2, 1 },
            .mip_levels = 1,
            .usage = wis::TextureUsage::CopyDst | wis::TextureUsage::ShaderResource
        };
        auto [res, htexture] = allocator.CreateTexture(desc);
        texture = std::move(htexture);
    }

    // Create descriptor buffer
    {
        auto desc_increment = device.GetDescriptorBufferUnitSize(wis::DescriptorHeapType::Descriptor);
        auto [res, hdesc] = device.CreateDescriptorBuffer(wis::DescriptorHeapType::Descriptor, wis::DescriptorMemory::ShaderVisible, 2u * desc_increment);
        desc_buffer = std::move(hdesc);

        auto sampler_increment = device.GetDescriptorBufferUnitSize(wis::DescriptorHeapType::Sampler);
        auto [res2, hdesc2] = device.CreateDescriptorBuffer(wis::DescriptorHeapType::Sampler, wis::DescriptorMemory::ShaderVisible, 1u * sampler_increment);
        sampler_buffer = std::move(hdesc2);
    }
    {
        auto [res, val] = allocator.CreateBuffer(sizeof(float) * 4, wis::BufferUsage::ConstantBuffer | wis::BufferUsage::CopyDst);
        cbuf = std::move(val);
    }

    // Upload vertex data to a buffer
    {
        auto memory = ubuf.Map<Vertex>();
        std::copy(std::begin(triangleVertices), std::end(triangleVertices), memory);
        ubuf.Unmap();

        auto memoryx = ubuf3.Map<glm::vec4>();
        *memoryx = { 1, 1, 0, 1 };
        ubuf3.Unmap();

        auto texture_mem = ubuf2.Map<uint32_t>();
        texture_mem[0] = 0xFF0000FF;
        texture_mem[1] = 0xFF00FF00;
        texture_mem[2] = 0xFFFF0000;
        texture_mem[3] = 0xFF00FFFF;
        ubuf2.Unmap();

        cmd_list.CopyBuffer(ubuf, vertex_buffer, { .size_bytes = sizeof(triangleVertices) });
        cmd_list.CopyBuffer(ubuf3, cbuf, { .size_bytes = sizeof(float) * 4 });
        cmd_list.BufferBarrier({
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::Draw,
            .access_before = wis::ResourceAccess::Common,
            .access_after = wis::ResourceAccess::VertexBuffer,
        },
        vertex_buffer);

        cmd_list.BufferBarrier({
            .sync_before = wis::BarrierSync::All,
            .sync_after = wis::BarrierSync::Draw,
            .access_before = wis::ResourceAccess::Common,
            .access_after = wis::ResourceAccess::ConstantBuffer,
        },
        cbuf);

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
                .size = { 2, 2, 1 },
                .format = wis::DataFormat::BGRA8Unorm,
            }
        };
        cmd_list.CopyBufferToTexture(ubuf2, texture, &region, 1);
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
        cmd_list.Close();

        wis::CommandListView cmd_lists[] = { cmd_list };
        queue.ExecuteCommandLists(cmd_lists, 1);

        WaitForGPU();

        auto [res, hsrv] = device.CreateShaderResource(texture, { .format = wis::DataFormat::BGRA8Unorm, .view_type = wis::TextureViewType::Texture2D, .subresource_range = {
                .base_mip_level = 0,
                .level_count = 1,
                .base_array_layer = 0,
                .layer_count = 1,
            }
        });
        srv = std::move(hsrv);
        desc_buffer.WriteShaderResource(0, 0, 0, 0, root, srv);
        desc_buffer.WriteConstantBuffer(0, 0, 1, 0, root, cbuf, sizeof(float) * 4);
    }

    {
        auto s1 = LoadShader(SHADER_DIR "/example.vs");
        auto s2 = LoadShader(SHADER_DIR "/example.ps");
        auto [result, vs] = device.CreateShader(s1.data(), uint32_t(s1.size()));
        auto [result2, ps] = device.CreateShader(s2.data(), uint32_t(s2.size()));

        vertex_shader = std::move(vs);
        pixel_shader = std::move(ps);
    }

    {
        wis::InputSlotDesc input_slots[] = {
            { .slot = 0, .stride_bytes = sizeof(Vertex), .input_class = wis::InputClass::PerVertex },
        };
        wis::InputAttribute input_attributes[] = {
            { .input_slot = 0, .semantic_name = "POSITION", .semantic_index = 0, .location = 0, .format = wis::DataFormat::RGB32Float, .offset_bytes = 0 },
            { .input_slot = 0, .semantic_name = "TEXCOORD", .semantic_index = 0, .location = 1, .format = wis::DataFormat::RG32Float, .offset_bytes = 12 },
        };
        wis::DataFormat attachment_formats[] = { wis::DataFormat::BGRA8Unorm };

        wis::GraphicsPipelineDesc desc{
            .root_signature = root,
            .input_layout = {
                .slots = input_slots,
                .slot_count = 1,
                .attributes = input_attributes,
                .attribute_count = 2,
            },
            .shaders = { .vertex = vertex_shader, .pixel = pixel_shader },
            .attachments = {
                .attachment_formats = attachment_formats,
                .attachments_count = 1,
            }
        };
        auto [res2, hpipeline] = device.CreateGraphicsPipeline(&desc);
        pipeline = std::move(hpipeline);
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
        auto [res, hsampler] = device.CreateSampler(&sample_desc);
        sampler = std::move(hsampler);

        sampler_buffer.WriteSampler(0, 0, sampler);
    }

    WaitForGPU();
}

void Test::App::OnResize(uint32_t width, uint32_t height)
{
    if (width == this->width && height == this->height)
        return;

    auto result = swap.Resize(width, height);
    if (result.status != wis::Status::Ok)
        throw std::runtime_error("Failed to resize swapchain");

    wis::RenderTargetDesc rt_desc{
        .format = wis::DataFormat::BGRA8Unorm,
        .layout = wis::TextureLayout::Texture2D,
        .mip = 0,
        .base_array_layer = 0,
        .layer_count = 1,
    };

    back_buffers = swap.GetBufferSpan();
    for (size_t i = 0; i < render_targets.size(); i++) {
        auto [res, hrt] = device.CreateRenderTarget(back_buffers[i], rt_desc);
        render_targets[i] = std::move(hrt);
    }
}

void Test::App::Frame()
{
    rotation += 0.01f;
    if (rotation > 1)
        rotation -= 1;

    auto res = cmd_list.Reset(pipeline);
    cmd_list.TextureBarrier({
        .sync_before = wis::BarrierSync::All,
        .sync_after = wis::BarrierSync::Draw,
        .access_before = wis::ResourceAccess::Common,
        .access_after = wis::ResourceAccess::RenderTarget,
        .state_before = wis::TextureState::Present,
        .state_after = wis::TextureState::RenderTarget,
        .subresource_range = {
            .base_mip_level = 0,
            .level_count = 1,
            .base_array_layer = 0,
            .layer_count = 1,
        },
    },
    back_buffers[swap.GetCurrentIndex()]);

    wis::RenderPassRenderTargetDesc targets{
        .target = render_targets[swap.GetCurrentIndex()],
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

    cmd_list.SetRootConstants(&rotation, 1, 0, wis::ShaderStages::Vertex);

    cmd_list.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);

    cmd_list.IASetVertexBuffers(&vertex_binding, 1);
    cmd_list.RSSetViewport({ 0, 0, float(width), float(height), 0, 1 });
    cmd_list.RSSetScissor({ 0, 0, int(width), int(height) });

    cmd_list.DrawInstanced(3);
    cmd_list.EndRenderPass();

    cmd_list.TextureBarrier({
        .sync_before = wis::BarrierSync::Draw,
        .sync_after = wis::BarrierSync::All,
        .access_before = wis::ResourceAccess::RenderTarget,
        .access_after = wis::ResourceAccess::Common,
        .state_before = wis::TextureState::RenderTarget,
        .state_after = wis::TextureState::Present,
        .subresource_range = {
            .base_mip_level = 0,
            .level_count = 1,
            .base_array_layer = 0,
            .layer_count = 1,
        },
    },
    back_buffers[swap.GetCurrentIndex()]);
    cmd_list.Close();

    wis::CommandListView lists[] = { cmd_list };
    queue.ExecuteCommandLists(lists, 1);

    auto result = swap.Present2(true);
    if (result.status != wis::Status::Ok && result.status != wis::Status::Occluded)
        throw std::runtime_error("Failed to present swapchain");

    WaitForGPU();
}

void Test::App::WaitForGPU()
{
    const uint64_t vfence = fence_value;
    std::ignore = queue.SignalQueue(fence, vfence);
    fence_value++;
    std::ignore = fence.Wait(vfence);
}

void Test::App::CreateRootSignature()
{
    wis::RootConstant root_constants[] = {
        {
            .stage = wis::ShaderStages::Vertex,
            .size_bytes = 4,
        },
    };

    wis::DescriptorTableEntry entries[] = {
        {
            .type = wis::DescriptorType::ShaderResource,
            .bind_register = 0,
            .binding = 0,
            .count = 1,
        },
        {
            .type = wis::DescriptorType::ConstantBuffer,
            .bind_register = 0,
            .binding = 1,
            .count = 1,
        },
        {
            .type = wis::DescriptorType::Sampler,
            .bind_register = 0,
            .binding = 0,
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
            .entry_count = 1,
            .stage = wis::ShaderStages::Pixel,
        },

    };
    auto [result, hroot] = device.CreateRootSignature(root_constants, sizeof(root_constants) / sizeof(root_constants[0]), tables, sizeof(tables) / sizeof(tables[0]));
    root = std::move(hroot);
}
