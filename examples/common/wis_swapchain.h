#pragma once
#include "wis_helper.h"

namespace ex {
class Swapchain
{
public:
    Swapchain() = default;
    Swapchain(wis::Device& device, wis::SwapChain xswap, uint32_t width, uint32_t height, wis::DataFormat format = ex::swapchain_format, bool stereo = false)
        : swap(std::move(xswap)), width(width), height(height), stereo(stereo), format(format)
    {
        fence = Unwrap(device.CreateFence(0));
        textures = swap.GetBufferSpan();

        wis::RenderTargetDesc rt_desc{
            .format = format,
            .layout = stereo ? wis::TextureLayout::Texture2DArray : wis::TextureLayout::Texture2D,
            .layer_count = stereo ? 2u : 1u, // stereo uses multiview extension
        };
        for (size_t i = 0; i < render_targets.size(); i++) {
            render_targets[i] = Unwrap(device.CreateRenderTarget(textures[i], rt_desc));
        }
    }
    ~Swapchain()
    {
        if (swap) {
            Throttle();
        }
    }

public:
    void Throttle() noexcept
    {
        CheckResult(fence.Wait(fence_values[frame_index] - 1));
    }
    bool Present(wis::CommandQueue& main_queue)
    {
        auto res = swap.Present();
        if (res.status != wis::Status::Ok) {
            return false;
        }

        CheckResult(main_queue.SignalQueue(fence, fence_value));

        frame_index = swap.GetCurrentIndex() % ex::flight_frames;
        CheckResult(fence.Wait(fence_values[frame_index]));

        fence_values[frame_index] = ++fence_value;
        return true;
    }
    void Resize(wis::Device& device, uint32_t width, uint32_t height)
    {
        this->width = width;
        this->height = height;
        CheckResult(swap.Resize(width, height));
        textures = swap.GetBufferSpan();

        wis::RenderTargetDesc rt_desc{
            .format = format,
            .layout = stereo ? wis::TextureLayout::Texture2DArray : wis::TextureLayout::Texture2D,
            .layer_count = stereo ? 2u : 1u, // stereo uses multiview extension
        };
        for (size_t i = 0; i < render_targets.size(); i++) {
            render_targets[i] = Unwrap(device.CreateRenderTarget(textures[i], rt_desc));
        }
    }
    uint32_t CurrentFrame() const
    {
        return swap.GetCurrentIndex();
    }
    const wis::SwapChain& GetSwapChain() const
    {
        return swap;
    }
    uint32_t GetWidth() const
    {
        return width;
    }
    uint32_t GetHeight() const
    {
        return height;
    }

    std::span<const wis::Texture> GetTextures() const
    {
        return textures;
    }
    const wis::Texture& GetTexture(size_t i) const
    {
        return textures[i];
    }
    const wis::RenderTarget& GetRenderTarget(size_t i) const
    {
        return render_targets[i];
    }

private:
    wis::SwapChain swap;
    wis::Fence fence;
    uint64_t fence_value = 1;
    uint64_t frame_index = 0;
    std::array<uint64_t, ex::flight_frames> fence_values{ 1, 0 };

    std::span<const wis::Texture> textures;
    std::array<wis::RenderTarget, ex::swap_buffer_count> render_targets;

    wis::DataFormat format = ex::swapchain_format;
    uint32_t width = 0;
    uint32_t height = 0;
    bool stereo = false;
};
} // namespace ex
