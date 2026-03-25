#ifndef WIS_DX12_SWAPCHAIN_CPP
#define WIS_DX12_SWAPCHAIN_CPP

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroySwapchain(WisDX12Swapchain* self)
{
    auto& swapchain = wis::from_handle_ref<wis::impl::DX12SwapchainImpl>(self);
    if (swapchain.swapchain) {
        swapchain.swapchain->Release();
        swapchain.swapchain = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12SwapchainPresent(const WisDX12Swapchain* self,
                                                          WisPresentFlags         flags,
                                                          const WisRect*          rects,
                                                          size_t                  rect_count)
{
    auto& swapchain = wis::from_handle_ref<const wis::impl::DX12SwapchainImpl>(self);
    UINT  dx_flags  = swapchain.vsync ? 0 : swapchain.flags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    dx_flags |= wis::detail::DX12Convert(flags);

    HRESULT hr = S_OK;
    if (!rects || rect_count == 0 || rect_count > wis::MaxCopyRegions) {
        hr = swapchain.swapchain->Present(swapchain.vsync, dx_flags);
    } else {
        RECT dx_rects[wis::MaxCopyRegions];
        for (size_t i = 0; i < rect_count; i++) {
            dx_rects[i] = RECT{
                .left   = static_cast<LONG>(rects[i].x),
                .top    = static_cast<LONG>(rects[i].y),
                .right  = static_cast<LONG>(rects[i].x + rects[i].width),
                .bottom = static_cast<LONG>(rects[i].y + rects[i].height),
            };
        }

        DXGI_PRESENT_PARAMETERS present_params{
            .DirtyRectsCount = static_cast<UINT>(rect_count),
            .pDirtyRects     = dx_rects,
        };
        hr = swapchain.swapchain->Present1(swapchain.vsync, dx_flags, &present_params);
    }

    if (hr == DXGI_ERROR_WAS_STILL_DRAWING) {
        return wis::detail::make_result<wis::detail::Func(), "Previous frame is still being presented, cannot present again yet">(WisStatusTimeout, hr);
    }

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to present swap chain">(hr);
    }
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WISDOM_API uint32_t wisDX12SwapchainGetCurrentIndex(const WisDX12Swapchain* self)
{
    return wis::from_handle_ref<const wis::impl::DX12SwapchainImpl>(self).swapchain->GetCurrentBackBufferIndex();
}

#endif // WIS_DX12_SWAPCHAIN_CPP