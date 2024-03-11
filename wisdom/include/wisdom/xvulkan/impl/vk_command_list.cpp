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

void wis::VKCommandList::BufferBarriers(wis::VKBufferBarrier2* barriers, uint32_t barrier_count) noexcept
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

void wis::VKCommandList::TextureBarriers(wis::VKTextureBarrier2* barriers, uint32_t barrier_count) noexcept
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