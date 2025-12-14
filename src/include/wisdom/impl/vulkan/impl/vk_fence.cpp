#ifndef WIS_VK_FENCE_CPP
#define WIS_VK_FENCE_CPP
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_fence.h>
#endif // !WISDOM_MODULE_DECL

/// @brief Signal the fence from CPU.
/// @param value Value to signal.

wis::Result wis::ImplVKFence::Signal(uint64_t value) const noexcept
{
    auto& device = fence.header().parent;
    VkSemaphoreSignalInfo signalInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO, nullptr, fence.get(),
                                      value };
    VkResult res = device.table().vkSignalSemaphore(device.get(), &signalInfo);
    return succeeded(res) ? wis::success
           : wis::make_result<wis::Func<wis::FuncD()>(), "vkSignalSemaphore failed to signal fence.">(res);
}

/// @brief Wait for the fence to reach a certain value.
/// @param value Value to wait for.
/// @return Boolean indicating whether the fence reached the value.

wis::Result wis::ImplVKFence::Wait(uint64_t value, uint64_t wait_ns) const noexcept // NOLINT
{
    auto& device = fence.header().parent;
    auto xfence = fence.get();
    VkSemaphoreWaitInfo wait_info{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .pNext = nullptr,
        .flags = 0,
        .semaphoreCount = 1,
        .pSemaphores = &xfence,
        .pValues = &value
    };
    VkResult result = device.table().vkWaitSemaphores(device.get(), &wait_info, wait_ns);

    return succeeded(result)
           ? wis::success
           : wis::make_result<wis::Func<wis::FuncD()>(), "vkWaitSemaphores failed to wait for fence.">(result);
}

/// @brief Get the current value of the fence.
/// @return Value of the fence.

uint64_t wis::ImplVKFence::GetCompletedValue() const noexcept
{
    auto& device = fence.header().parent;
    uint64_t value = 0;
    std::ignore = device.table().vkGetSemaphoreCounterValue(device.get(), fence.get(),
                  &value); // always succeeds
    return value;
}

#endif // VK_FENCE_CPP
