#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>
#endif

namespace wis {
class VKFence;

template<>
class Internal<VKFence>
{
public:
    wis::shared_handle<vk::Semaphore> fence;
    vk::Device device; // little overhead for better performance
};

WIS_EXPORT class VKFence : public QueryInternal<VKFence>
{
public:
    VKFence() = default;
    explicit VKFence(wis::shared_handle<vk::Semaphore> fence) noexcept
        : QueryInternal(std::move(fence), fence.getParent().get())
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
    [[nodiscard]] uint64_t GetCompletedValue() const noexcept
    {
        return device.getSemaphoreCounterValue(fence.get()).value;
    }

    /// @brief Wait for the fence to reach a certain value.
    /// @param value Value to wait for.
    /// @return Boolean indicating whether the fence reached the value.
    bool Wait(uint64_t value) const noexcept // NOLINT
    {
        if (GetCompletedValue() >= value)
            return true;

        auto s = fence.get();
        vk::SemaphoreWaitInfo waitInfo{ {}, 1, &s, &value };
        return succeeded(device.waitSemaphores(waitInfo, std::numeric_limits<uint64_t>::max()));
    }

    /// @brief Signal the fence from CPU.
    /// @param value Value to signal.
    void Signal(uint64_t value) noexcept
    {
        vk::SemaphoreSignalInfo signalInfo{
            fence.get(),
            value
        };
        std::ignore = device.signalSemaphore(&signalInfo);
    }
};
} // namespace wis
