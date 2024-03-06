#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_command_list.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xvulkan/vk_checks.h>

void wis::VKCommandList::CopyBuffer(VKBufferView source, VKBufferView destination, wis::BufferRegion region) const noexcept
{
    VkBufferCopy copy{
        .srcOffset = region.src_offset,
        .dstOffset = region.dst_offset,
        .size = region.size_bytes,
    };
    device.table().vkCmdCopyBuffer(command_list, std::get<0>(source), std::get<0>(destination), 1, &copy);
}

wis::Result wis::VKCommandList::Reset(VKPipelineHandle new_pipeline) noexcept {
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

bool wis::VKCommandList::Close() noexcept {
    if (closed) {
        return true;
    }
    return closed = succeeded(device.table().vkEndCommandBuffer(command_list));
}