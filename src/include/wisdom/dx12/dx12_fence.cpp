#ifndef WIS_DX12_FENCE_CPP
#define WIS_DX12_FENCE_CPP

#include <wisdom/generated/backend_api.h>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/util/allocation.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyFence(WisDX12Fence* self)
{
    auto& [fence, event] = wis::from_handle_ref<wis::impl::DX12FenceImpl>(self);
    if (!fence) {
        return;
    }

    fence->Release();
    CloseHandle(event);
    fence = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12FenceGetCompletedValue(const WisDX12Fence* self)
{
    auto& [fence, event] = wis::from_handle_ref<const wis::impl::DX12FenceImpl>(self);
    return fence->GetCompletedValue();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12FenceWait(const WisDX12Fence* self,
                                      uint64_t            value,
                                      uint64_t            wait_ns)
{
    auto& [fence, event] = wis::from_handle_ref<const wis::impl::DX12FenceImpl>(self);
    HRESULT hr           = fence->SetEventOnCompletion(value, event);
    if (FAILED(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to set event on fence">(hr);
    }
    DWORD wait_result = WaitForSingleObject(event, wait_ns == UINT64_MAX ? INFINITE : static_cast<DWORD>(wait_ns / 1'000'000));
    if (wait_result == WAIT_OBJECT_0) {
        return wis::detail::dx_success;
    } else if (wait_result == WAIT_TIMEOUT) {
        return { WisStatus::WisStatusTimeout, 0, "Wait timed out." };
    } else {
        return wis::detail::make_result<wis::detail::Func(), "Failed to wait for fence event">(E_FAIL);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12FenceSignal(const WisDX12Fence* self,
                                        uint64_t            value)
{
    auto& [fence, event] = wis::from_handle_ref<const wis::impl::DX12FenceImpl>(self);
    HRESULT hr           = fence->Signal(value);
    if (FAILED(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to signal fence">(hr);
    }
    return wis::detail::dx_success;
}

#endif // WIS_DX12_FENCE_CPP