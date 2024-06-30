#pragma once
#include <d3d12.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_unique_event.h>

namespace wis {
class DX12Fence;

template<>
struct Internal<DX12Fence>
{
    wis::com_ptr<ID3D12Fence1> fence;
    wis::unique_event fence_event;
};

/// @brief A fence is a synchronization primitive that allows the CPU to wait for the GPU to finish
/// rendering a frame.
class DX12Fence : public QueryInternal<DX12Fence>
{
public:
    DX12Fence() = default;
    explicit DX12Fence(wis::com_ptr<ID3D12Fence1> xfence) noexcept
        : QueryInternal(std::move(xfence), CreateEventW(nullptr, false, false, nullptr)) { }
    DX12Fence(DX12Fence&& o) noexcept = default;
    DX12Fence& operator=(DX12Fence&& o) noexcept = default;

    operator DX12FenceView() const noexcept
    {
        return fence.get();
    }

    operator bool() const noexcept
    {
        return bool(fence);
    }

public:
    /// @brief Get the current value of the fence.
    /// @return Value of the fence.
    [[nodiscard]] uint64_t
    GetCompletedValue() const noexcept
    {
        return fence->GetCompletedValue();
    }

    /// @brief Wait for the fence to reach a certain value.
    /// @param value Value to wait for.
    /// @return Boolean indicating whether the fence reached the value.
    [[nodiscard]] WIS_INLINE wis::Result
    Wait(uint64_t value,
         uint64_t wait_ns = std::numeric_limits<uint64_t>::max()) const noexcept;

    /// @brief Signal the fence from CPU.
    /// @param value Value to signal.
    [[nodiscard]] WIS_INLINE wis::Result Signal(uint64_t value) const noexcept
    {
        HRESULT hr = fence->Signal(value);
        return !succeeded(hr) ? wis::make_result<FUNC, "Failed to signal fence">(hr) : wis::success;
    }
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_fence.cpp"
#endif // !WISDOM_HEADER_ONLY
