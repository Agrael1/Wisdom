#pragma once
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
class DX12CommandQueue : public QueryInternal<DX12CommandQueue>
{
public:
    DX12CommandQueue() = default;
    explicit DX12CommandQueue(wis::com_ptr<ID3D12CommandQueue> queue) noexcept
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
} // namespace wis
