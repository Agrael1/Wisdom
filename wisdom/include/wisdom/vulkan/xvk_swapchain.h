#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_views.h>

namespace wis {
class VKSwapChain;

template<>
struct Internal<VKSwapChain> {
    wis::managed_handle_ex<VkSwapchainKHR> swapchain;
};

class VKSwapChain : public QueryInternal<VKSwapChain>
{
public:
    VKSwapChain() = default;
    VKSwapChain(wis::managed_handle_ex<VkSwapchainKHR> swapchain) noexcept
        : QueryInternal(std::move(swapchain))
    {
    }
};
} // namespace wis

// namespace wis {
// class VKSwapChain;
//
// template<>
// struct Internal<VKSwapChain>
//{
//     Internal() = default;
//     Internal(wis::shared_handle<vk::SwapchainKHR> swap, vk::Queue graphics_queue, vk::SurfaceFormatKHR format, vk::PresentModeKHR present_mode) noexcept
//         : swap(std::move(swap))
//         , graphics_queue(graphics_queue)
//         , format(format)
//         , present_mode(present_mode)
//         , device(this->swap.getParent().get())
//     {
//         auto [result, value] = device.createSemaphoreUnique({});
//         auto [result2, value2] = device.createSemaphoreUnique({});
//         if (!succeeded(result) || !succeeded(result2))
//             return;
//         graphics_semaphore = std::move(value);
//         present_semaphore = std::move(value2);
//     }
//
//     [[nodiscard]] vk::SwapchainKHR GetSwapchain() const noexcept
//     {
//         return swap.get();
//     }
//     [[nodiscard]] vk::SurfaceKHR GetSurface() const noexcept
//     {
//         return swap.getSurface().get();
//     }
//
// public:
//     wis::shared_handle<vk::SwapchainKHR> swap;
//     vk::Device device;
//
//     vk::Queue graphics_queue;
//     vk::SurfaceFormatKHR format;
//     vk::PresentModeKHR present_mode = vk::PresentModeKHR::eFifo;
//
//     vk::UniqueSemaphore graphics_semaphore;
//     vk::UniqueSemaphore present_semaphore;
// };
//
// class VKSwapChain : public QueryInternal<VKSwapChain>
//{
// public:
//     VKSwapChain() = default;
//     WIS_INLINE explicit VKSwapChain(wis::shared_handle<vk::SwapchainKHR> xswap,
//                                     vk::Queue graphics_queue,
//                                     VKCommandQueue present_queue,
//                                     VKCommandList initialization,
//                                     vk::SurfaceFormatKHR format,
//                                     vk::PresentModeKHR present_mode,
//                                     bool stereo) noexcept;
//     VKSwapChain(VKSwapChain&&) noexcept = default;
//     VKSwapChain& operator=(VKSwapChain&&) noexcept = default;
//     ~VKSwapChain()
//     {
//         ReleaseSemaphore();
//     }
//     operator bool() const noexcept
//     {
//         return bool(swap) && bool(graphics_semaphore) && bool(present_semaphore);
//     }
//
//     /// @brief Get the current image index in the swapchain
//     /// @return Index of the current image
//     [[nodiscard]] uint32_t GetNextIndex() const noexcept
//     {
//         return present_index;
//     }
//
//     /// @brief Get all the render targets in the swapchain
//     /// @return Span of render targets
//     [[nodiscard]] std::span<const VKTexture> GetRenderTargets() const noexcept
//     {
//         return back_buffers;
//     }
//
//     /// @brief Get the current render target in the swapchain
//     /// @return Buffer view of the current render target TODO: Make a texture view
//     [[nodiscard]] VKTextureView GetBackBuffer() const noexcept
//     {
//         return back_buffers[present_index];
//     }
//
//     /// @brief Present the swapchain
//     /// @return true if succeeded
//     WIS_INLINE bool Present() noexcept;
//
//     /// @brief Check if stereo is supported
//     /// @return true if stereo is supported
//     [[nodiscard]] bool StereoSupported() const noexcept
//     {
//         return stereo;
//     }
//
//     /// @brief Resize the swapchain
//     /// For the method to succeed, all swapchain buffers must be released first
//     /// @param width New width
//     /// @param height New height
//     /// @return true if succeeded
//     [[nodiscard]] bool Resize(uint32_t width, uint32_t height) noexcept
//     {
//         uint32_t nbuffers = back_buffers.size();
//         back_buffers.clear();
//
//         vk::SwapchainKHR gswap = swap.get();
//         while (true) {
//
//             vk::SwapchainCreateInfoKHR desc{
//                 vk::SwapchainCreateFlagBitsKHR{}, GetSurface(),
//                 nbuffers, format.format, format.colorSpace,
//                 vk::Extent2D{ width, height },
//                 stereo ? 2u : 1u,
//                 vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
//                 vk::SharingMode::eExclusive, 0u, nullptr,
//                 vk::SurfaceTransformFlagBitsKHR::eIdentity,
//                 vk::CompositeAlphaFlagBitsKHR::eOpaque,
//                 present_mode, true, gswap // NOLINT
//             };
//
//             auto r = device.createSwapchainKHR(&desc, nullptr, &gswap);
//             if (r != vk::Result::eSuccess) {
//                 gswap = nullptr;
//                 continue;
//             }
//             swap = wis::shared_handle<vk::SwapchainKHR>{
//                 gswap,
//                 swap.getParent(), swap.getSurface()
//             };
//             CreateImages();
//             return true;
//         }
//     }
//
// private:
//     void CreateImages() noexcept
//     {
//         initialization.Reset();
//         auto [result, xback_buffers] = device.getSwapchainImagesKHR(swap.get());
//         if (!succeeded(result))
//             return;
//
//         back_buffers.reserve(xback_buffers.size());
//         for (auto& i : xback_buffers) {
//             back_buffers.emplace_back(
//                     format.format, wis::shared_handle<vk::Image>{ i, swap.getParent(), vk::SwapchainOwns::yes });
//             initialization.TextureBarrier(
//                     {
//                             .state_before = TextureState::Undefined,
//                             .state_after = TextureState::Present,
//                             .access_before = ResourceAccess::NoAccess,
//                             .access_after = ResourceAccess::NoAccess,
//                     },
//                     { i, format.format });
//         }
//         initialization.Close();
//         present_queue.ExecuteCommandList(initialization);
//         present_queue.GetInternal().queue.waitIdle();
//
//         AquireNextIndex();
//     }
//     WIS_INLINE bool AquireNextIndex() noexcept;
//     WIS_INLINE void ReleaseSemaphore() noexcept;
//
//     VKCommandQueue present_queue;
//     VKCommandList initialization; //< Initialization command list
//     std::vector<VKTexture> back_buffers; //< Render targets
//     mutable uint32_t present_index = 0; //< Current render target index
//     bool stereo = false; //< True if stereo is supported by the swapchain
// };
// } // namespace wis
//
// #if defined(WISDOM_HEADER_ONLY)
// #include "impl/vk_swapchain.inl"
// #endif
