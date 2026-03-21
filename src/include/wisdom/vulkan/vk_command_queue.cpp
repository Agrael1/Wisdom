#ifndef WIS_VK_COMMAND_QUEUE_CPP
#define WIS_VK_COMMAND_QUEUE_CPP
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <bit>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandQueue(WisVKCommandQueue* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandQueueImpl>(self);
    if (impl.queue) {
        wis::detail::VKReleaseDevice(impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
        impl.queue         = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandQueueSubmit(const WisVKCommandQueue*    self,
                                                          const WisVKCommandListView* lists,
                                                          size_t                      count)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandQueueImpl>(self);

    // I am not sorry, reinterpret_cast is the only way to convert from WisVKCommandListView
    // (which is a pointer to an opaque handle) to VkCommandBuffer* without violating strict aliasing rules.
    VkSubmitInfo submit_info{
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext              = nullptr,
        .commandBufferCount = static_cast<uint32_t>(count),
        .pCommandBuffers    = reinterpret_cast<const VkCommandBuffer*>(lists),
    };

    auto vr = impl.device_header->header.command_queue_table.vkQueueSubmit(impl.queue, 1, &submit_info, nullptr);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "vkQueueSubmit failed to submit command list(s)">(vr);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandQueueSignalFence(const WisVKCommandQueue* self,
                                                               WisVKFenceView           fence,
                                                               uint64_t                 value)
{
    auto&                 impl  = wis::from_handle_ref<const wis::impl::VKCommandQueueImpl>(self);
    VkQueue               queue = impl.queue;
    VkSemaphore           sem   = std::bit_cast<VkSemaphore>(fence);
    VkSemaphoreSubmitInfo sem_submit{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = sem,
        .value     = value,
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 info{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &sem_submit
    };
    VkResult result = impl.device_header->header.command_queue_table.vkQueueSubmit2(queue, 1, &info, nullptr);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "vkQueueSubmit failed to signal fence">(result);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandQueueWaitFence(const WisVKCommandQueue* self,
                                                             WisVKFenceView           fence,
                                                             uint64_t                 value)
{
    auto&                 impl  = wis::from_handle_ref<const wis::impl::VKCommandQueueImpl>(self);
    VkQueue               queue = impl.queue;
    VkSemaphore           sem   = std::bit_cast<VkSemaphore>(fence);
    VkSemaphoreSubmitInfo sem_submit{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = sem,
        .value     = value,
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 info{
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos    = &sem_submit
    };
    VkResult result = impl.device_header->header.command_queue_table.vkQueueSubmit2(queue, 1, &info, nullptr);
    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "vkQueueSubmit failed to signal fence">(result);
    }
    return wis::detail::vk_success;
}

#endif // WIS_VK_FENCE_CPP
