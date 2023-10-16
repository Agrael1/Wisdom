#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/dx12/xdx12_checks.h>
#include <wisdom/dx12/xdx12_views.h>
#include <wisdom/util/com_ptr.h>
#include <d3d12.h>

namespace wis {
class DX12Fence;

template<>
class Internal<DX12Fence>
{
    struct unique_event {
        unique_event()
            : hevent(CreateEvent(nullptr, false, false, nullptr)) { }
        unique_event(unique_event const&) = delete;
        unique_event& operator=(unique_event const&) = delete;
        unique_event(unique_event&&) = default;
        unique_event& operator=(unique_event&&) = default;
        ~unique_event()
        {
            CloseHandle(hevent);
        }
        auto get() const noexcept
        {
            return hevent;
        }
        operator bool() const noexcept
        {
            return bool(hevent);
        }
        bool wait(uint32_t wait_ms) const noexcept
        {
            return WaitForSingleObject(hevent, wait_ms) == WAIT_OBJECT_0;
        }

    public:
        HANDLE hevent;
    };

public:
    wis::com_ptr<ID3D12Fence1> fence;
    unique_event fence_event;
};

/// @brief A fence is a synchronization primitive that allows the CPU to wait for the GPU to finish rendering a frame.
class DX12Fence : public QueryInternal<DX12Fence>
{
public:
    DX12Fence() = default;
    explicit DX12Fence(wis::com_ptr<ID3D12Fence1> xfence) noexcept
        : QueryInternal(std::move(xfence))
    {
    }
    DX12Fence(DX12Fence&& o) noexcept = default;
    DX12Fence& operator=(DX12Fence&& o) noexcept = default;

    operator bool() const noexcept
    {
        return bool(fence);
    }

public:
    /// @brief Get the current value of the fence.
    /// @return Value of the fence.
    [[nodiscrd]] uint64_t
    GetCompletedValue() const noexcept
    {
        return fence->GetCompletedValue();
    }

    /// @brief Wait for the fence to reach a certain value.
    /// @param value Value to wait for.
    /// @return Boolean indicating whether the fence reached the value.
    wis::Result Wait(uint64_t value, uint64_t wait_ns = std::numeric_limits<uint64_t>::max()) const noexcept
    {
        if (GetCompletedValue() >= value)
            return wis::success;

        HRESULT hr = fence->SetEventOnCompletion(value, fence_event.get());
        if (!succeeded(hr))
            return wis::make_result<FUNC, "Failed to set event">(hr);

        return fence_event.wait(uint32_t(wait_ns / 1000)) ? wis::success : wis::make_result<FUNC, "Failed to wait for event">(E_FAIL);
    }

    /// @brief Signal the fence from CPU.
    /// @param value Value to signal.
    wis::Result Signal(uint64_t value) const noexcept
    {
        HRESULT hr = fence->Signal(value);
        return !succeeded(hr)
                ? wis::make_result<FUNC, "Failed to signal fence">(hr)
                : wis::success;
    }
};
} // namespace wis
