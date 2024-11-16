#include "work_node.h"
#include "lut_loader.h"
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

static std::expected<std::string, std::string_view>
LoadShader(std::filesystem::path p) noexcept
{
    if constexpr (wis::shader_intermediate == wis::ShaderIntermediate::DXIL)
        p += u".cso";
    else
        p += u".spv";

    if (!std::filesystem::exists(p))
        return std::expected<std::string, std::string_view> {
        std::unexpect, "File not found"
    };

    std::ifstream t{ p, std::ios::binary };
    if (!t.is_open())
        return std::expected<std::string, std::string_view> {
        std::unexpect, "Failed to open file"
    };

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string ret;
    ret.resize(size);
    t.seekg(0);
    t.read(ret.data(), size);
    return ret;
}

//--------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------

std::expected<WorkNode, std::string_view>
CreateWorkNode(wis::Adapter&& adapter)
{
    WorkNode node{};
    wis::DeviceExtension* extensions[] = {
        &node.extended_alloc,
        &node.desc_buffer_ext
    };

    // Create Device
    {
        auto [res, device] = wis::CreateDevice(adapter, extensions, std::size(extensions));
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.work_device = std::move(device);
    }

    // Create Graphics Queue
    {
        auto [res, queue] = node.work_device.CreateCommandQueue(wis::QueueType::Graphics);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.queue = std::move(queue);
    }

    // Create Resource Allocator
    {
        auto [res, allocator] = node.work_device.CreateAllocator();
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.allocator = std::move(allocator);
    }

    // Create LUT Texture
    {
        using namespace wis;
        LutLoader loader;
        auto lut_data = loader.LoadLut("lut.cube");
        if (lut_data.type == LutType::Undefined)
            return std::unexpected("Invalid LUT file");

        wis::TextureDesc desc{
            .format = wis::DataFormat::RGBA32Float,
            .size = { uint32_t(lut_data.stride), (uint32_t)lut_data.stride, (uint32_t)lut_data.stride },
            .layout = wis::TextureLayout::Texture3D,
            .usage = wis::TextureUsage::ShaderResource | wis::TextureUsage::HostCopy
        };
        auto [res, texture] = node.extended_alloc.CreateGPUUploadTexture(node.allocator, desc, wis::TextureState::CopyDest, wis::MemoryFlags::DedicatedAllocation);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);

        wis::TextureRegion region{
            .offset = { 0, 0, 0 },
            .size = { uint32_t(lut_data.stride), uint32_t(lut_data.stride), uint32_t(lut_data.stride) },
            .format = wis::DataFormat::RGBA32Float
        };
        auto res2 = node.extended_alloc.WriteMemoryToSubresourceDirect(lut_data.data.get(), texture, wis::TextureState::CopyDest, region);
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);

        node.lut = std::move(texture);
    }

    // Create Texture
    {
        using namespace wis;
        auto data = LoadPNGImage("image.png");

        wis::TextureDesc desc{
            .format = wis::DataFormat::RGBA8Unorm,
            .size = { data.width, data.height, 1 },
            .layout = wis::TextureLayout::Texture2D,
            .usage = wis::TextureUsage::ShaderResource | wis::TextureUsage::HostCopy
        };
        auto [res, texture] = node.extended_alloc.CreateGPUUploadTexture(node.allocator, desc, wis::TextureState::CopyDest, wis::MemoryFlags::DedicatedAllocation);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);

        wis::TextureRegion region{
            .offset = { 0, 0, 0 },
            .size = { data.width, data.height, 1 },
            .format = wis::DataFormat::RGBA8Unorm
        };
        auto res2 = node.extended_alloc.WriteMemoryToSubresourceDirect(data.data.data(), texture, wis::TextureState::CopyDest, region);
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);

        node.texture = std::move(texture);
    }

    // Create SRV LUT
    {
        using namespace wis;
        ShaderResourceDesc desc{
            .format = wis::DataFormat::RGBA32Float,
            .view_type = wis::TextureViewType::Texture3D,
            .subresource_range = { 0, 1, 0, 1 }
        };
        auto [res, srv] = node.work_device.CreateShaderResource(node.lut, desc);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.srv_lut = std::move(srv);
    }

    // Create SRV Texture
    {
        using namespace wis;
        ShaderResourceDesc desc{
            .format = wis::DataFormat::RGBA8Unorm,
            .view_type = wis::TextureViewType::Texture2D,
            .subresource_range = { 0, 1, 0, 1 }
        };
        auto [res, srv] = node.work_device.CreateShaderResource(node.texture, desc);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.srv = std::move(srv);
    }

    // Create Sampler
    {
        constexpr wis::SamplerDesc sample_desc{
            .min_filter = wis::Filter::Point,
            .mag_filter = wis::Filter::Point,
            .mip_filter = wis::Filter::Point,
            .address_u = wis::AddressMode::ClampToEdge,
            .address_v = wis::AddressMode::ClampToEdge,
            .address_w = wis::AddressMode::ClampToEdge,
            .min_lod = 0,
            .max_lod = 1.0f,
            .mip_lod_bias = 0.0f,
            .comparison_op = wis::Compare::None,
        };
        auto [res, sampler] = node.work_device.CreateSampler(&sample_desc);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.sampler = std::move(sampler);
    }

    // Create Shaders
    {
        auto sv = LoadShader("lut.vs");
        auto sp = LoadShader("lut_tetra.ps");

        if (!sv)
            return std::unexpected(sp.error());
        if (!sp)
            return std::unexpected(sp.error());

        auto [res, vertex_shader] = node.work_device.CreateShader(sv.value().data(), sv.value().size());
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);

        auto [res2, pixel_shader] = node.work_device.CreateShader(sp.value().data(), sp.value().size());
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);

        node.vertex_shader = std::move(vertex_shader);
        node.pixel_shader = std::move(pixel_shader);
    }

    // Create Root Signature
    {
        using namespace wis;
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
        auto [result, root] = node.desc_buffer_ext.CreateRootSignature(nullptr, 0, tables, sizeof(tables) / sizeof(tables[0]));
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);
        node.root = std::move(root);
    }

    // Create Pipeline
    {
        wis::DataFormat attachment_formats[] = { wis::DataFormat::RGBA8Unorm };

        wis::GraphicsPipelineDesc desc{
            .root_signature = node.root,
            .shaders = { .vertex = node.vertex_shader, .pixel = node.pixel_shader },
            .attachments = {
                .attachment_formats = attachment_formats,
                .attachments_count = 1,
            },
            .flags = wis::PipelineFlags::DescriptorBuffer,
        };
        auto [res2, hpipeline] = node.work_device.CreateGraphicsPipeline(&desc);
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);
        node.pipeline = std::move(hpipeline);
    }

    // Create Fence
    {
        auto [res, fence] = node.work_device.CreateFence(0);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.fence = std::move(fence);
    }

    // Create Command List
    {
        auto [res, cmd_list] = node.work_device.CreateCommandList(wis::QueueType::Graphics);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);
        node.cmd_list = std::move(cmd_list);
    }

    // Create Descriptor Buffers
    {
        uint32_t al_d = node.desc_buffer_ext.GetDescriptorTableAlignment(wis::DescriptorHeapType::Descriptor);
        uint32_t al_s = node.desc_buffer_ext.GetDescriptorTableAlignment(wis::DescriptorHeapType::Sampler);
        uint32_t inc_d = node.desc_buffer_ext.GetDescriptorSize(wis::DescriptorHeapType::Descriptor);
        uint32_t inc_s = node.desc_buffer_ext.GetDescriptorSize(wis::DescriptorHeapType::Sampler);

        auto desc_size = wis::detail::aligned_size(inc_d * 2, al_d);
        auto [res, hdesc] = node.desc_buffer_ext.CreateDescriptorBuffer(wis::DescriptorHeapType::Descriptor, wis::DescriptorMemory::ShaderVisible, desc_size);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);

        auto sampler_size = wis::detail::aligned_size(inc_s, al_s);
        auto [res2, hdesc2] = node.desc_buffer_ext.CreateDescriptorBuffer(wis::DescriptorHeapType::Sampler, wis::DescriptorMemory::ShaderVisible, sampler_size);
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);

        // Set Descriptor Buffers
        hdesc.WriteShaderResource(0, 1, node.srv);
        hdesc.WriteShaderResource(0, 0, node.srv_lut);

        hdesc2.WriteSampler(0, 0, node.sampler);

        node.desc_buffer = std::move(hdesc);
        node.sampler_buffer = std::move(hdesc2);
    }

    node.PrepareResources();

    return std::expected<WorkNode, std::string_view> { std::move(node) };
}

void WorkNode::PrepareResources()
{
    std::ignore = cmd_list.Reset();

    wis::TextureBarrier2 barriers[] = {
        {
            .barrier = {
                .access_before = wis::ResourceAccess::NoAccess,
                .access_after = wis::ResourceAccess::NoAccess,
                .state_before = wis::TextureState::Undefined,
                .state_after = wis::TextureState::ShaderResource,
                .subresource_range = { 0, 1, 0, 1 },
            },
            .texture = texture,
        },
        {
            .barrier = {
                .access_before = wis::ResourceAccess::NoAccess,
                .access_after = wis::ResourceAccess::NoAccess,
                .state_before = wis::TextureState::Undefined,
                .state_after = wis::TextureState::ShaderResource,
                .subresource_range = { 0, 1, 0, 1 },
            },
            .texture = lut,
        }
    };
    cmd_list.TextureBarriers(barriers, std::size(barriers));
    cmd_list.Close();

    wis::CommandListView cmd_list_view{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view, 1);
    WaitForGPU();
}

void WorkNode::CreateOutputTexture(wis::Size2D frame)
{
    using namespace wis;
    width = frame.width;
    height = frame.height;

    wis::TextureDesc desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = { frame.width, frame.height, 1 },
        .usage = wis::TextureUsage::RenderTarget | wis::TextureUsage::CopySrc,
    };
    auto [result, tex] = allocator.CreateTexture(desc);
    if (result.status != wis::Status::Ok)
        return;

    // Create buffer
    {
        auto [r, b] = allocator.CreateReadbackBuffer(frame.width * frame.height * 4);
        if (r.status != wis::Status::Ok)
            return;
        out_buffer = std::move(b);
        mapping = out_buffer.Map();
    }

    // Create render target
    {
        wis::RenderTargetDesc rt_desc{
            .format = wis::DataFormat::RGBA8Unorm,
        };
        auto [res, rt] = work_device.CreateRenderTarget(tex, rt_desc);
        if (res.status != wis::Status::Ok)
            return;
        render_target = std::move(rt);
    }

    std::ignore = cmd_list.Reset();
    wis::TextureBarrier barrier{
        .sync_after = wis::BarrierSync::Copy,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::NoAccess,
        .state_before = wis::TextureState::Undefined,
        .state_after = wis::TextureState::CopySource,
        .subresource_range = { 0, 1, 0, 1 },
    };
    cmd_list.TextureBarrier(barrier, tex);
    cmd_list.Close();

    wis::CommandListView cmd_list_view{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view, 1);
    WaitForGPU();

    out_texture = std::move(tex);
}

void WorkNode::WaitForGPU() noexcept
{
    const uint64_t vfence = fence_value;
    queue.SignalQueue(fence, vfence);
    fence_value++;
    std::ignore = fence.Wait(vfence);
}

void WorkNode::Frame()
{
    std::ignore = cmd_list.Reset(pipeline);

    wis::TextureBarrier barrier{
        .sync_before = wis::BarrierSync::None,
        .sync_after = wis::BarrierSync::Draw,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::RenderTarget,
        .state_before = wis::TextureState::CopySource,
        .state_after = wis::TextureState::RenderTarget,
        .subresource_range = { 0, 1, 0, 1 },
    };
    cmd_list.TextureBarrier(barrier, out_texture);

    wis::RenderPassRenderTargetDesc rprtdesc{
        .target = render_target,
        .load_op = wis::LoadOperation::Clear,
        .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f },
    };
    wis::RenderPassDesc rpdesc{
        .target_count = 1,
        .targets = &rprtdesc,
    };
    cmd_list.BeginRenderPass(&rpdesc);
    cmd_list.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
    cmd_list.SetRootSignature(root);
    ;
    desc_buffer_ext.SetDescriptorBuffers(cmd_list, desc_buffer, sampler_buffer);
    desc_buffer_ext.SetDescriptorTableOffset(cmd_list, root, 0, desc_buffer, 0);
    desc_buffer_ext.SetDescriptorTableOffset(cmd_list, root, 1, sampler_buffer, 0);

    cmd_list.RSSetViewport({ 0, 0, float(width), float(height), 0, 1 });
    cmd_list.RSSetScissor({ 0, 0, int(width), int(height) });

    cmd_list.DrawInstanced(3);
    cmd_list.EndRenderPass();
    cmd_list.Close();

    wis::CommandListView cmd_list_view{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view, 1);
    WaitForGPU();

    std::ignore = cmd_list.Reset(pipeline);
    wis::TextureBarrier out_barrier{
        .sync_before = wis::BarrierSync::Draw,
        .sync_after = wis::BarrierSync::Copy,
        .access_before = wis::ResourceAccess::RenderTarget,
        .access_after = wis::ResourceAccess::CopySource,
        .state_before = wis::TextureState::RenderTarget,
        .state_after = wis::TextureState::CopySource,
        .subresource_range = { 0, 1, 0, 1 },
    };
    cmd_list.TextureBarrier(out_barrier, out_texture);

    wis::BufferTextureCopyRegion region{
        .texture = {
            .size = { width, height, 1 },
            .format = wis::DataFormat::RGBA8Unorm,
        }
    };
    cmd_list.CopyTextureToBuffer(out_texture, out_buffer, &region, 1);
    cmd_list.Close();

    wis::CommandListView cmd_list_view2{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view2, 1);
    WaitForGPU();
}

void WorkNode::DumpFrame(const char* name)
{
    stbi_write_png(name, width, height, 4, mapping, width * 4);
}
