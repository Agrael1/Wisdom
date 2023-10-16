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
    wis::managed_handle_ex<VkSemaphore> fence;
};

class VKFence : public QueryInternal<VKFence>
{
public:
    VKFence() = default;
    explicit VKFence(wis::managed_handle_ex<VkSemaphore> in_fence) noexcept
        : QueryInternal(std::move(in_fence))
    {
    }
    operator VKFenceView() const noexcept
    {
        return fence.get();
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
        auto& device  = fence.header().parent;
        uint64_t value = 0;
        std::ignore = device.table()->vkGetSemaphoreCounterValue(device.get(), fence.get(), &value); // always succeeds
        return value;
    }

    /// @brief Wait for the fence to reach a certain value.
    /// @param value Value to wait for.
    /// @return Boolean indicating whether the fence reached the value.
    wis::Result Wait(uint64_t value, uint64_t wait_ns = std::numeric_limits<uint64_t>::max()) const noexcept // NOLINT
    {
        if (GetCompletedValue() >= value)
            return wis::success;

        auto& device = fence.header().parent;
        auto xfence = this->fence.get();
        VkSemaphoreWaitInfo waitInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO, nullptr, 0, 1, &xfence, &value };
        VkResult result = device.table()->vkWaitSemaphores(device.get(), &waitInfo, wait_ns);

        return succeeded(result)
                ? wis::success
                : wis::make_result<FUNC, "vkWaitSemaphores failed to wait for fence.">(result);
    }

    /// @brief Signal the fence from CPU.
    /// @param value Value to signal.
    wis::Result Signal(uint64_t value) const noexcept
    {
        auto& device = fence.header().parent;
        VkSemaphoreSignalInfo signalInfo{
            VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
            nullptr,
            fence.get(),
            value
        };
        VkResult res = device.table()->vkSignalSemaphore(device.get(), &signalInfo);
        return succeeded(res)
                ? wis::success
                : wis::make_result<FUNC, "vkSignalSemaphore failed to signal fence.">(res);
    }
};
} // namespace wis
