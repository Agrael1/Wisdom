#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xvulkan/vk_resource.h>
#include <wisdom/xvulkan/vk_command_list.h>

namespace wis {
class VKSwapChain;
class VKDevice;

namespace detail {
struct VKSwapChainCreateInfo {
    wis::managed_handle_ex<VkSwapchainKHR> swapchain;
    std::unique_ptr<VKTexture[]> back_buffers;
    uint32_t back_buffer_count = 0;
    VkFormat format = VK_FORMAT_UNDEFINED;

    VkCommandBuffer initialization = nullptr;
    VkCommandPool command_pool = nullptr;
    VkQueue present_queue = nullptr;

    mutable uint32_t present_index = 0;
    VkSemaphore present_semaphore = nullptr;
    VkSemaphore graphics_semaphore = nullptr;

public:
    VKSwapChainCreateInfo() = default;
    VKSwapChainCreateInfo(wis::managed_handle_ex<VkSwapchainKHR> swapchain,
                          VkCommandBuffer initialization,
                          VkCommandPool command_pool,
                          VkQueue present_queue,
                          VkFormat format) noexcept
        : swapchain(std::move(swapchain))
        , initialization(initialization)
        , command_pool(command_pool)
        , present_queue(present_queue)
        , format(format)
    {
    }
    VKSwapChainCreateInfo(const VKSwapChainCreateInfo&) = delete;
    VKSwapChainCreateInfo(VKSwapChainCreateInfo&& o) noexcept
        : swapchain(std::move(o.swapchain))
        , back_buffers(std::move(o.back_buffers))
        , back_buffer_count(o.back_buffer_count)
        , initialization(std::exchange(o.initialization, nullptr))
        , command_pool(std::exchange(o.command_pool, nullptr))
        , present_queue(std::exchange(o.present_queue, nullptr))
        , format(o.format)
    {
    }

    ~VKSwapChainCreateInfo() noexcept
    {
        if (!swapchain)
            return;

        auto& table = *swapchain.header().parent.table();
        if (command_pool) {
            table.vkDestroyCommandPool(swapchain.header().parent.get(), command_pool, nullptr);
        }
    }

public:
    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result AquireNextIndex() noexcept;
};
} // namespace detail

template<>
struct Internal<VKSwapChain> : detail::VKSwapChainCreateInfo {
};

class VKSwapChain : public QueryInternal<VKSwapChain>
{
public:
    VKSwapChain() = default;
    explicit VKSwapChain(detail::VKSwapChainCreateInfo internals) noexcept
        : QueryInternal(std::move(internals))
    {
    }
    operator bool() const noexcept
    {
        return bool(swapchain);
    }

public:
    /// @brief Get the current image index in the swapchain
    /// @return Index of the current image
    [[nodiscard]] uint32_t GetNextIndex() const noexcept
    {
        return present_index;
    }
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY

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