#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/xdx12/dx12_views.h>
#include <wisdom/xdx12/dx12_resource.h>
#include <memory>

namespace wis {
class DX12SwapChain;
class DX12Device;

namespace detail {
struct DX12SwapChainCreateInfo {
    wis::com_ptr<IDXGISwapChain4> chain;
    std::unique_ptr<DX12Texture[]> back_buffers;
    size_t back_buffer_count = 0;

    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers() noexcept;
};
}


template<>
struct Internal<DX12SwapChain> : public detail::DX12SwapChainCreateInfo {
};

class DX12SwapChain : public QueryInternal<DX12SwapChain>
{
public:
    DX12SwapChain() noexcept = default;
    explicit DX12SwapChain(detail::DX12SwapChainCreateInfo&& swap_i) noexcept
        : QueryInternal(std::move(swap_i))
    {
        
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