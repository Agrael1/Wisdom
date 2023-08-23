#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>
#endif

namespace wis {
class DX12CommandQueue;

template<>
class Internal<DX12CommandQueue>
{
public:
    winrt::com_ptr<ID3D12CommandQueue> queue;
};

/// @brief A command queue is used to submit command lists to the GPU.
WIS_EXPORT class DX12CommandQueue : public QueryInternal<DX12CommandQueue>
{
public:
    DX12CommandQueue() = default;
    explicit DX12CommandQueue(winrt::com_ptr<ID3D12CommandQueue> queue) noexcept
        : QueryInternal(std::move(queue)) { }
    operator DX12CommandQueueView() const noexcept
    {
        return queue.get();
    }
    operator bool() const noexcept
    {
        return queue != nullptr;
    }

public:
    /// @brief Execute a command list on the GPU.
    /// @param list List to execute.
    void ExecuteCommandList(DX12CommandListView list) const noexcept
    {
        queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&list));
        log_context();
    }
    /// @brief Execute a command list on the GPU.
    /// @param list List to execute.
    void DXExecuteCommandLists(std::span<DX12CommandListView> lists) const noexcept
    {
        queue->ExecuteCommandLists(lists.size(), reinterpret_cast<ID3D12CommandList* const*>(lists.data()));
        log_context();
    }

    /// @brief Signal a fence with some value.
    /// @param fence Fence to signal.
    /// @param value Value to signal with.
    /// @return true if call succeeded.
    bool Signal(DX12FenceView fence, uint64_t value) const noexcept
    {
        return wis::succeeded(queue->Signal(fence, value));
    }
};
} // namespace wis
