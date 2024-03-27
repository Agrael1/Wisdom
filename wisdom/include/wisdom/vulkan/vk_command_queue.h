#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>

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
    operator VKQueueView() const noexcept
    {
        return queue;
    }

public:
    WIS_INLINE void ExecuteCommandLists(const VKCommandListView* lists, uint32_t count) const noexcept;
    WIS_INLINE wis::Result SignalQueue(VKFenceView fence, uint64_t value) const noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_command_queue.cpp"
#endif // !WISDOM_HEADER_ONLY