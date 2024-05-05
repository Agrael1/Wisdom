#pragma once
#ifdef WISDOM_BUILD_BINARIES
#include <wisdom/dx12/dx12_fence.h>
#endif // !WISDOM_HEADER_ONLY

/// @brief Wait for the fence to reach a certain value.
/// @param value Value to wait for.
/// @return Boolean indicating whether the fence reached the value.

wis::Result wis::DX12Fence::Wait(uint64_t value, uint64_t wait_ns) const noexcept
{
    if (GetCompletedValue() >= value)
        return wis::success;

    HRESULT hr = fence->SetEventOnCompletion(value, fence_event.get());
    if (!succeeded(hr))
        return wis::make_result<FUNC, "Failed to set event">(hr);

    auto st = fence_event.wait(uint32_t(wait_ns / 1000));
    return st == wis::Status::Timeout  ? wis::Result{ st, "Wait timed out" }
           :
           st != wis::Status::Error ? wis::success
           : wis::make_result<FUNC, "Failed to wait for event">(E_FAIL);
}
