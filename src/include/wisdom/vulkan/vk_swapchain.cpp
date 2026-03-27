#ifndef WIS_VK_SWAPCHAIN_CPP
#define WIS_VK_SWAPCHAIN_CPP

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <algorithm>

namespace wis::detail {
inline VkResult VKAcquireNextImage(const impl::VKSwapchainImpl& impl) noexcept
{
    auto& swapchain_header = impl.swapchain_header->header;
    auto& swapchain_table  = *impl.swapchain_table;
    auto  semaphores       = swapchain_header.GetImageAvailableSemaphores();

    // Acquire the next image index for the new swapchain to update internal state
    auto result = impl.swapchain_table->vkAcquireNextImageKHR(impl.device,
                                                              impl.swapchain,
                                                              impl.lazy_acquire ? 0 : std::numeric_limits<uint64_t>::max(),
                                                              semaphores[impl.acquire_index],
                                                              nullptr,
                                                              &impl.present_index);

    if (result != VK_SUCCESS) {
        return result; // Caller can choose to handle timeout differently (e.g. by skipping rendering and trying again next frame) so return a distinct result code for this case
    }

    VkPipelineStageFlags2 stage_mask = 0;
    if (swapchain_header.create_info.imageUsage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
        stage_mask |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (swapchain_header.create_info.imageUsage & VK_IMAGE_USAGE_STORAGE_BIT) {
        stage_mask |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (swapchain_header.create_info.imageUsage & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
        stage_mask |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    }

    VkSemaphoreSubmitInfo submit_info{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = semaphores[impl.acquire_index],
        .stageMask = stage_mask,
    };

    VkSubmitInfo2 desc2{
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext                  = nullptr,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos    = &submit_info,
    };
    return swapchain_table.vkQueueSubmit2(impl.present_queue, 1, &desc2, nullptr);
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroySwapchain(WisVKSwapchain* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKSwapchainImpl>(self);

    if (impl.swapchain != VK_NULL_HANDLE) {
        // Enqueue destruction fence
        auto  qtable = impl.swapchain_table;
        auto& dtable = impl.swapchain_header->header.device_header->header.device_table;
        qtable->vkQueueSubmit2(impl.present_queue, 0, nullptr, impl.destroy_fence);
        qtable->vkWaitForFences(impl.device, 1, &impl.destroy_fence, VK_TRUE, std::numeric_limits<uint64_t>::max());

        // Destroy frame semaphores
        auto semaphores = impl.swapchain_header->header.GetSemaphores();
        for (auto sem : semaphores) {
            dtable.vkDestroySemaphore(impl.device, sem, nullptr);
        }

        // Destroy swapchain
        auto& head = impl.swapchain_header->header;
        qtable->vkDestroySwapchainKHR(head.device_header->header.device, impl.swapchain, nullptr);
        dtable.vkDestroyFence(impl.device, impl.destroy_fence, nullptr);

        wis::detail::VKReleaseSwapchain(impl.swapchain, impl.swapchain_header);
        impl.swapchain = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKSwapchainPresent(const WisVKSwapchain* self,
                                                        WisPresentFlags       flags,
                                                        const WisRect*        rects,
                                                        size_t                rect_count)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKSwapchainImpl>(self);

    auto image_ready_semaphores     = impl.swapchain_header->header.GetImageAvailableSemaphores();
    auto render_finished_semaphores = impl.swapchain_header->header.GetRenderFinishedSemaphores();

    // Signal the present semaphore for the current back buffer to ensure that presentation waits for rendering to finish
    VkSemaphoreSubmitInfo present_submit_info{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext     = nullptr,
        .semaphore = render_finished_semaphores[impl.present_index],
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 desc{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext                    = nullptr,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &present_submit_info,
    };
    impl.swapchain_table->vkQueueSubmit2(impl.present_queue, 1, &desc, nullptr);

    VkPresentInfoKHR present_info{
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext              = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &render_finished_semaphores[impl.present_index],
        .swapchainCount     = 1,
        .pSwapchains        = &impl.swapchain,
        .pImageIndices      = &impl.present_index,
        .pResults           = nullptr,
    };

    auto vr = impl.swapchain_table->vkQueuePresentKHR(impl.present_queue, &present_info);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "vkQueuePresentKHR failed">(vr);
    }

    impl.lazy_acquire = flags & WisPresentFlagsTimeoutOnBlock;

    auto result = wis::detail::VKAcquireNextImage(impl);
    if (result == VK_TIMEOUT) {
        return wis::detail::make_result<wis::detail::Func(), "vkAcquireNextImageKHR timed out">(VK_TIMEOUT);
    }

    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "vkAcquireNextImageKHR failed">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKSwapchainGetCurrentIndex(const WisVKSwapchain* self,
                                                                uint32_t*             index)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKSwapchainImpl>(self);
    if (impl.lazy_acquire) {
        impl.lazy_acquire = false;
        auto vr           = wis::detail::VKAcquireNextImage(impl);
        if (!wis::detail::succeeded(vr)) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to acquire next image from swapchain">(vr);
        }
    }

    *index = impl.present_index;
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKSwapchainUpdate(const WisVKSwapchain*         self,
                                                       const WisSwapchainUpdateDesc* desc)
{
    auto& impl        = wis::from_handle_ref<const wis::impl::VKSwapchainImpl>(self);
    auto& header      = impl.swapchain_header->header;
    auto& create_info = header.create_info;

    VkFormat new_format     = wis::detail::VKConvert(desc->format);
    bool     size_changed   = desc->width != 0 && desc->height != 0 && (desc->width != create_info.imageExtent.width || desc->height != create_info.imageExtent.height);
    bool     format_changed = desc->format != WisDataFormatUnknown && new_format != create_info.imageFormat;
    bool     count_changed  = desc->image_count != 0 && desc->image_count != create_info.minImageCount;
    bool     vsync_changed  = desc->vsync != (create_info.presentMode == VK_PRESENT_MODE_FIFO_KHR);

    auto present_mode = VK_PRESENT_MODE_FIFO_KHR;
    if (vsync_changed && !desc->vsync) {
        auto modes = header.GetSupportedPresentModes();
        if (header.tearing) {
            if (std::ranges::find(modes, VK_PRESENT_MODE_IMMEDIATE_KHR) != std::end(modes)) {
                present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            } else if (std::ranges::find(modes, VK_PRESENT_MODE_FIFO_RELAXED_KHR) != std::end(modes)) {
                present_mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
            }
        } else if (std::ranges::find(modes, VK_PRESENT_MODE_MAILBOX_KHR) != std::end(modes) && (create_info.imageArrayLayers == 1)) {
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }
    vsync_changed = vsync_changed && present_mode != create_info.presentMode;
    if (!size_changed && !format_changed && !count_changed && !vsync_changed) {
        return wis::detail::vk_success; // nothing to update
    }

    // Check format support if format is changing
    if (format_changed) {
        auto formats = header.GetSupportedFormats();
        if (std::ranges::find_if(formats, [new_format](const VkSurfaceFormatKHR& fmt) { return fmt.format == new_format; }) == std::end(formats)) {
            return wis::detail::make_result<wis::detail::Func(), "Requested format is not supported for presentation">(VK_ERROR_FORMAT_NOT_SUPPORTED);
        }
    }

    // Get surface capabilities to validate new size if it is changing
    // Query surface props
    VkSurfaceCapabilities2KHR capabilities{
        .sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR,
        .pNext = nullptr,
    };
    if (size_changed || count_changed) {
        VkPhysicalDeviceSurfaceInfo2KHR surface_info{
            .sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
            .pNext   = nullptr,
            .surface = header.surface
        };
        header.vkGetPhysicalDeviceSurfaceCapabilities2KHR(header.physical_device, &surface_info, &capabilities);

        capabilities.surfaceCapabilities.maxImageCount = capabilities.surfaceCapabilities.maxImageCount == 0
                ? wis::AbsoluteMaxSwapchainImages
                : capabilities.surfaceCapabilities.maxImageCount;
    }

    if (count_changed &&
        (desc->image_count < capabilities.surfaceCapabilities.minImageCount ||
         desc->image_count > capabilities.surfaceCapabilities.maxImageCount)) {
        return wis::detail::make_result<wis::detail::Func(), "Requested swapchain image count is out of bounds for the given surface">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Store backups
    auto saved_extent = create_info.imageExtent;
    auto saved_format = create_info.imageFormat;
    auto saved_count  = create_info.minImageCount;
    auto saved_mode   = create_info.presentMode;

    auto restore_on_failure = [&create_info, saved_extent, saved_format, saved_count, saved_mode]() {
        create_info.imageExtent   = saved_extent;
        create_info.imageFormat   = saved_format;
        create_info.minImageCount = saved_count;
        create_info.presentMode   = saved_mode;
        create_info.oldSwapchain  = VK_NULL_HANDLE;
    };

    create_info.imageExtent.width  = desc->width != 0
             ? std::clamp(desc->width, capabilities.surfaceCapabilities.minImageExtent.width, capabilities.surfaceCapabilities.maxImageExtent.width)
             : create_info.imageExtent.width;
    create_info.imageExtent.height = desc->height != 0
            ? std::clamp(desc->height, capabilities.surfaceCapabilities.minImageExtent.height, capabilities.surfaceCapabilities.maxImageExtent.height)
            : create_info.imageExtent.height;

    create_info.imageFormat   = desc->format != WisDataFormatUnknown ? new_format : create_info.imageFormat;
    create_info.minImageCount = desc->image_count != 0 ? desc->image_count : create_info.minImageCount;
    create_info.presentMode   = vsync_changed ? present_mode : create_info.presentMode;
    create_info.oldSwapchain  = impl.swapchain;

    // Recreate swapchain with new parameters
    auto vr = impl.swapchain_table->vkCreateSwapchainKHR(impl.device, &create_info, nullptr, &impl.swapchain);
    if (!wis::detail::succeeded(vr)) {
        restore_on_failure();
        return wis::detail::make_result<wis::detail::Func(), "Failed to create new swapchain during update">(vr);
    }

    // First, let's stall the queue to ensure the GPU is not accessing the swapchain before we destroy it.
    vr = impl.swapchain_table->vkQueueSubmit2(impl.present_queue, 0, nullptr, impl.destroy_fence);
    if (!wis::detail::succeeded(vr)) {
        restore_on_failure();
        return wis::detail::make_result<wis::detail::Func(), "Failed to submit queue for swapchain update">(vr);
    }

    // Wait for the GPU to finish with the swapchain
    vr = impl.swapchain_table->vkWaitForFences(impl.device, 1, &impl.destroy_fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
    if (!wis::detail::succeeded(vr)) {
        restore_on_failure();
        return wis::detail::make_result<wis::detail::Func(), "Failed to wait for fence during swapchain update">(vr);
    }

    // Destroy old swapchain after new one is created to minimize the time we are without a valid swapchain
    impl.swapchain_table->vkDestroySwapchainKHR(impl.device, create_info.oldSwapchain, nullptr);

    impl.acquire_index = 0;
    impl.lazy_acquire  = false;

    vr = wis::detail::VKAcquireNextImage(impl);
    if (vr != VK_SUCCESS) {
        // no restore
        return wis::detail::make_result<wis::detail::Func(), "Failed to acquire next image for the new swapchain during update">(vr);
    }

    return wis::detail::vk_success;
}

#endif // WIS_VK_SWAPCHAIN_CPP