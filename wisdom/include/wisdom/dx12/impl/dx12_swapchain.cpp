#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/dx12/dx12_swapchain.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/util/misc.h>

wis::Result wis::detail::DX12SwapChainCreateInfo::InitBackBuffers() noexcept
{
    size_t frame_count = 0;

    DXGI_SWAP_CHAIN_DESC1 desc;
    chain->GetDesc1(&desc);
    frame_count = desc.BufferCount;

    if (frame_count > back_buffer_count) {
        back_buffers = wis::detail::make_unique_for_overwrite<wis::DX12Texture[]>(frame_count);
        if (!back_buffers)
            return wis::make_result<FUNC, "Out of memory">(E_OUTOFMEMORY);
    }

    for (uint32_t n = 0; n < frame_count; n++) {
        wis::com_ptr<ID3D12Resource> rc;
        if (!succeeded(chain->GetBuffer(n, __uuidof(ID3D12Resource), rc.put_void()))) {
            back_buffer_count = n + 1;
            break;
        }
        back_buffers[n] = wis::DX12Texture(std::move(rc), nullptr, nullptr);
    }
    back_buffer_count = frame_count;
    return wis::success;
}

wis::Result wis::DX12SwapChain::Resize(uint32_t width, uint32_t height) noexcept
{
    if (width == 0 || height == 0)
        return wis::make_result<FUNC, "Invalid size">(E_INVALIDARG);

    for (uint32_t n = 0; n < back_buffer_count; n++) {
        back_buffers[n] = {};
    }

    HRESULT hr = chain->ResizeBuffers(back_buffer_count, width, height, DXGI_FORMAT_UNKNOWN, 0);

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to resize swap chain">(hr);

    for (uint32_t n = 0; n < back_buffer_count; n++) {
        wis::com_ptr<ID3D12Resource> rc;
        if (!wis::succeeded(hr = chain->GetBuffer(n, __uuidof(ID3D12Resource), rc.put_void())))
            return wis::make_result<FUNC, "Failed to get back buffer">(hr);

        back_buffers[n] = wis::DX12Texture(std::move(rc), nullptr, nullptr);
    }
    return wis::success;
}