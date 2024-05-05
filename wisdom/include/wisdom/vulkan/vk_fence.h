#pragma once
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/global/internal.h>
#include <wisvk/vk_loader.hpp>
#include <limits>

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
    [[nodiscard]] WIS_INLINE uint64_t GetCompletedValue() const noexcept;

    /// @brief Wait for the fence to reach a certain value.
    /// @param value Value to wait for.
    /// @return Boolean indicating whether the fence reached the value.
    [[nodiscard]] WIS_INLINE wis::Result
    Wait(uint64_t value,
         uint64_t wait_ns = std::numeric_limits<uint64_t>::max()) const noexcept;

    /// @brief Signal the fence from CPU.
    /// @param value Value to signal.
    [[nodiscard]] WIS_INLINE wis::Result Signal(uint64_t value) const noexcept;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_fence.cpp"
#endif // !WISDOM_HEADER_ONLY
