#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/util/com_ptr.h>
#include <wisdom/dx12/xdx12_views.h>
#include <dxgi1_6.h>

namespace wis {
class DX12SwapChain;
class DX12Device;

template<>
struct Internal<DX12SwapChain> {
    wis::com_ptr<IDXGISwapChain4> chain;
};

class DX12SwapChain : public QueryInternal<DX12SwapChain>
{
public:
    DX12SwapChain() noexcept = default;
    explicit DX12SwapChain(wis::com_ptr<IDXGISwapChain4> chain)noexcept
        : QueryInternal(std::move(chain)) {}
    operator bool () const noexcept { return bool(chain); }

public:
};

[[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::DX12SwapChain>
DX12CreateSwapchainWin32(const DX12Device& device, DX12QueueView main_queue, const wis::SwapchainDesc* desc, void* hwnd) noexcept;
} // namespace wis


#ifdef WISDOM_HEADER_ONLY
#include <wisdom/dx12/impl/dx12_swapchain.h>
#endif // !WISDOM_HEADER_ONLY