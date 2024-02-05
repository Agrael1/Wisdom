#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/util/com_ptr.h>
#include <wisdom/dx12/xdx12_views.h>
#include <wisdom/dx12/xdx12_checks.h>
#include <d3d11.h>
#include <dxgi1_6.h>

#include <vector>
#include <memory>

namespace wis {
class DX12SwapChain;
class DX12Device;

struct DX12SwapchainHelpers {
    static WIS_INLINE void ToSwapchainDesc(DXGI_SWAP_CHAIN_DESC1& swap_desc, const wis::SwapchainDesc* desc) noexcept;
    static WIS_INLINE wis::com_ptr<ID3D11Device> CreateD3D11Device() noexcept;
};

template<>
struct Internal<DX12SwapChain> {
    wis::com_ptr<IDXGISwapChain4> chain;
    std::unique_ptr<wis::com_ptr<ID3D12Resource>[]> back_buffers;
    size_t back_buffer_count = 0;
};

class DX12SwapChain : public QueryInternal<DX12SwapChain>
{
public:
    DX12SwapChain() noexcept = default;
    explicit DX12SwapChain(wis::com_ptr<IDXGISwapChain4> in_chain) noexcept
        : QueryInternal(std::move(in_chain))
    {
        GetBuffers();
    }
    operator bool() const noexcept { return bool(chain); }

public:
private:
    WIS_INLINE void
    GetBuffers() noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/dx12/impl/dx12_swapchain.h>
#endif // !WISDOM_HEADER_ONLY