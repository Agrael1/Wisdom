#ifndef WIS_DX12_SWAPCHAIN_CPP
#define WIS_DX12_SWAPCHAIN_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/dx12_convert.hpp>

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroySwapchain(WisDX12Swapchain* self)
{
    auto& swapchain = wis::from_handle_ref<wis::impl::DX12SwapchainImpl>(self);
    if (swapchain.swapchain) {
        swapchain.swapchain->Release();
        swapchain.swapchain = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12SwapchainPresent(
    const WisDX12Swapchain* self,
    WisPresentFlags flags,
    const WisRect* rects,
    size_t rect_count
)
{
    auto& swapchain = wis::from_handle_ref<const wis::impl::DX12SwapchainImpl>(self);
    UINT dx_flags = swapchain.vsync ? 0 : swapchain.flags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    dx_flags |= wis::detail::DX12Convert(flags);

    HRESULT hr = S_OK;
    if (!rects || rect_count == 0 || rect_count > wis::MaxCopyRegions) {
        hr = swapchain.swapchain->Present(swapchain.vsync, dx_flags);
    } else {
        RECT dx_rects[wis::MaxCopyRegions];
        for (size_t i = 0; i < rect_count; i++) {
            dx_rects[i] = RECT{
                .left = static_cast<LONG>(rects[i].x),
                .top = static_cast<LONG>(rects[i].y),
                .right = static_cast<LONG>(rects[i].x + rects[i].width),
                .bottom = static_cast<LONG>(rects[i].y + rects[i].height),
            };
        }

        DXGI_PRESENT_PARAMETERS present_params{
            .DirtyRectsCount = static_cast<UINT>(rect_count),
            .pDirtyRects = dx_rects,
        };
        hr = swapchain.swapchain->Present1(swapchain.vsync, dx_flags, &present_params);
    }

    if (hr == DXGI_ERROR_WAS_STILL_DRAWING) {
        return wis::detail::make_result<
            wis::detail::Func(),
            "Previous frame is still being presented, cannot present again yet">(WisStatusTimeout, hr);
    }

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to present swap chain">(hr);
    }
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12SwapchainGetCurrentIndex(const WisDX12Swapchain* self, uint32_t* index)
{
    *index = wis::from_handle_ref<const wis::impl::DX12SwapchainImpl>(self).swapchain->GetCurrentBackBufferIndex();
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12SwapchainUpdate(
    const WisDX12Swapchain* self,
    const WisSwapchainUpdateDesc* desc
)
{
    auto& swapchain = wis::from_handle_ref<const wis::impl::DX12SwapchainImpl>(self);

    // early out conditions
    uint32_t width = desc->width == 0 ? swapchain.width : desc->width;
    uint32_t height = desc->height == 0 ? swapchain.height : desc->height;
    uint32_t image_count = desc->image_count == 0 ? swapchain.backbuffer_count : desc->image_count;

    bool size_changed = width != swapchain.width || height != swapchain.height;
    bool count_changed = image_count != swapchain.backbuffer_count;
    bool format_changed = desc->format != 0 && desc->format != swapchain.data_format;

    if (!size_changed && !count_changed && !format_changed) {
        // Easy route
        swapchain.vsync = desc->vsync;
        return wis::detail::dx_success; // nothing to update
    }

    auto hr = swapchain.swapchain
                  ->ResizeBuffers(image_count, width, height, wis::detail::DX12Convert(desc->format), swapchain.flags);

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to resize swap chain buffers">(hr);
    }
    swapchain.width = width;
    swapchain.height = height;
    swapchain.data_format = format_changed ? desc->format : swapchain.data_format;
    swapchain.backbuffer_count = image_count;
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12SwapchainGetTextures(
    const WisDX12Swapchain* self,
    WisDX12Texture* buffers,
    size_t buffer_count
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12SwapchainImpl>(self);
    if (buffer_count < impl.backbuffer_count) {
        return wis::detail::make_result<
            wis::detail::Func(),
            "Provided buffer count is less than the number of swapchain backbuffers">(E_INVALIDARG);
    }

    for (uint32_t i = 0; i < impl.backbuffer_count; i++) {
        ID3D12Resource* backbuffer = nullptr;
        HRESULT hr = impl.swapchain->GetBuffer(i, IID_ID3D12Resource, reinterpret_cast<void**>(&backbuffer));
        if (!wis::detail::succeeded(hr)) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to get swap chain buffer resource">(hr);
        }
        *new (&buffers[i]) wis::impl::DX12TextureImpl{
            .resource = backbuffer,
        };
    }
    return wis::detail::dx_success;
}

#endif // WIS_DX12_SWAPCHAIN_CPP
