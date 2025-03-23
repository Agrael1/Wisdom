#ifndef WIS_DX12_FENCE_H
#define WIS_DX12_FENCE_H
#ifndef WISDOM_MODULE_DECL
#include <d3d12.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_unique_event.h>
#endif

namespace wis {
class DX12Fence;

WISDOM_EXPORT
template<>
struct Internal<DX12Fence> {
    wis::com_ptr<ID3D12Fence1> fence;
    wis::unique_event fence_event = CreateEventW(nullptr, false, false, nullptr);
};

class ImplDX12Fence : public QueryInternal<DX12Fence>
{
public:
    ImplDX12Fence() noexcept = default;

    operator DX12FenceView() const noexcept
    {
        return fence.get();
    }

    operator bool() const noexcept
    {
        return bool(fence);
    }

public:
    [[nodiscard]] uint64_t
    GetCompletedValue() const noexcept
    {
        return fence->GetCompletedValue();
    }

    [[nodiscard]] WIS_INLINE wis::Result
    Wait(uint64_t value,
         uint64_t wait_ns = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] wis::Result
    Signal(uint64_t value) const noexcept
    {
        HRESULT hr = fence->Signal(value);
        return !succeeded(hr) ? wis::make_result<FUNC, "Failed to signal fence">(hr) : wis::success;
    }
};

#pragma region DX12Fence
/**
 * @brief Represents fence for synchronization of GPU timeline.
 * */
WISDOM_EXPORT
class DX12Fence : public wis::ImplDX12Fence
{
public:
    using wis::ImplDX12Fence::ImplDX12Fence;

public:
    /**
     * @brief Get the current value of the fence.
     * @return Value of the fence.
     * */
    inline uint64_t GetCompletedValue() const noexcept
    {
        return wis::ImplDX12Fence::GetCompletedValue();
    }
    /**
     * @brief Wait on CPU for the fence to reach a certain value.
     * @param value Value to wait for.
     * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
     * */
    [[nodiscard]] inline wis::Result Wait(uint64_t value, uint64_t wait_ns = UINT64_MAX) const noexcept
    {
        return wis::ImplDX12Fence::Wait(value, wait_ns);
    }
    /**
     * @brief Signal the fence from CPU.
     * @param value Value to signal.
     * */
    [[nodiscard]] inline wis::Result Signal(uint64_t value) const noexcept
    {
        return wis::ImplDX12Fence::Signal(value);
    }
};
#pragma endregion DX12Fence

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_fence.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // WIS_DX12_FENCE_H
