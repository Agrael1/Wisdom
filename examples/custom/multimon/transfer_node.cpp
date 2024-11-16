#include "transfer_node.h"
#include <stb_image_write.h>
#include <filesystem>
#include <fstream>

static std::expected<std::string, std::string_view>
LoadShader(std::filesystem::path p) noexcept
{
    if constexpr (wis::shader_intermediate == wis::ShaderIntermediate::DXIL)
        p += u".cso";
    else
        p += u".spv";

    if (!std::filesystem::exists(p))
        return std::expected<std::string, std::string_view>{
            std::unexpect, "File not found"
        };

    std::ifstream t{ p, std::ios::binary };
    if (!t.is_open())
        return std::expected<std::string, std::string_view>{
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

std::expected<TransferNode, std::string_view>
CreateTransferNode(wis::Adapter&& adapter)
{
    TransferNode node;

    // Create transfer device
    {
        wis::DeviceExtension* exts[]{
            &node.ext_mem_host,
            &node.ext_alloc,
            &node.desc_buffer_ext
        };
        auto [result, device] = wis::CreateDevice(adapter, exts, std::size(exts));
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);

        node.transfer_device = std::move(device);
    }

    // Create allocator
    {
        auto [result, allocator] = node.transfer_device.CreateAllocator();
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);

        node.allocator = std::move(allocator);
    }

    // Create main queue
    {
        // Has to be graphics queue for copy operations, because present
        auto [result, queue] = node.transfer_device.CreateCommandQueue(wis::QueueType::Graphics);
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);

        node.queue = std::move(queue);
    }

    // Create Fence
    {
        auto [result, fence] = node.transfer_device.CreateFence();
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);
        node.fence = std::move(fence);
    }

    // Create Command List
    {
        auto [result, cmd_list] = node.transfer_device.CreateCommandList(wis::QueueType::Graphics);
        auto [result2, cmd_list2] = node.transfer_device.CreateCommandList(wis::QueueType::Graphics);
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);
        if (result2.status != wis::Status::Ok)
            return std::unexpected(result2.error);
        node.cmd_list = std::move(cmd_list);
        node.copy_cmd_list = std::move(cmd_list2);
    }

    // Create shaders
    {
        auto sv = LoadShader("present.vs");
        auto sp = LoadShader("present.ps");

        if (!sv)
            return std::unexpected(sp.error());
        if (!sp)
            return std::unexpected(sp.error());

        auto [res, vertex_shader] = node.transfer_device.CreateShader(sv.value().data(), sv.value().size());
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);

        auto [res2, pixel_shader] = node.transfer_device.CreateShader(sp.value().data(), sp.value().size());
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);

        node.vs = std::move(vertex_shader);
        node.ps = std::move(pixel_shader);
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
                    .entry_count = 1,
                    .stage = wis::ShaderStages::Pixel,
            },
            {
                    .type = wis::DescriptorHeapType::Sampler,
                    .entries = entries + 1,
                    .entry_count = 1,
                    .stage = wis::ShaderStages::Pixel,
            },

        };
        wis::RootConstant constants[] = {
            {
                    .stage = wis::ShaderStages::Vertex,
                    .size_bytes = 4 * sizeof(float),
            },
        };
        auto [result, root] = node.desc_buffer_ext.CreateRootSignature(constants, 1, tables, sizeof(tables) / sizeof(tables[0]));
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);
        node.root_signature = std::move(root);
    }

    // Create Pipeline
    {
        wis::DataFormat attachment_formats[] = { wis::DataFormat::RGBA8Unorm };

        wis::GraphicsPipelineDesc desc{
            .root_signature = node.root_signature,
            .shaders = { .vertex = node.vs, .pixel = node.ps },
            .attachments = {
                    .attachment_formats = attachment_formats,
                    .attachments_count = 1,
            },
            .flags = wis::PipelineFlags::DescriptorBuffer,
        };
        auto [res2, hpipeline] = node.transfer_device.CreateGraphicsPipeline(&desc);
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);
        node.pipeline_state = std::move(hpipeline);
    }

    // Create Sampler
    {
        wis::SamplerDesc desc{
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
        auto [r, s] = node.transfer_device.CreateSampler(&desc);
        if (r.status != wis::Status::Ok)
            return std::unexpected(r.error);
        node.sampler = std::move(s);
    }

    // Create Descriptor Buffers
    {
        auto table_alignment = node.desc_buffer_ext.GetDescriptorTableAlignment(wis::DescriptorHeapType::Descriptor);
        auto unit_size = node.desc_buffer_ext.GetDescriptorSize(wis::DescriptorHeapType::Descriptor);

        auto [res, hdesc] = node.desc_buffer_ext.CreateDescriptorBuffer(wis::DescriptorHeapType::Descriptor, wis::DescriptorMemory::ShaderVisible, unit_size);
        if (res.status != wis::Status::Ok)
            return std::unexpected(res.error);

        unit_size = node.desc_buffer_ext.GetDescriptorSize(wis::DescriptorHeapType::Sampler);
        auto [res2, hdesc2] = node.desc_buffer_ext.CreateDescriptorBuffer(wis::DescriptorHeapType::Sampler, wis::DescriptorMemory::ShaderVisible, unit_size);
        if (res2.status != wis::Status::Ok)
            return std::unexpected(res2.error);

        // Set Descriptor Buffers
        hdesc2.WriteSampler(0, 0, node.sampler);

        node.desc_buffer = std::move(hdesc);
        node.sampler_buffer = std::move(hdesc2);
    }

    return std::move(node);
}

void TransferNode::InitSwapchain(wis::SwapChain&& swap, wis::SwapChain&& swap2)
{
    back_buffers[0] = swap.GetBufferSpan();
    back_buffers[1] = swap2.GetBufferSpan();
    this->swap[0] = std::move(swap);
    this->swap[1] = std::move(swap2);

    // Create Render Target
    wis::RenderTargetDesc rt_desc{
        .format = wis::DataFormat::RGBA8Unorm,
    };

    for (size_t i = 0; i < 2; i++) {
        auto [r, rt] = transfer_device.CreateRenderTarget(back_buffers[0][i], rt_desc);
        render_targets[0][i] = std::move(rt);
    }
    for (size_t i = 0; i < 2; i++) {
        auto [r, rt] = transfer_device.CreateRenderTarget(back_buffers[1][i], rt_desc);
        render_targets[1][i] = std::move(rt);
    }
}

void TransferNode::Resize(uint32_t width, uint32_t height, uint32_t swapn)
{
    wis::Size2D xframe_size = { width, height };
    auto result = swap[swapn].Resize(width, height);
    if (result.status != wis::Status::Ok)
        return;

    back_buffers[swapn] = swap[swapn].GetBufferSpan();
    frame_size[swapn] = xframe_size;

    // Create Render Target
    wis::RenderTargetDesc rt_desc{
        .format = wis::DataFormat::RGBA8Unorm,
    };

    for (size_t i = 0; i < 2; i++) {
        auto [r, rt] = transfer_device.CreateRenderTarget(back_buffers[swapn][i], rt_desc);
        render_targets[swapn][i] = std::move(rt);
    }
}

void TransferNode::VKImportFrame(wis::Size2D frame, void* mapping)
{
    using namespace wis;
    // create an external buffer
    auto [r, b] = ext_mem_host.CreateExternalBuffer(allocator, mapping, frame.width * frame.height * 4);
    input_buffer = std::move(b);

    // create debug buffer
    auto [r1, b1] = allocator.CreateReadbackBuffer(frame.width * frame.height * 4);
    debug_buffer = std::move(b1);

    // create a texture
    wis::TextureDesc desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .size = { frame.width, frame.height, 1 },
        .usage = wis::TextureUsage::CopyDst | wis::TextureUsage::ShaderResource,
    };
    auto [r2, t] = allocator.CreateTexture(desc);
    texture = std::move(t);

    // create a shader resource view
    wis::ShaderResourceDesc srv_desc{
        .format = wis::DataFormat::RGBA8Unorm,
        .view_type = wis::TextureViewType::Texture2D,
        .subresource_range = { 0, 1, 0, 1 },
    };
    auto [r3, srv] = transfer_device.CreateShaderResource(texture, srv_desc);
    texture_srv = std::move(srv);

    // write data to Descriptor Buffer
    desc_buffer.WriteShaderResource(0, 0, texture_srv);
    input_size = frame;

    std::ignore = cmd_list.Reset();
    wis::BufferBarrier source_barrier{
        .sync_before = wis::BarrierSync::None,
        .sync_after = wis::BarrierSync::Copy,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::CopySource,
    };
    wis::BufferBarrier source_barrier2{
        .sync_before = wis::BarrierSync::None,
        .sync_after = wis::BarrierSync::Copy,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::CopyDest,
    };
    wis::TextureBarrier tex_barrier{
        .sync_before = wis::BarrierSync::None,
        .sync_after = wis::BarrierSync::PixelShading,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::ShaderResource,
        .state_before = wis::TextureState::Undefined,
        .state_after = wis::TextureState::ShaderResource,
        .subresource_range = {
                0,
                1,
                0,
                1 }
    };

    cmd_list.BufferBarrier(source_barrier, input_buffer);
    cmd_list.BufferBarrier(source_barrier2, debug_buffer);
    cmd_list.TextureBarrier(tex_barrier, texture);
    cmd_list.Close();
    wis::CommandListView cmd_list_view{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view, 1);
    WaitForGPU();
}

void TransferNode::WaitForGPU() noexcept
{
    const uint64_t vfence = fence_value;
    std::ignore = queue.SignalQueue(fence, vfence);
    fence_value++;
    std::ignore = fence.Wait(vfence);
}

void TransferNode::Frame()
{
    uint32_t index[] = { swap[0].GetCurrentIndex(), swap[1].GetCurrentIndex() };

    std::ignore = copy_cmd_list.Reset();
    wis::BufferBarrier source_barrier{
        .sync_before = wis::BarrierSync::None,
        .sync_after = wis::BarrierSync::Copy,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::CopySource,
    };
    wis::BufferBarrier dest_barrier{
        .sync_before = wis::BarrierSync::Copy,
        .sync_after = wis::BarrierSync::None,
        .access_before = wis::ResourceAccess::CopySource,
        .access_after = wis::ResourceAccess::NoAccess,
    };

    wis::TextureBarrier2 ainput_barriers[] = {
        { .barrier{ .sync_before = wis::BarrierSync::None,
                    .sync_after = wis::BarrierSync::Copy,
                    .access_before = wis::ResourceAccess::NoAccess,
                    .access_after = wis::ResourceAccess::CopyDest,
                    .state_before = wis::TextureState::ShaderResource,
                    .state_after = wis::TextureState::CopyDest,
                    .subresource_range = { 0, 1, 0, 1 } },
          .texture = texture }
    };
    wis::TextureBarrier2 inter_barriers[] = {
        { .barrier{ .sync_before = wis::BarrierSync::Copy,
                    .sync_after = wis::BarrierSync::Draw,
                    .access_before = wis::ResourceAccess::CopyDest,
                    .access_after = wis::ResourceAccess::ShaderResource,
                    .state_before = wis::TextureState::CopyDest,
                    .state_after = wis::TextureState::ShaderResource,
                    .subresource_range = { 0, 1, 0, 1 } },
          .texture = texture },
    };

    copy_cmd_list.TextureBarriers(ainput_barriers, std::size(ainput_barriers));

    wis::BufferTextureCopyRegion region{
        .texture = {
                .size = { input_size.width, input_size.height, 1 },
                .format = wis::DataFormat::RGBA8Unorm,
        }
    };
    copy_cmd_list.CopyBufferToTexture(input_buffer, texture, &region, 1);
    copy_cmd_list.TextureBarriers(inter_barriers, std::size(inter_barriers));
    copy_cmd_list.Close();

    wis::CommandListView cmd_list_view[]{ copy_cmd_list, /*cmd_list*/ };
    queue.ExecuteCommandLists(cmd_list_view, std::size(cmd_list_view));
    WaitForGPU();

    // Command List
    std::ignore = cmd_list.Reset(pipeline_state);

    wis::TextureBarrier2 input_barriers[] = {
        { .barrier{ .sync_before = wis::BarrierSync::None,
                    .sync_after = wis::BarrierSync::Draw,
                    .access_before = wis::ResourceAccess::NoAccess,
                    .access_after = wis::ResourceAccess::RenderTarget,
                    .state_before = wis::TextureState::Present,
                    .state_after = wis::TextureState::RenderTarget,
                    .subresource_range = { 0, 1, 0, 1 } },
          .texture = back_buffers[0][index[0]] },
        { .barrier{ .sync_before = wis::BarrierSync::None,
                    .sync_after = wis::BarrierSync::Draw,
                    .access_before = wis::ResourceAccess::NoAccess,
                    .access_after = wis::ResourceAccess::RenderTarget,
                    .state_before = wis::TextureState::Present,
                    .state_after = wis::TextureState::RenderTarget,
                    .subresource_range = { 0, 1, 0, 1 } },
          .texture = back_buffers[1][index[1]] },
    };

    wis::TextureBarrier2 output_barriers[] = {
        { .barrier{ .sync_before = wis::BarrierSync::Draw,
                    .sync_after = wis::BarrierSync::None,
                    .access_before = wis::ResourceAccess::RenderTarget,
                    .access_after = wis::ResourceAccess::Common,
                    .state_before = wis::TextureState::RenderTarget,
                    .state_after = wis::TextureState::Present,
                    .subresource_range = { 0, 1, 0, 1 } },
          .texture = back_buffers[0][index[0]] },
        { .barrier{ .sync_before = wis::BarrierSync::Draw,
                    .sync_after = wis::BarrierSync::None,
                    .access_before = wis::ResourceAccess::RenderTarget,
                    .access_after = wis::ResourceAccess::Common,
                    .state_before = wis::TextureState::RenderTarget,
                    .state_after = wis::TextureState::Present,
                    .subresource_range = { 0, 1, 0, 1 } },
          .texture = back_buffers[1][index[1]] },
    };

    cmd_list.TextureBarriers(input_barriers, std::size(input_barriers));
    cmd_list.SetRootSignature(root_signature);
    desc_buffer_ext.SetDescriptorBuffers(cmd_list, desc_buffer, sampler_buffer);

    // Render Pass 1
    wis::RenderPassRenderTargetDesc rprtdesc{
        .target = render_targets[0][index[0]],
        .load_op = wis::LoadOperation::Clear,
        .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f },
    };
    wis::RenderPassDesc rpdesc{
        .target_count = 1,
        .targets = &rprtdesc,
    };
    cmd_list.BeginRenderPass(&rpdesc);
    cmd_list.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
    cmd_list.RSSetViewport({ 0, 0, float(frame_size[0].width), float(frame_size[0].height), 0, 1 });
    cmd_list.RSSetScissor({ 0, 0, int(frame_size[0].width), int(frame_size[0].height) });

    struct RC {
        float offset[2];
        float size[2];
    } rc{ { 0.0f, 0.0f }, { 0.5, 1 } };

    cmd_list.SetRootConstants(&rc, sizeof(rc) / 4, 0, wis::ShaderStages::Vertex);
    desc_buffer_ext.SetDescriptorTableOffset(cmd_list, root_signature, 0, desc_buffer, 0);
    desc_buffer_ext.SetDescriptorTableOffset(cmd_list, root_signature, 1, sampler_buffer, 0);

    cmd_list.DrawInstanced(3, 1, 0, 0);
    cmd_list.EndRenderPass();

    // Render Pass 2
    rprtdesc.target = render_targets[1][index[1]];
    cmd_list.BeginRenderPass(&rpdesc);
    cmd_list.IASetPrimitiveTopology(wis::PrimitiveTopology::TriangleList);
    cmd_list.RSSetViewport({ 0, 0, float(frame_size[1].width), float(frame_size[1].height), 0, 1 });
    cmd_list.RSSetScissor({ 0, 0, int(frame_size[1].width), int(frame_size[1].height) });

    rc.offset[0] = 0.5f;
    cmd_list.SetRootConstants(&rc, sizeof(rc) / 4, 0, wis::ShaderStages::Vertex);
    desc_buffer_ext.SetDescriptorTableOffset(cmd_list, root_signature, 0, desc_buffer, 0);
    desc_buffer_ext.SetDescriptorTableOffset(cmd_list, root_signature, 1, sampler_buffer, 0);

    cmd_list.DrawInstanced(3, 1, 0, 0);
    cmd_list.EndRenderPass();

    cmd_list.TextureBarriers(output_barriers, std::size(output_barriers));
    cmd_list.Close();

    wis::CommandListView cmd_list_view3[]{ cmd_list };
    queue.ExecuteCommandLists(cmd_list_view3, std::size(cmd_list_view3));

    auto result = swap[0].Present();
    result = swap[1].Present();

    result = swap[0].WaitForPresent();
    result = swap[1].WaitForPresent();

    WaitForGPU();
}

void TransferNode::BlitFrame(uint32_t swapn)
{
    auto& cmd_i = cmd_list.GetInternal();
}
