#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xvulkan/vk_resource.h>
#include <wisdom/xvulkan/vk_command_list.h>

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
    h::VkSemaphore present_semaphore = nullptr; // if signalled, it means the presentation is done
    h::VkSemaphore graphics_semaphore = nullptr; // if signalled, it means the rendering is done

    std::unique_ptr<VKTexture[]> back_buffers;
    VkSurfaceFormatKHR format{};

    uint32_t back_buffer_count = 0;
    mutable uint32_t present_index = 0;
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

    bool stereo = false;
    bool stereo_requested = false;

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
                          VkPresentModeKHR present_mode,
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
        , stereo(stereo)
        , stereo_requested(stereo_requested)
    {
    }
    VKSwapChainCreateInfo(const VKSwapChainCreateInfo&) = delete;
    VKSwapChainCreateInfo& operator=(const VKSwapChainCreateInfo&) = delete;
    VKSwapChainCreateInfo(VKSwapChainCreateInfo&&) noexcept = default;
    VKSwapChainCreateInfo& operator=(VKSwapChainCreateInfo&&) noexcept = default;

    ~VKSwapChainCreateInfo() noexcept
    {
        if (!swapchain)
            return;

        auto& table = device.table();
        auto hdevice = device.get();

        ReleaseSemaphore();
        table.vkDestroySemaphore(hdevice, present_semaphore, nullptr);
        table.vkDestroySemaphore(hdevice, graphics_semaphore, nullptr);
        table.vkDestroyCommandPool(hdevice, command_pool, nullptr);
        table.vkDestroySwapchainKHR(hdevice, swapchain, nullptr);
    }

public:
    [[nodiscard]] WIS_INLINE wis::Result InitSemaphores() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers(VkExtent2D image_size) noexcept;
    [[nodiscard]] WIS_INLINE wis::Result AquireNextIndex() const noexcept;
    WIS_INLINE void ReleaseSemaphore() const noexcept;
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
    [[nodiscard]] uint32_t GetCurrentIndex() const noexcept { return present_index; }

    [[nodiscard]] bool StereoSupported() const noexcept { return stereo; }

    [[nodiscard]] WIS_INLINE wis::Result Resize(uint32_t width, uint32_t height) noexcept;

    [[nodiscard]] WIS_INLINE wis::Result Present() const noexcept;

    [[nodiscard]] std::pair<const VKTexture*, uint32_t> GetBuffers() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] std::span<const VKTexture> GetBufferSpan() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY
