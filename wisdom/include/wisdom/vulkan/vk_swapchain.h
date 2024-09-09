#ifndef VK_SWAPCHAIN_H
#define VK_SWAPCHAIN_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_command_list.h>

namespace wis {
class VKSwapChain;
class VKDevice;

namespace detail {
struct VKSwapChainCreateInfo {
    wis::SharedSurface surface;
    wis::SharedDevice device;

    VkPhysicalDevice adapter = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR getCaps = nullptr;

    h::VkSwapchainKHR swapchain = nullptr;
    h::VkCommandBuffer initialization = nullptr;
    h::VkCommandPool command_pool = nullptr;
    h::VkQueue present_queue = nullptr;
    h::VkQueue graphics_queue = nullptr;
    std::unique_ptr<VkSemaphore[]> present_semaphores = nullptr; // if signalled, it means the presentation is required
    std::unique_ptr<VkSemaphore[]> image_ready_semaphores = nullptr; // if signalled, it means the rendering is available
    std::unique_ptr<VKTexture[]> back_buffers;
    h::VkFence fence = nullptr; // only used for initialization and resizing

    VkSurfaceFormatKHR format{};
    uint64_t present_id = 0;

    uint32_t back_buffer_count = 0;
    mutable uint32_t present_index = 0;
    mutable VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

    uint8_t supported_presentations = 0;
    bool stereo = false;
    bool tearing = false;
    bool stereo_requested = false;
    mutable uint8_t acquire_index = 0;

public:
    VKSwapChainCreateInfo() = default;
    VKSwapChainCreateInfo(wis::SharedSurface surface,
                          wis::SharedDevice device,
                          VkPhysicalDevice adapter,
                          PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR getCaps,
                          VkSwapchainKHR swapchain,
                          VkCommandBuffer initialization,
                          VkCommandPool command_pool,
                          VkQueue present_queue,
                          VkQueue graphics_queue,
                          VkSurfaceFormatKHR format,
                          VkPresentModeKHR present_mode, uint8_t supported_presentations,
                          bool tearing,
                          bool stereo, bool stereo_requested) noexcept
        : surface(std::move(surface))
        , device(std::move(device))
        , adapter(adapter)
        , getCaps(getCaps)
        , swapchain(swapchain)
        , initialization(initialization)
        , command_pool(command_pool)
        , present_queue(present_queue)
        , graphics_queue(graphics_queue)
        , format(format)
        , present_mode(present_mode)
        , supported_presentations(supported_presentations)
        , tearing(tearing)
        , stereo(stereo)
        , stereo_requested(stereo_requested)
    {
    }
    VKSwapChainCreateInfo(const VKSwapChainCreateInfo&) = delete;
    VKSwapChainCreateInfo& operator=(const VKSwapChainCreateInfo&) = delete;
    VKSwapChainCreateInfo(VKSwapChainCreateInfo&&) noexcept = default;
    WIS_INLINE VKSwapChainCreateInfo& operator=(VKSwapChainCreateInfo&&) noexcept;

    ~VKSwapChainCreateInfo() noexcept
    {
        Destroy();
    }

    WIS_INLINE void Destroy() noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::Result InitSemaphores() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers(VkExtent2D image_size) noexcept;
    [[nodiscard]] WIS_INLINE wis::Result AquireNextIndex() const noexcept;
    WIS_INLINE void ReleaseSemaphores() noexcept;
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
    [[nodiscard]] uint32_t GetCurrentIndex() const noexcept
    {
        return present_index;
    }

    [[nodiscard]] bool StereoSupported() const noexcept
    {
        return stereo;
    }

    [[nodiscard]] WIS_INLINE wis::Result Resize(uint32_t width, uint32_t height) noexcept;

    [[nodiscard]] WIS_INLINE wis::Result Present() const noexcept;

    [[nodiscard]] WIS_INLINE wis::Result Present2(bool vsync) const noexcept;

    [[nodiscard]] std::pair<const VKTexture*, uint32_t> GetBuffers() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] std::span<const VKTexture> GetBufferSpan() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] WIS_INLINE wis::Result WaitForPresent(uint64_t timeout_ns = std::numeric_limits<uint64_t>::max()) const noexcept;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY

#endif // VK_SWAPCHAIN_H
