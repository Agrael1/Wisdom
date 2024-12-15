#ifndef WIS_DX12_FENCE_CPP
#define WIS_DX12_FENCE_CPP
#include <wisdom/dx12/dx12_fence.h>

wis::Result wis::ImplDX12Fence::Wait(uint64_t value, uint64_t wait_ns) const noexcept
{
    if (GetCompletedValue() >= value)
        return wis::success;

    HRESULT hr = fence->SetEventOnCompletion(value, fence_event.get());
    if (!succeeded(hr))
        return wis::make_result<FUNC, "Failed to set event">(hr);

    auto st = fence_event.wait(wait_ns == std::numeric_limits<uint64_t>::max()
                                       ? std::numeric_limits<uint32_t>::max()
                                       : uint32_t(wait_ns / 1000));
    return st == wis::Status::Timeout  ? wis::Result{ st, "Wait timed out" }
            : st != wis::Status::Error ? wis::success
                                       : wis::make_result<FUNC, "Failed to wait for event">(E_FAIL);
}
#endif // DX12_FENCE_CPP
