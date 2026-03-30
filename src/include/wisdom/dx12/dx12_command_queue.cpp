#ifndef WIS_DX12_COMMAND_QUEUE_CPP
#define WIS_DX12_COMMAND_QUEUE_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/util/allocation.hpp>

#include <bit>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandQueue(WisDX12CommandQueue* self)
{
    auto& [queue] = wis::from_handle_ref<wis::impl::DX12CommandQueueImpl>(self);
    if (!queue) {
        return;
    }
    queue->Release();
    queue = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisDX12CommandQueueSubmit(const WisDX12CommandQueue* self, const WisDX12CommandListView* lists, size_t count)
{
    auto& [queue] = wis::from_handle_ref<const wis::impl::DX12CommandQueueImpl>(self);
    queue->ExecuteCommandLists(static_cast<UINT>(count), reinterpret_cast<ID3D12CommandList* const*>(lists));
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisDX12CommandQueueSignalFence(const WisDX12CommandQueue* self, WisDX12FenceView fence, uint64_t value)
{
    auto& [queue] = wis::from_handle_ref<const wis::impl::DX12CommandQueueImpl>(self);
    auto hr = queue->Signal(std::bit_cast<ID3D12Fence*>(fence), value);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to signal fence on command queue">(hr);
    }

    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisDX12CommandQueueWaitFence(const WisDX12CommandQueue* self, WisDX12FenceView fence, uint64_t value)
{
    auto& [queue] = wis::from_handle_ref<const wis::impl::DX12CommandQueueImpl>(self);
    auto hr = queue->Wait(std::bit_cast<ID3D12Fence*>(fence), value);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to enqueue wait on fence for command queue">(hr);
    }
    return wis::detail::dx_success;
}

#endif // WIS_DX12_COMMAND_QUEUE_CPP