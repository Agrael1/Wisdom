#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_command_list.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xvulkan/vk_checks.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>
#include <wisdom/util/small_allocator.h>

void wis::VKCommandList::CopyBuffer(VKBufferView source, VKBufferView destination, wis::BufferRegion region) const noexcept
{
    VkBufferCopy copy{
        .srcOffset = region.src_offset,
        .dstOffset = region.dst_offset,
        .size = region.size_bytes,
    };
    device.table().vkCmdCopyBuffer(command_list, std::get<0>(source), std::get<0>(destination), 1, &copy);
}

wis::Result wis::VKCommandList::Reset(VKPipelineHandle new_pipeline) noexcept
{
    Close();

    auto& dtable = device.table();

    auto result = dtable.vkResetCommandBuffer(command_list, {});
    if (!succeeded(result)) {
        return make_result<FUNC, "vkResetCommandBuffer failed">(result);
    }
    pipeline = std::move(std::get<0>(new_pipeline));

    VkCommandBufferBeginInfo desc{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = {},
        .pInheritanceInfo = nullptr,
    };
    result = dtable.vkBeginCommandBuffer(command_list, &desc);
    if (!succeeded(result)) {
        return make_result<FUNC, "vkBeginCommandBuffer failed">(result);
    }
    closed = false;
    if (pipeline)
        dtable.vkCmdBindPipeline(command_list, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
    return wis::success;
}

bool wis::VKCommandList::Close() noexcept
{
    if (closed) {
        return true;
    }
    return closed = succeeded(device.table().vkEndCommandBuffer(command_list));
}

namespace wis::detail {
inline VkBufferMemoryBarrier2 to_vk(wis::BufferBarrier barrier, VkBuffer buffer) noexcept
{
    return VkBufferMemoryBarrier2{
        .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = convert_vk(barrier.sync_before),
        .srcAccessMask = convert_vk(barrier.access_before),
        .dstStageMask = convert_vk(barrier.sync_after),
        .dstAccessMask = convert_vk(barrier.access_after),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .buffer = buffer,
        .offset = barrier.offset,
        .size = barrier.size,
    };
}
inline VkImageMemoryBarrier2 to_vk(wis::TextureBarrier barrier, VkImage texture, VkFormat format) noexcept
{
    auto& subresource = barrier.subresource_range;
    return VkImageMemoryBarrier2{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = convert_vk(barrier.sync_before),
        .srcAccessMask = convert_vk(barrier.access_before),
        .dstStageMask = convert_vk(barrier.sync_after),
        .dstAccessMask = convert_vk(barrier.access_after),
        .oldLayout = convert_vk(barrier.state_before),
        .newLayout = convert_vk(barrier.state_after),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = texture,
        .subresourceRange = {
                .aspectMask = aspect_flags(format),
                .baseMipLevel = subresource.base_mip_level,
                .levelCount = subresource.level_count,
                .baseArrayLayer = subresource.base_array_layer,
                .layerCount = subresource.layer_count,
        }
    };
}
} // namespace wis::detail

void wis::VKCommandList::BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer) noexcept
{
    auto hbuffer = std::get<0>(buffer);
    if (!hbuffer)
        return;

    VkBufferMemoryBarrier2 desc = detail::to_vk(barrier, hbuffer);
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .bufferMemoryBarrierCount = 1,
        .pBufferMemoryBarriers = &desc,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::BufferBarriers(const wis::VKBufferBarrier2* barriers, uint32_t barrier_count) noexcept
{
    wis::detail::limited_allocator<VkBufferMemoryBarrier2, 8> allocator(barrier_count, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < barrier_count; i++) {
        data[i] = detail::to_vk(barriers[i].barrier, std::get<0>(barriers[i].buffer));
    }
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .bufferMemoryBarrierCount = barrier_count,
        .pBufferMemoryBarriers = data,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture) noexcept
{
    auto htexture = std::get<0>(texture);
    if (!htexture)
        return;

    VkImageMemoryBarrier2 image_memory_barrier = detail::to_vk(barrier, htexture, std::get<1>(texture));
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &image_memory_barrier,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::TextureBarriers(const wis::VKTextureBarrier2* barriers, uint32_t barrier_count) noexcept
{
    wis::detail::limited_allocator<VkImageMemoryBarrier2, 8> allocator(barrier_count, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < barrier_count; i++) {
        data[i] = detail::to_vk(barriers[i].barrier, std::get<0>(barriers[i].texture), std::get<1>(barriers[i].texture));
    }
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .imageMemoryBarrierCount = barrier_count,
        .pImageMemoryBarriers = data,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::BeginRenderPass(const wis::VKRenderPassDesc* pass_desc) noexcept
{
    if (!pass_desc->target_count)
        return;

    auto& dtable = device.table();
    wis::detail::limited_allocator<VkRenderingAttachmentInfo, 8> allocator(pass_desc->target_count, true);
    auto* data = allocator.data();
    wis::Size2D extent = std::get<1>(pass_desc->targets[0].target);

    for (size_t i = 0; i < pass_desc->target_count; i++) {
        auto& target = pass_desc->targets[i];
        data[i] = VkRenderingAttachmentInfo{
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = std::get<0>(target.target),
            .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp = convert_vk(target.load_op),
            .storeOp = convert_vk(target.store_op),
        };
        if (data[i].loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
            data[i].clearValue = {
                .color = { .float32{ target.clear_value[0], target.clear_value[1], target.clear_value[2], target.clear_value[3] } }
            };
    }

    VkRenderingInfo info{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .pNext = nullptr,
        .flags = convert_vk(pass_desc->flags),
        .renderArea = {
                .offset = { 0, 0 },
                .extent = { extent.width, extent.height },
        },
        .layerCount = 1,
        .viewMask = 0,
        .colorAttachmentCount = pass_desc->target_count,
        .pColorAttachments = data,
    };

    dtable.vkCmdBeginRendering(command_list, &info);
}

void wis::VKCommandList::EndRenderPass() noexcept
{
    device.table().vkCmdEndRendering(command_list);
}

void wis::VKCommandList::RSSetViewport(wis::Viewport vp) noexcept
{
    VkViewport vkvp{
        .x = vp.top_leftx,
        .y = vp.top_lefty,
        .width = vp.width,
        .height = vp.height,
        .minDepth = vp.min_depth,
        .maxDepth = vp.max_depth,
    };
    device.table().vkCmdSetViewport(command_list, 0, 1, &vkvp);
}
void wis::VKCommandList::RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept
{
    static_assert(sizeof(VkViewport) == sizeof(wis::Viewport));
    device.table().vkCmdSetViewport(command_list, 0, count, reinterpret_cast<const VkViewport*>(vp));
}

void wis::VKCommandList::RSSetScissor(wis::Scissor scissor) noexcept
{
    VkRect2D rect{
        .offset = { scissor.left, scissor.top },
        .extent = { uint32_t(scissor.right - scissor.left), uint32_t(scissor.bottom - scissor.top) },
    };
    device.table().vkCmdSetScissor(command_list, 0, 1, &rect);
}

void wis::VKCommandList::IASetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
{
    device.table().vkCmdSetPrimitiveTopology(command_list, convert_vk(topology));
}

void wis::VKCommandList::SetRootSignature(wis::VKRootSignatureView root_signature) noexcept
{
    pipeline_layout = std::get<0>(root_signature);
}