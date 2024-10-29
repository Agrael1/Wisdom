#ifndef WIS_VK_COMMAND_QUEUE_CPP
#define WIS_VK_COMMAND_QUEUE_CPP
#include <wisdom/vulkan/vk_command_queue.h>

void wis::ImplVKCommandQueue::ExecuteCommandLists(const VKCommandListView* lists,
                                                  uint32_t count) const noexcept
{
    VkSubmitInfo submit_info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .commandBufferCount = count,
        .pCommandBuffers = reinterpret_cast<const VkCommandBuffer*>(lists),
    };

    std::ignore = device.table().vkQueueSubmit(queue, 1, &submit_info, nullptr);
}

wis::Result wis::ImplVKCommandQueue::SignalQueue(VKFenceView fence, uint64_t value) const noexcept
{
    VkSemaphore sem = std::get<0>(fence);
    VkSemaphoreSubmitInfo sem_submit{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = sem,
        .value = value,
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos = &sem_submit
    };
    VkResult result = device.table().vkQueueSubmit2(queue, 1, &info, nullptr);
    return succeeded(result) ? wis::success
                             : wis::make_result<FUNC, "vkQueueSubmit failed to signal fence">(result);
}

wis::Result wis::ImplVKCommandQueue::WaitQueue(VKFenceView fence, uint64_t value) const noexcept
{
    VkSemaphore sem = std::get<0>(fence);
    VkSemaphoreSubmitInfo sem_submit{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = sem,
        .value = value,
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &sem_submit
    };
    VkResult result = device.table().vkQueueSubmit2(queue, 1, &info, nullptr);
    return succeeded(result) ? wis::success
                             : wis::make_result<FUNC, "vkQueueSubmit failed to signal fence">(result);
}

#endif // !VK_COMMAND_QUEUE_CPP
