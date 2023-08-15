#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>
#endif

namespace wis {
class VKCommandQueue;

template<>
struct Internal<VKCommandQueue> {
    vk::Queue queue;
};

/// @brief A command queue is used to submit command lists to the GPU.
WIS_EXPORT class VKCommandQueue : public QueryInternal<VKCommandQueue>
{
public:
    VKCommandQueue() = default;
    explicit VKCommandQueue(vk::Queue queue) noexcept
        : QueryInternal(queue) { }
    operator VKCommandQueueView() const noexcept
    {
        return queue;
    }
    operator bool() const noexcept
    {
        return bool(queue);
    }

    /// @brief Execute a command list on the GPU.
    /// @param list List to execute.
    void ExecuteCommandList(VKCommandListView command_list) const noexcept
    {
        vk::PipelineStageFlags wait_dst_stage_mask = vk::PipelineStageFlagBits::eAllCommands;

        vk::SubmitInfo submit_info{
            .pWaitDstStageMask = &wait_dst_stage_mask,
            .commandBufferCount = 1,
            .pCommandBuffers = &command_list,
        };
        queue.submit(submit_info);
    }

    /// @brief Signal a fence with some value.
    /// @param fence Fence to signal.
    /// @param value Value to signal with.
    /// @return true if call succeeded.
    bool Signal(VKFenceView fence, uint64_t value) const noexcept
    {
        vk::TimelineSemaphoreSubmitInfo submit{
            .signalSemaphoreValueCount = 1,
            .pSignalSemaphoreValues = &value
        };

        vk::SubmitInfo info{
            .pNext = &submit,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &fence,
        };
        return wis::succeeded(queue.submit(1, &info, nullptr));
    }
};
} // namespace wis
