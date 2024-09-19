#include "transfer_node.h"
#include <stb_image_write.h>

std::expected<TransferNode, std::string_view>
CreateTransferNode(wis::Adapter&& adapter)
{
    TransferNode node;

    // Create transfer device
    {
        auto [result, device] = wis::CreateDevice(adapter);
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
        if (result.status != wis::Status::Ok)
            return std::unexpected(result.error);
        node.cmd_list = std::move(cmd_list);
    }
    return std::move(node);
}

void TransferNode::InitSwapchain(wis::SwapChain&& swap)
{
    back_buffers = swap.GetBufferSpan();
    this->swap = std::move(swap);
}

void TransferNode::Resize(uint32_t width, uint32_t height)
{
    auto result = swap.Resize(width, height);
    if (result.status != wis::Status::Ok)
        return;

    back_buffers = swap.GetBufferSpan();
    this->width = width;
    this->height = height;
}

void TransferNode::VKCreateInputBuffer(wis::Size2D frame)
{
    using namespace wis;

    auto [result, buffer] = allocator.CreateBuffer(wis::detail::aligned_size(uint64_t(frame.width * frame.height * 4), 4096ull),
                            wis::BufferUsage::CopySrc,
                            wis::MemoryType::Readback,
                            wis::MemoryFlags::Mapped);

    if (result.status != wis::Status::Ok)
        return;

    input_buffer = std::move(buffer);
}

void TransferNode::WaitForGPU() noexcept
{
    const uint64_t vfence = fence_value;
    queue.SignalQueue(fence, vfence);
    fence_value++;
    std::ignore = fence.Wait(vfence);
}

void TransferNode::Frame()
{
    std::ignore = cmd_list.Reset();
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
    wis::TextureBarrier input_barrier{
        .sync_before = wis::BarrierSync::None,
        .sync_after = wis::BarrierSync::Copy,
        .access_before = wis::ResourceAccess::NoAccess,
        .access_after = wis::ResourceAccess::CopyDest,
        .state_before = wis::TextureState::Present,
        .state_after = wis::TextureState::CopyDest,
        .subresource_range = { 0, 1, 0, 1 }
    };
    wis::TextureBarrier output_barrier{
        .sync_before = wis::BarrierSync::Copy,
        .sync_after = wis::BarrierSync::None,
        .access_before = wis::ResourceAccess::CopyDest,
        .access_after = wis::ResourceAccess::Common,
        .state_before = wis::TextureState::CopyDest,
        .state_after = wis::TextureState::Present,
        .subresource_range = { 0, 1, 0, 1 }
    };

    uint32_t index = swap.GetCurrentIndex();

    cmd_list.BufferBarrier(source_barrier, input_buffer);
    cmd_list.TextureBarrier(input_barrier, back_buffers[index]);

    wis::BufferTextureCopyRegion region{
        .texture = {
            .size = { width, height, 1 },
            .format = wis::DataFormat::RGBA8Unorm,
        }
    };
    cmd_list.CopyBufferToTexture(input_buffer, back_buffers[index], &region, 1);

    cmd_list.TextureBarrier(output_barrier, back_buffers[index]);
    cmd_list.BufferBarrier(dest_barrier, input_buffer);
    cmd_list.Close();

    wis::CommandListView cmd_list_view{ cmd_list };
    queue.ExecuteCommandLists(&cmd_list_view, 1);

    auto result = swap.Present();

    WaitForGPU();
}
