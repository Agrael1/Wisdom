#pragma once
#include <wisdom/vulkan/xvk_views.h>
#include <wisdom/vulkan/xvk_checks.h>
#include <wisdom/api/internal.h>
#include <wisvk/vk_loader.hpp>
#include <wisdom/api/consts.h>

namespace wis {
class VKFence;

template<>
struct Internal<VKFence> {
    SharedDevice device;
    VkSemaphore fence;
};

class VKFence : public QueryInternal<VKFence>
{
public:
    VKFence() = default;
    explicit VKFence(SharedDevice in_device, VkSemaphore in_fence) noexcept
        : QueryInternal(std::move(in_device), in_fence)
    {
    }
    ~VKFence()
    {
        device.table()->vkDestroySemaphore(device.get(), fence, nullptr);
    }
    VKFence(VKFence&& o) noexcept
        : QueryInternal(std::move(o.device), std::exchange(o.fence, nullptr))
    {
    }
    VKFence& operator=(VKFence&& o) noexcept
    {
        std::swap(device, o.device);
        std::swap(fence, o.fence);
        return *this;
    }
    operator VKFenceView() const noexcept
    {
        return fence;
    }
    operator bool() const noexcept
    {
        return bool(fence);
    }

public:
    /// @brief Get the current value of the fence.
    /// @return Value of the fence.
    [[nodiscard]] uint64_t
    GetCompletedValue() const noexcept
    {
        uint64_t value = 0;
        std::ignore = device.table()->vkGetSemaphoreCounterValue(device.get(), fence, &value); // always succeeds
        return value;
    }

    /// @brief Wait for the fence to reach a certain value.
    /// @param value Value to wait for.
    /// @return Boolean indicating whether the fence reached the value.
    wis::Result Wait(uint64_t value) const noexcept // NOLINT
    {
        if (GetCompletedValue() >= value)
            return wis::success;

        VkSemaphoreWaitInfo waitInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO, nullptr, 0, 1, &fence, &value };
        VkResult result = device.table()->vkWaitSemaphores(device.get(), &waitInfo, std::numeric_limits<uint64_t>::max());

        return succeeded(result)
                ? wis::success
                : wis::make_result<FUNC, "vkWaitSemaphores failed to wait for fence.">(result);
    }

    /// @brief Signal the fence from CPU.
    /// @param value Value to signal.
    wis::Result Signal(uint64_t value) const noexcept
    {
        VkSemaphoreSignalInfo signalInfo{
            VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
            nullptr,
            fence,
            value
        };
        VkResult res = device.table()->vkSignalSemaphore(device.get(), &signalInfo);
        return succeeded(res)
                ? wis::success
                : wis::make_result<FUNC, "vkSignalSemaphore failed to signal fence.">(res);
    }
};
} // namespace wis
