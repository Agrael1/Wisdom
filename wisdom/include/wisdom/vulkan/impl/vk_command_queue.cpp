#ifndef WIS_VK_COMMAND_QUEUE_CPP
#define WIS_VK_COMMAND_QUEUE_CPP
#include <wisdom/vulkan/vk_command_queue.h>

void wis::VKCommandQueue::ExecuteCommandLists(const VKCommandListView* lists,
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

wis::Result wis::VKCommandQueue::SignalQueue(VKFenceView fence, uint64_t value) const noexcept
{
    VkSemaphore sem = std::get<0>(fence);
    VkTimelineSemaphoreSubmitInfoKHR submit{
        .sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreValueCount = 0,
        .pWaitSemaphoreValues = nullptr,
        .signalSemaphoreValueCount = 1,
        .pSignalSemaphoreValues = &value
    };

    VkSubmitInfo info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = &submit,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 0,
        .pCommandBuffers = nullptr,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &sem
    };
    VkResult result = device.table().vkQueueSubmit(queue, 1, &info, nullptr);
    return succeeded(result) ? wis::success
           : wis::make_result<FUNC, "vkQueueSubmit failed to signal fence">(result);
}

wis::Result wis::VKCommandQueue::WaitQueue(VKFenceView fence, uint64_t value) const noexcept
{
    VkSemaphore sem = std::get<0>(fence);
    VkTimelineSemaphoreSubmitInfoKHR submit{
        .sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreValueCount = 1,
        .pWaitSemaphoreValues = &value,
        .signalSemaphoreValueCount = 0,
        .pSignalSemaphoreValues = nullptr
    };

    VkSubmitInfo info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = &submit,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 0,
        .pCommandBuffers = nullptr,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &sem
    };
    VkResult result = device.table().vkQueueSubmit(queue, 1, &info, nullptr);
    return succeeded(result) ? wis::success
           : wis::make_result<FUNC, "vkQueueSubmit failed to signal fence">(result);
}

#endif // !VK_COMMAND_QUEUE_CPP
