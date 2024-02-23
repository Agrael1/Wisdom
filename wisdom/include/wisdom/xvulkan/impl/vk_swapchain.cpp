#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_swapchain.h>
#endif // !WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_checks.h>

#include <wisdom/util/misc.h>
#include <wisdom/util/small_allocator.h>

wis::Result wis::detail::VKSwapChainCreateInfo::InitBackBuffers() noexcept
{
    auto& table = *swapchain.header().parent.table();
    uint32_t new_back_buffer_count = 0;
    auto result = table.vkGetSwapchainImagesKHR(swapchain.header().parent.get(), swapchain.get(), &new_back_buffer_count, nullptr);

    if (new_back_buffer_count > back_buffer_count) {
        back_buffers = wis::detail::make_unique_for_overwrite<VKTexture[]>(new_back_buffer_count);
        if (!back_buffers)
            return { wis::make_result<FUNC, "failed to allocate back_buffers array">(result) };

        back_buffer_count = new_back_buffer_count;
    }

    wis::detail::limited_allocator<VkImage> allocator{ new_back_buffer_count };
    result = table.vkGetSwapchainImagesKHR(swapchain.header().parent.get(), swapchain.get(), &new_back_buffer_count, allocator.data());

    if (!wis::succeeded(result))
        return { wis::make_result<FUNC, "vkGetSwapchainImagesKHR failed">(result) };

    for (uint32_t i = 0; i < back_buffer_count; ++i) {
        back_buffers[i] = VKTexture{ format, allocator.data()[i] };
    }
    return AquireNextIndex();
}

wis::Result wis::detail::VKSwapChainCreateInfo::AquireNextIndex() noexcept
{
    auto& table = *swapchain.header().parent.table();
    auto device = swapchain.header().parent.get();
    auto result = table.vkAcquireNextImageKHR(device, swapchain.get(), std::numeric_limits<uint64_t>::max(), present_semaphore, nullptr, &present_index);

    VkPipelineStageFlags stage_mask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkSubmitInfo signal_submit_info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &present_semaphore,
        .pWaitDstStageMask = &stage_mask,
    };
    result = table.vkQueueSubmit(present_queue, 1, &signal_submit_info, nullptr);
    return wis::succeeded(result) ? wis::success : wis::make_result<FUNC, "vkAcquireNextImageKHR failed">(result);
}
wis::Result wis::detail::VKSwapChainCreateInfo::InitSemaphores() noexcept
{
    auto& table = *swapchain.header().parent.table();
    VkSemaphoreCreateInfo semaphore_info{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };
    auto result = table.vkCreateSemaphore(swapchain.header().parent.get(), &semaphore_info, nullptr, &present_semaphore);
    if (!wis::succeeded(result))
        return { wis::make_result<FUNC, "vkCreateSemaphore failed for present_semaphore">(result) };

    result = table.vkCreateSemaphore(swapchain.header().parent.get(), &semaphore_info, nullptr, &graphics_semaphore);
    return wis::succeeded(result) ? wis::success : wis::make_result<FUNC, "vkCreateSemaphore failed for graphics_semaphore">(result);
}

void wis::detail::VKSwapChainCreateInfo::ReleaseSemaphore() noexcept
{
    VkSubmitInfo signal_submit_info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &present_semaphore,
    };
    auto& table = *swapchain.header().parent.table();
    table.vkQueueSubmit(present_queue, 1, &signal_submit_info, nullptr);
    table.vkQueueWaitIdle(present_queue);
}