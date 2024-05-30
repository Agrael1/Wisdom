#ifndef VK_FENCE_CPP
#define VK_FENCE_CPP
#include <wisdom/vulkan/vk_fence.h>

/// @brief Signal the fence from CPU.
/// @param value Value to signal.

wis::Result wis::VKFence::Signal(uint64_t value) const noexcept
{
    auto& device = fence.header().parent;
    VkSemaphoreSignalInfo signalInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO, nullptr, fence.get(),
                                      value };
    VkResult res = device.table().vkSignalSemaphore(device.get(), &signalInfo);
    return succeeded(res) ? wis::success
           : wis::make_result<FUNC, "vkSignalSemaphore failed to signal fence.">(res);
}

/// @brief Wait for the fence to reach a certain value.
/// @param value Value to wait for.
/// @return Boolean indicating whether the fence reached the value.

wis::Result wis::VKFence::Wait(uint64_t value, uint64_t wait_ns) const noexcept // NOLINT
{
    if (GetCompletedValue() >= value)
        return wis::success;

    auto& device = fence.header().parent;
    auto xfence = this->fence.get();
    VkSemaphoreWaitInfo waitInfo{
        VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO, nullptr, 0, 1, &xfence, &value
    };
    VkResult result = device.table().vkWaitSemaphores(device.get(), &waitInfo, wait_ns);

    return succeeded(result)
           ? wis::success
           : wis::make_result<FUNC, "vkWaitSemaphores failed to wait for fence.">(result);
}

/// @brief Get the current value of the fence.
/// @return Value of the fence.

uint64_t wis::VKFence::GetCompletedValue() const noexcept
{
    auto& device = fence.header().parent;
    uint64_t value = 0;
    std::ignore = device.table().vkGetSemaphoreCounterValue(device.get(), fence.get(),
                  &value); // always succeeds
    return value;
}

#endif // VK_FENCE_CPP
