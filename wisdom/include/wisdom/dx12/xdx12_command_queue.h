#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/api/consts.h>
#include <wisdom/dx12/xdx12_checks.h>
#include <wisdom/dx12/xdx12_views.h>

namespace wis {
class DX12CommandQueue;

template<>
struct Internal<DX12CommandQueue> {
public:
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

public:
    /// @brief Execute a command list on the GPU.
    /// @param list List to execute.
    // void ExecuteCommandList(DX12CommandListView list) const noexcept
    //{
    //     queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&list));
    //     log_context();
    // }

    /// @brief Execute a command list on the GPU.
    /// @param list List to execute.
    // void DXExecuteCommandLists(std::span<DX12CommandListView> lists) const noexcept
    //{
    //     queue->ExecuteCommandLists(lists.size(), reinterpret_cast<ID3D12CommandList* const*>(lists.data()));
    //     log_context();
    // }

    wis::Result Signal(DX12FenceView fence, uint64_t value) const noexcept
    {
        HRESULT hr;
        return wis::succeeded(hr = queue->Signal(std::get<0>(fence), value))
                ? wis::success
                : wis::make_result<FUNC, "Signal failed">(hr);
    }
};
} // namespace wis
