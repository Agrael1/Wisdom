#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_unique_event.h>
#include <memory>

namespace wis {
class DX12SwapChain;
class DX12Device;

namespace detail {
struct DX12SwapChainCreateInfo {
    wis::com_ptr<IDXGISwapChain4> chain;
    std::unique_ptr<DX12Texture[]> back_buffers;
    wis::unique_event present_event = nullptr;
    uint32_t back_buffer_count = 0;
    bool stereo = false;
    bool vsync = true;

    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers() noexcept;
};
} // namespace detail

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
    operator bool() const noexcept
    {
        return bool(chain);
    }

public:
    /// @brief Get the current image index in the swapchain
    /// @return Index of the current image
    [[nodiscard]] uint32_t
    GetCurrentIndex() const noexcept
    {
        return chain->GetCurrentBackBufferIndex();
    }
    /// @brief Check if stereo is supported
    /// @return true if stereo is supported
    [[nodiscard]] bool StereoSupported() const noexcept
    {
        return stereo;
    }

    /// @brief Resize the swapchain
    /// For the method to succeed, all swapchain buffers must be released first
    /// @param width New width
    /// @param height New height
    /// @return true if succeeded
    [[nodiscard]] WIS_INLINE wis::Result
    Resize(uint32_t width, uint32_t height) noexcept;

    /// @brief Present the swapchain
    /// @return true if succeeded
    [[nodiscard]] wis::Result Present() noexcept
    {
        auto hr = chain->Present(vsync, 0);
        if (!wis::succeeded(hr)) {
            return wis::make_result<FUNC, "Presentation failed">(hr);
        }
        return wis::success;
    }

    [[nodiscard]] std::pair<const DX12Texture*, uint32_t> GetBuffers() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] std::span<const DX12Texture> GetBufferSpan() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] wis::Result WaitForPresent(uint64_t timeout_ns = std::numeric_limits<uint64_t>::max()) const noexcept
    {
        auto st = present_event.wait(uint32_t(timeout_ns / 1000));
        return st == wis::Status::Timeout  ? wis::Result{ st, "Wait timed out" }
                : st != wis::Status::Error ? wis::success
                                           : wis::make_result<FUNC, "Failed to wait for event">(E_FAIL);
    }
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY
