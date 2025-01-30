#ifndef WIS_VK_SWAPCHAIN_H
#define WIS_VK_SWAPCHAIN_H
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
    h::VkSwapchainKHR swapchain = nullptr;
    h::VkCommandBuffer initialization = nullptr;
    h::VkCommandPool command_pool = nullptr;
    h::VkQueue present_queue = nullptr;
    h::VkQueue graphics_queue = nullptr;
    std::unique_ptr<VkSemaphore[]> render_completed_semaphore = nullptr;
    std::unique_ptr<VkSemaphore[]> image_ready_semaphores = nullptr; // if signalled, it means the rendering is available
    std::unique_ptr<VKTexture[]> back_buffers;
    std::array<h::VkFence, 2> fences = {}; // only used for initialization and resizing

    VkSurfaceFormatKHR format{};
    uint64_t present_id = 0;

    uint32_t back_buffer_count = 0;
    VkPresentScalingFlagsEXT scaling{}; 
    mutable uint32_t present_index = 0;
    mutable VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

    uint8_t supported_presentations = 0;
    bool stereo = false;
    bool tearing = false;
    bool stereo_requested = false;
    mutable uint8_t acquire_index = 0;

public:
    VKSwapChainCreateInfo() = default;
    VKSwapChainCreateInfo(VKSwapChainCreateInfo&&) noexcept = default;
    WIS_INLINE VKSwapChainCreateInfo& operator=(VKSwapChainCreateInfo&&) noexcept;
    ~VKSwapChainCreateInfo() noexcept
    {
        Destroy();
    }

    WIS_INLINE void Destroy() noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::Result InitBackBuffers(VkExtent2D image_size) noexcept;
    [[nodiscard]] WIS_INLINE wis::Result AcquireNextIndex() const noexcept;
    [[nodiscard]] WIS_INLINE wis::Result AcquireNextIndexAndWait() const noexcept;
    WIS_INLINE void ReleaseSemaphores() noexcept;
};
} // namespace detail

template<>
struct Internal<VKSwapChain> : detail::VKSwapChainCreateInfo {
};

class ImplVKSwapChain : public QueryInternal<VKSwapChain>
{
public:
    ImplVKSwapChain() = default;
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
    [[nodiscard]] const VKTexture* GetBuffers(uint32_t* out_textures_count) const noexcept
    {
        *out_textures_count = back_buffer_count;
        return back_buffers.get();
    }
    [[nodiscard]] std::span<const VKTexture> GetBufferSpan() const noexcept
    {
        return { back_buffers.get(), back_buffer_count };
    }
    [[nodiscard]] WIS_INLINE wis::Result WaitForPresent(uint64_t timeout_ns = std::numeric_limits<uint64_t>::max()) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::Result VKRecreateSwapchain(uint32_t width, uint32_t height, void* pNext) noexcept;
    [[nodiscard]] WIS_INLINE wis::Result VKPresent(void* pNext) const noexcept;
};

#pragma region VKSwapChain
/**
 * @brief Represents swap chain object for presenting images.
 * */
class VKSwapChain : public wis::ImplVKSwapChain
{
public:
    using wis::ImplVKSwapChain::ImplVKSwapChain;

public:
    /**
     * @brief Get the current image index in the swapchain.
     * @return Index of the current image.
     * */
    inline uint32_t GetCurrentIndex() const noexcept
    {
        return wis::ImplVKSwapChain::GetCurrentIndex();
    }
    /**
     * @brief Check if stereo is supported.
     * @return true if stereo is supported.
     * */
    inline bool StereoSupported() const noexcept
    {
        return wis::ImplVKSwapChain::StereoSupported();
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
        return wis::ImplVKSwapChain::Resize(width, height);
    }
    /**
     * @brief Present the swapchain.
     * Presentation always gets queued to the queue specified upon creation.
     * */
    [[nodiscard]] inline wis::Result Present() noexcept
    {
        return wis::ImplVKSwapChain::Present();
    }
    /**
     * @brief Present the swapchain with vsync option.
     * Requires wis::DeviceFeature::DynamicVSync to be supported.
     * Otherwise is identical to wis::VKSwapChain.
     * @param in_vsync Enable vsync.
     * */
    [[nodiscard]] inline wis::Result Present2(bool in_vsync) noexcept
    {
        return wis::ImplVKSwapChain::Present2(in_vsync);
    }
    /**
     * @brief Wait for the presentation to finish.
     * @param timeout_ns The timeout in nanoseconds. Default is infinite.
     * */
    [[nodiscard]] inline wis::Result WaitForPresent(uint64_t timeout_ns = UINT64_MAX) const noexcept
    {
        return wis::ImplVKSwapChain::WaitForPresent(timeout_ns);
    }
};
#pragma endregion VKSwapChain

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_swapchain.cpp"
#endif // !WISDOM_HEADER_ONLY

#endif // VK_SWAPCHAIN_H
