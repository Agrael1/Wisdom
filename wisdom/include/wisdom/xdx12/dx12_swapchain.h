#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xdx12/dx12_views.h>
#include <wisdom/xdx12/dx12_checks.h>

namespace wis {
class DX12SwapChain;
class DX12Device;

template<>
struct Internal<DX12SwapChain> {
    wis::com_ptr<IDXGISwapChain4> chain;
    //std::unique_ptr<wis::com_ptr<ID3D12Resource>[]> back_buffers;
    //size_t back_buffer_count = 0;
};

class DX12SwapChain : public QueryInternal<DX12SwapChain>
{
public:
    DX12SwapChain() noexcept = default;
    explicit DX12SwapChain(wis::com_ptr<IDXGISwapChain4> in_chain) noexcept
        : QueryInternal(std::move(in_chain))
    {
        //GetBuffers();
    }
    operator bool() const noexcept { return bool(chain); }

public:
private:
    //WIS_INLINE void
    //GetBuffers() noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/dx12_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY