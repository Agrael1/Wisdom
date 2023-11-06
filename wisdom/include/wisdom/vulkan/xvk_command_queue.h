#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_views.h>
#include <wisdom/vulkan/xvk_checks.h>
#include <wisdom/api/consts.h>

namespace wis {
class VKCommandQueue;

template<>
class Internal<VKCommandQueue>
{
public:
    wis::SharedDevice device;
    VkQueue queue;
};

/// @brief A command queue is used to submit command lists to the GPU.
class VKCommandQueue : public QueryInternal<VKCommandQueue>
{
public:
    VKCommandQueue() = default;
    explicit VKCommandQueue(wis::SharedDevice device, VkQueue queue)
        : QueryInternal(std::move(device), queue) { }
    VKCommandQueue(VKCommandQueue&& o) noexcept
        : QueryInternal(std::move(o.device), std::exchange(o.queue, nullptr)) { }
    VKCommandQueue& operator=(VKCommandQueue&& o) noexcept
    {
        std::swap(device, o.device);
        std::swap(queue, o.queue);
        return *this;
    }
    operator bool() const noexcept
    {
        return bool(queue);
    }

public:
    void ExecuteCommandLists(const VKCommandListView* lists, uint32_t count) const noexcept
    {
        VkSubmitInfo submit_info{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBufferCount = count,
            .pCommandBuffers = reinterpret_cast<const VkCommandBuffer*>(lists),
        };

        std::ignore = device.table()->vkQueueSubmit(queue, 1, &submit_info, nullptr);
    }

    wis::Result SignalQueue(VKFenceView fence, uint64_t value) const noexcept
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
        VkResult result = device.table()->vkQueueSubmit(queue, 1, &info, nullptr);
        return succeeded(result)
                ? wis::success
                : wis::make_result<FUNC, "vkQueueSubmit failed to signal fence">(result);
    }
};
} // namespace wis
