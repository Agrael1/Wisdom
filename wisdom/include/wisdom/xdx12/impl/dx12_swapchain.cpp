#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_swapchain.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xdx12/dx12_checks.h>
#include <wisdom/util/misc.h>

wis::Result wis::detail::DX12SwapChainCreateInfo::InitBackBuffers()noexcept
{
    size_t frame_count = 0;

    DXGI_SWAP_CHAIN_DESC1 desc;
    chain->GetDesc1(&desc);
    frame_count = desc.BufferCount;

    if (frame_count > back_buffer_count)
    {
        back_buffers = wis::detail::make_unique_for_overwrite<wis::DX12Texture[]>(frame_count);
        if (!back_buffers)
            return wis::make_result <FUNC, "Out of memory">(E_OUTOFMEMORY);
    }

    for (uint32_t n = 0; n < frame_count; n++) {
        wis::com_ptr<ID3D12Resource> rc;
        if (!succeeded(chain->GetBuffer(n, __uuidof(ID3D12Resource), rc.put_void())))
        {
            back_buffer_count = n + 1;
            break;
        }
        back_buffers[n] = wis::DX12Texture(std::move(rc), nullptr);
    }
    back_buffer_count = frame_count;
    return wis::success;
}