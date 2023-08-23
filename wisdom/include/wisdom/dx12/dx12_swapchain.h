#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>
#include <wisdom/dx12/dx12_resource.h>
#include <dxgi1_5.h>
#endif

namespace wis {
class DX12SwapChain;

template<>
class Internal<DX12SwapChain>
{
public:
    winrt::com_ptr<IDXGISwapChain4> chain{};
};

/// @brief SwapChain implementation for DX12
WIS_EXPORT class DX12SwapChain : public QueryInternal<DX12SwapChain>
{
public:
    DX12SwapChain() = default;
    explicit DX12SwapChain(winrt::com_ptr<IDXGISwapChain4> xchain, uint32_t frame_count, bool stereo)
        : QueryInternal(std::move(xchain)), stereo(stereo)
    {
        render_targets.reserve(frame_count);
        for (uint32_t n = 0; n < frame_count; n++) {
            winrt::com_ptr<ID3D12Resource> rc;
            wis::succeeded(chain->GetBuffer(n, __uuidof(ID3D12Resource), rc.put_void()));
            render_targets.emplace_back(std::move(rc), nullptr);
        }
    }
    operator bool() const noexcept
    {
        return chain != nullptr;
    }

public:
    /// @brief Get the current image index in the swapchain
    /// @return Index of the current image
    [[nodiscard]] uint32_t
    GetNextIndex() const noexcept
    {
        return chain->GetCurrentBackBufferIndex();
    }

    /// @brief Get all the render targets in the swapchain
    /// @return Span of render targets
    [[nodiscard]] std::span<const DX12Texture>
    GetRenderTargets() const noexcept
    {
        return render_targets;
    }

    /// @brief Get the current render target in the swapchain
    /// @return Buffer view of the current render target TODO: Make a texture view
    [[nodiscard]] DX12TextureView
    GetBackBuffer() const noexcept
    {
        return render_targets[chain->GetCurrentBackBufferIndex()];
    }

    /// @brief Present the swapchain
    /// @return true if succeeded
    bool Present() noexcept
    {
        return wis::succeeded(chain->Present(0, 0));
    }

    /// @brief Check if stereo is supported
    /// @return true if stereo is supported
    [[nodiscard]] bool
    StereoSupported() const noexcept
    {
        return stereo;
    }

    /// @brief Resize the swapchain
    /// For the method to succeed, all swapchain buffers must be released first
    /// @param width New width
    /// @param height New height
    /// @return true if succeeded
    [[nodiscard]] bool
    Resize(uint32_t width, uint32_t height) noexcept
    {
        size_t current_size = render_targets.size();
        render_targets.clear(); // release all resources
        if (!wis::succeeded(chain->ResizeBuffers(current_size, width, height, DXGI_FORMAT_UNKNOWN, 0)))
            return false;

        for (uint32_t n = 0; n < current_size; n++) {
            winrt::com_ptr<ID3D12Resource> rc;
            if (!wis::succeeded(chain->GetBuffer(n, __uuidof(ID3D12Resource), rc.put_void())))
                return false;
            render_targets.emplace_back(std::move(rc), nullptr);
        }
        return true;
    }

private:
    std::vector<DX12Texture> render_targets; //< Render targets
    bool stereo = false; //< Stereo support
};
} // namespace wis
