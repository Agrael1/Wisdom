#ifndef WIS_VK_COMMAND_QUEUE_H
#define WIS_VK_COMMAND_QUEUE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
class VKCommandQueue;

WISDOM_EXPORT
template<>
struct Internal<VKCommandQueue> {
    wis::SharedDevice device;
    h::VkQueue queue;

public:
    Internal() noexcept = default;
    ~Internal() noexcept = default;
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }

        device = std::move(o.device);
        queue = std::move(o.queue);
        return *this;
    }
};

/// @brief A command queue is used to submit command lists to the GPU.
class ImplVKCommandQueue : public QueryInternal<VKCommandQueue>
{
public:
    ImplVKCommandQueue() = default;
    operator bool() const noexcept
    {
        return bool(queue);
    }
    operator VKQueueView() const noexcept
    {
        return queue;
    }

public:
    WIS_INLINE void ExecuteCommandLists(const VKCommandListView* lists, uint32_t count) const noexcept;
    WIS_INLINE wis::Result SignalQueue(VKFenceView fence, uint64_t value) const noexcept;
    WIS_INLINE wis::Result WaitQueue(VKFenceView fence, uint64_t value) const noexcept;
};
#pragma region VKCommandQueue
/**
 * @brief Represents command queue for executing command lists.
 * */
WISDOM_EXPORT
class VKCommandQueue : public wis::ImplVKCommandQueue
{
public:
    using wis::ImplVKCommandQueue::ImplVKCommandQueue;

public:
    /**
     * @brief Executes the command lists.
     * @param lists The command lists to execute.
     * @param count The number of command lists to execute.
     * */
    inline void ExecuteCommandLists(const wis::VKCommandListView* lists, uint32_t count) const noexcept
    {
        wis::ImplVKCommandQueue::ExecuteCommandLists(lists, count);
    }
    /**
     * @brief Enqueue the signal to the queue, that gets executed after all the work has been done.
     * @param fence The fence to signal.
     * @param value The value to signal the fence with.
     * */
    [[nodiscard]] inline wis::Result SignalQueue(wis::VKFenceView fence, uint64_t value) const noexcept
    {
        return wis::ImplVKCommandQueue::SignalQueue(std::move(fence), value);
    }
    /**
     * @brief Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
     * Can still be enqueued after the signal.
     * @param fence The fence to wait on.
     * @param value The value to wait the fence to reach.
     * */
    [[nodiscard]] inline wis::Result WaitQueue(wis::VKFenceView fence, uint64_t value) const noexcept
    {
        return wis::ImplVKCommandQueue::WaitQueue(std::move(fence), value);
    }
};
#pragma endregion VKCommandQueue

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_command_queue.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !
