#ifndef WIS_VK_SWAPCHAIN_CPP
#define WIS_VK_SWAPCHAIN_CPP

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <cassert>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroySwapchain(WisVKSwapchain* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKSwapchainImpl>(self);

    if (impl.swapchain != VK_NULL_HANDLE) {
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
    auto result = impl.swapchain_table->vkAcquireNextImageKHR(impl.device,
                                                              impl.swapchain,
                                                              flags & WisPresentFlagsTimeoutOnBlock
                                                                      ? 0
                                                                      : std::numeric_limits<uint64_t>::max(),
                                                              image_ready_semaphores[impl.acquire_index],
                                                              nullptr,
                                                              &impl.present_index);
    if (result == VK_TIMEOUT) {
        return wis::detail::make_result<wis::detail::Func(), "vkAcquireNextImageKHR timed out">(VK_TIMEOUT);
    }

    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "vkAcquireNextImageKHR failed">(result);
    }

    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WISDOM_API uint32_t wisVKSwapchainGetCurrentIndex(const WisVKSwapchain* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKSwapchainImpl>(self);
    if (impl.lazy_acquire) {
        auto image_ready_semaphores = impl.swapchain_header->header.GetImageAvailableSemaphores();
        auto result                = impl.swapchain_table->vkAcquireNextImageKHR(impl.device,
                                                                              impl.swapchain,
                                                                              std::numeric_limits<uint64_t>::max(),
                                                                              image_ready_semaphores[impl.acquire_index],
                                                                              nullptr,
                                                                              &impl.present_index);
        // That should never fail since we are using an infinite timeout.
        assert(wis::detail::succeeded(result) && "vkAcquireNextImageKHR failed during lazy acquire in GetCurrentIndex");
        impl.lazy_acquire = false;
    }

    return impl.present_index;
}

#endif // WIS_VK_SWAPCHAIN_CPP