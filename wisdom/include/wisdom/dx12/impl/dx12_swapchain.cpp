#ifndef WIS_DX12_SWAPCHAIN_CPP
#define WIS_DX12_SWAPCHAIN_CPP
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/util/misc.h>
#endif

wis::Result wis::detail::DX12SwapChainCreateInfo::InitBackBuffers() noexcept
{
    uint32_t frame_count = 0;

    DXGI_SWAP_CHAIN_DESC1 desc;
    chain->GetDesc1(&desc);
    frame_count = desc.BufferCount;

    if (frame_count > back_buffer_count) {
        back_buffers = wis::detail::make_unique_for_overwrite<wis::DX12Texture[]>(frame_count);
        if (!back_buffers) {
            return wis::make_result<wis::Func<wis::FuncD()>(), "Out of memory">(E_OUTOFMEMORY);
        }
    }

    for (uint32_t n = 0; n < frame_count; n++) {
        auto& bb_internal = back_buffers[n].GetMutableInternal();
        if (!succeeded(chain->GetBuffer(n, __uuidof(*bb_internal.resource), bb_internal.resource.put_void()))) {
            back_buffer_count = n + 1;
            break;
        }
    }
    back_buffer_count = frame_count;
    return wis::success;
}

wis::Result wis::ImplDX12SwapChain::Resize(uint32_t width, uint32_t height) noexcept
{
    if (width == 0 || height == 0) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Invalid size">(E_INVALIDARG);
    }

    for (uint32_t n = 0; n < back_buffer_count; n++) {
        back_buffers[n] = {};
    }

    HRESULT hr = chain->ResizeBuffers(back_buffer_count,
                                      width,
                                      height,
                                      DXGI_FORMAT_UNKNOWN,
                                      DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING * uint32_t(tearing));

    if (!wis::succeeded(hr)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to resize swap chain">(hr);
    }

    for (uint32_t n = 0; n < back_buffer_count; n++) {
        auto& bb_internal = back_buffers[n].GetMutableInternal();
        if (!succeeded(chain->GetBuffer(n, __uuidof(*bb_internal.resource), bb_internal.resource.put_void()))) {
            back_buffer_count = n + 1;
            break;
        }
    }
    return wis::success;
}

#endif // WIS_DX12_SWAPCHAIN_CPP
