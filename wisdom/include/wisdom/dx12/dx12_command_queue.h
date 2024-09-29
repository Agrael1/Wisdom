#ifndef WIS_DX12_COMMAND_QUEUE_H
#define WIS_DX12_COMMAND_QUEUE_H
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_views.h>

namespace wis {
class DX12CommandQueue;

template<>
struct Internal<DX12CommandQueue> {
    wis::com_ptr<ID3D12CommandQueue> queue;
};

/// @brief A command queue is used to submit command lists to the GPU.
class ImplDX12CommandQueue : public QueryInternal<DX12CommandQueue>
{
public:
    ImplDX12CommandQueue() = default;
    explicit ImplDX12CommandQueue(wis::com_ptr<ID3D12CommandQueue> queue) noexcept
        : QueryInternal(std::move(queue)) { }

    operator bool() const noexcept
    {
        return bool(queue);
    }
    operator DX12QueueView() const noexcept
    {
        return queue.get();
    }

public:
    void ExecuteCommandLists(const DX12CommandListView* lists, uint32_t count) const noexcept
    {
        queue->ExecuteCommandLists(count, reinterpret_cast<ID3D12CommandList* const*>(lists));
        check_context();
    }

    wis::Result SignalQueue(DX12FenceView fence, uint64_t value) const noexcept
    {
        HRESULT hr;
        return wis::succeeded(hr = queue->Signal(std::get<0>(fence), value))
               ? wis::success
               : wis::make_result<FUNC, "Signal failed">(hr);
    }
    wis::Result WaitQueue(DX12FenceView fence, uint64_t value) const noexcept
    {
        HRESULT hr;
        return wis::succeeded(hr = queue->Wait(std::get<0>(fence), value))
               ? wis::success
               : wis::make_result<FUNC, "Wait failed">(hr);
    }
};
#pragma region DX12CommandQueue
/**
 * @brief Represents command queue for executing command lists.
 * */
struct DX12CommandQueue : public wis::ImplDX12CommandQueue {
public:
    using wis::ImplDX12CommandQueue::ImplDX12CommandQueue;

public:
    /**
     * @brief Executes the command lists.
     * @param lists The command lists to execute.
     * @param count The number of command lists to execute.
     * */
    inline void ExecuteCommandLists(const wis::DX12CommandListView* lists, uint32_t count) const noexcept
    {
        return wis::ImplDX12CommandQueue::ExecuteCommandLists(lists, count);
    }
    /**
     * @brief Enqueue the signal to the queue, that gets executed after all the work has been done.
     * @param fence The fence to signal.
     * @param value The value to signal the fence with.
     * */
    [[nodiscard]] inline wis::Result SignalQueue(wis::DX12FenceView fence, uint64_t value) const noexcept
    {
        return wis::ImplDX12CommandQueue::SignalQueue(std::move(fence), value);
    }
    /**
     * @brief Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
     * Can still be enqueued after the signal.
     * @param fence The fence to wait on.
     * @param value The value to wait the fence to reach.
     * */
    [[nodiscard]] inline wis::Result WaitQueue(wis::DX12FenceView fence, uint64_t value) const noexcept
    {
        return wis::ImplDX12CommandQueue::WaitQueue(std::move(fence), value);
    }
};
#pragma endregion DX12CommandQueue
} // namespace wis

#endif // WIS_DX12_COMMAND_QUEUE_H