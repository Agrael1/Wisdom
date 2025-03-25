#ifndef WIS_DX12_SWAPCHAIN_H
#define WIS_DX12_SWAPCHAIN_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/generated/api/api.hpp>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_unique_event.h>
#include <memory>
#endif

namespace wis {
WISDOM_EXPORT class DX12SwapChain;

WISDOM_EXPORT
namespace detail {
struct DX12SwapChainCreateInfo {
    wis::com_ptr<IDXGISwapChain4> chain;
    std::unique_ptr<DX12Texture[]> back_buffers;
    wis::unique_event present_event = nullptr;
    uint32_t back_buffer_count = 0;
    bool stereo = false;
    bool vsync = true;
    bool tearing = false;

    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers() noexcept;
};
} // namespace detail

WISDOM_EXPORT
template<>
struct Internal<DX12SwapChain> : public detail::DX12SwapChainCreateInfo {
};

class ImplDX12SwapChain : public QueryInternal<DX12SwapChain>
{
public:
    ImplDX12SwapChain() noexcept = default;
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
            return wis::make_result<wis::Func<wis::FuncD()>(), "Presentation failed">(hr);
        }
        return wis::success;
    }

    [[nodiscard]] wis::Result Present2(bool in_vsync) noexcept
    {
        auto hr = chain->Present(in_vsync, 0);
        if (!wis::succeeded(hr)) {
            return wis::make_result<wis::Func<wis::FuncD()>(), "Presentation failed">(hr);
        }
        return wis::success;
    }

    [[nodiscard]] std::pair<const DX12Texture*, uint32_t> GetBuffers() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] const DX12Texture* GetBuffers(uint32_t* out_tex_count) const noexcept
    {
        *out_tex_count = back_buffer_count;
        return back_buffers.get();
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
                                           : wis::make_result<wis::Func<wis::FuncD()>(), "Failed to wait for event">(E_FAIL);
    }
};

#pragma region DX12SwapChain
/**
 * @brief Represents swap chain object for presenting images.
 * */
WISDOM_EXPORT
class DX12SwapChain : public wis::ImplDX12SwapChain
{
public:
    using wis::ImplDX12SwapChain::ImplDX12SwapChain;

public:
    /**
     * @brief Get the current image index in the swapchain.
     * @return Index of the current image.
     * */
    inline uint32_t GetCurrentIndex() const noexcept
    {
        return wis::ImplDX12SwapChain::GetCurrentIndex();
    }
    /**
     * @brief Check if stereo is supported.
     * @return true if stereo is supported.
     * */
    inline bool StereoSupported() const noexcept
    {
        return wis::ImplDX12SwapChain::StereoSupported();
    }
    /**
     * @brief Resize the swapchain.
     * Transition may be expensive.
     * For the method to succeed, all swapchain buffers must be destroyed first
     * @param width New width
     * @param height New height
     * */
    [[nodiscard]] inline wis::Result Resize(uint32_t width, uint32_t height) noexcept
    {
        return wis::ImplDX12SwapChain::Resize(width, height);
    }
    /**
     * @brief Present the swapchain.
     * Presentation always gets queued to the queue specified upon creation.
     * */
    [[nodiscard]] inline wis::Result Present() noexcept
    {
        return wis::ImplDX12SwapChain::Present();
    }
    /**
     * @brief Present the swapchain with vsync option.
     * Requires wis::DeviceFeature::DynamicVSync to be supported.
     * Otherwise is identical to wis::DX12SwapChain.
     * @param in_vsync Enable vsync.
     * */
    [[nodiscard]] inline wis::Result Present2(bool in_vsync) noexcept
    {
        return wis::ImplDX12SwapChain::Present2(in_vsync);
    }
    /**
     * @brief Wait for the presentation to finish.
     * @param timeout_ns The timeout in nanoseconds. Default is infinite.
     * */
    [[nodiscard]] inline wis::Result WaitForPresent(uint64_t timeout_ns = UINT64_MAX) const noexcept
    {
        return wis::ImplDX12SwapChain::WaitForPresent(timeout_ns);
    }
};
#pragma endregion DX12SwapChain

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !WIS_DX12_SWAPCHAIN_H