#ifndef VK_COMMAND_QUEUE_H
#define VK_COMMAND_QUEUE_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>

namespace wis {
class VKCommandQueue;

template<>
struct Internal<VKCommandQueue> {
    wis::SharedDevice device;
    h::VkQueue queue;

public:
    Internal() noexcept = default;
    ~Internal() noexcept = default;
    Internal(wis::SharedDevice device, VkQueue queue) noexcept
        : device(std::move(device)), queue(queue) { }

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
class VKCommandQueue : public QueryInternal<VKCommandQueue>
{
public:
    VKCommandQueue() = default;
    explicit VKCommandQueue(wis::SharedDevice device, VkQueue queue)
        : QueryInternal(std::move(device), queue) { }

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
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_command_queue.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !
