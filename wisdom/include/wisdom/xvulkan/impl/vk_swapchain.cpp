#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_swapchain.h>
#endif // !WISDOM_HEADER_ONLY
#include <wisdom/xvulkan/vk_checks.h>

#include <wisdom/util/misc.h>
#include <wisdom/util/small_allocator.h>

wis::Result wis::detail::VKSwapChainCreateInfo::InitBackBuffers() noexcept
{
    auto& table = device.table();
    uint32_t new_back_buffer_count = 0;
    auto result = table.vkGetSwapchainImagesKHR(device.get(), swapchain, &new_back_buffer_count, nullptr);

    if (new_back_buffer_count > back_buffer_count) {
        back_buffers = wis::detail::make_unique_for_overwrite<VKTexture[]>(new_back_buffer_count);
        if (!back_buffers)
            return { wis::make_result<FUNC, "failed to allocate back_buffers array">(result) };

        back_buffer_count = new_back_buffer_count;
    }

    wis::detail::limited_allocator<VkImage> allocator{ new_back_buffer_count };
    result = table.vkGetSwapchainImagesKHR(device.get(), swapchain, &new_back_buffer_count, allocator.data());

    if (!wis::succeeded(result))
        return { wis::make_result<FUNC, "vkGetSwapchainImagesKHR failed">(result) };

    for (uint32_t i = 0; i < back_buffer_count; ++i) {
        back_buffers[i] = VKTexture{ format.format, allocator.data()[i] };
    }
    return AquireNextIndex();
}

wis::Result wis::detail::VKSwapChainCreateInfo::AquireNextIndex() const noexcept
{
    auto& table = device.table();
    auto result = table.vkAcquireNextImageKHR(device.get(), swapchain, std::numeric_limits<uint64_t>::max(), present_semaphore, nullptr, &present_index);
    return wis::succeeded(result) ? wis::success : wis::make_result<FUNC, "vkAcquireNextImageKHR failed">(result);
}
wis::Result wis::detail::VKSwapChainCreateInfo::InitSemaphores() noexcept
{
    auto& table = device.table();
    VkSemaphoreCreateInfo semaphore_info{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };
    auto result = table.vkCreateSemaphore(device.get(), &semaphore_info, nullptr, &present_semaphore);
    if (!wis::succeeded(result))
        return { wis::make_result<FUNC, "vkCreateSemaphore failed for present_semaphore">(result) };

    result = table.vkCreateSemaphore(device.get(), &semaphore_info, nullptr, &graphics_semaphore);
    return wis::succeeded(result) ? wis::success : wis::make_result<FUNC, "vkCreateSemaphore failed for graphics_semaphore">(result);
}

void wis::detail::VKSwapChainCreateInfo::ReleaseSemaphore() const noexcept
{
    auto& dtable = device.table();
    dtable.vkQueueWaitIdle(present_queue);
    dtable.vkQueueWaitIdle(graphics_queue);
}

wis::Result wis::VKSwapChain::Resize(uint32_t width, uint32_t height) noexcept
{
    auto& dtable = device.table();

    VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo wait{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &present_semaphore.handle,
        .pWaitDstStageMask = &wait_stages,
    };
    dtable.vkQueueSubmit(graphics_queue, 1, &wait, nullptr);

    dtable.vkQueueWaitIdle(present_queue);
    dtable.vkQueueWaitIdle(graphics_queue);

    VkSwapchainCreateInfoKHR desc{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = surface.get(),
        .minImageCount = back_buffer_count,
        .imageFormat = format.format,
        .imageColorSpace = format.colorSpace,
        .imageExtent = { width, height },
        .imageArrayLayers = stereo ? 2u : 1u,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = swapchain,
    };

    auto result = dtable.vkCreateSwapchainKHR(device.get(), &desc, nullptr, &swapchain);
    if (!succeeded(result)) {
        return wis::make_result<FUNC, "vkCreateSwapchainKHR failed">(result);
    }

    auto rres = InitBackBuffers();
    return rres;
}

wis::Result wis::VKSwapChain::Present() const noexcept
{
    auto& dtable = device.table();

    VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo desc{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &present_semaphore.handle,
        .pWaitDstStageMask = &wait_stages,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &graphics_semaphore.handle,
    };
    dtable.vkQueueSubmit(graphics_queue, 1, &desc, nullptr);

    VkPresentInfoKHR present_info{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &graphics_semaphore.handle,
        .swapchainCount = 1,
        .pSwapchains = &swapchain.handle,
        .pImageIndices = &present_index,
        .pResults = nullptr,
    };

    auto result = dtable.vkQueuePresentKHR(present_queue, &present_info);
    return wis::succeeded(result)
            ? AquireNextIndex()
            : wis::make_result<FUNC, "vkQueuePresentKHR failed">(result);
}