#ifndef WIS_VK_FENCE_H
#define WIS_VK_FENCE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/global/internal.h>
#include <wisvk/vk_loader.hpp>
#include <limits>
#endif // !WISDOM_MODULE_DECL

namespace wis {
WISDOM_EXPORT class VKFence;

WISDOM_EXPORT
template<>
struct Internal<VKFence> {
    wis::managed_handle_ex<VkSemaphore> fence;
};

class ImplVKFence : public QueryInternal<VKFence>
{
public:
    ImplVKFence() = default;
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
    [[nodiscard]] WIS_INLINE wis::Result
    Signal(uint64_t value) const noexcept;
};

#pragma region VKFence
/**
 * @brief Represents fence for synchronization of GPU timeline.
 * */
WISDOM_EXPORT
class VKFence : public wis::ImplVKFence
{
public:
    using wis::ImplVKFence::ImplVKFence;

public:
    /**
     * @brief Get the current value of the fence.
     * @return Value of the fence.
     * */
    inline uint64_t GetCompletedValue() const noexcept
    {
        return wis::ImplVKFence::GetCompletedValue();
    }
    /**
     * @brief Wait on CPU for the fence to reach a certain value.
     * @param value Value to wait for.
     * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
     * */
    [[nodiscard]] inline wis::Result Wait(uint64_t value, uint64_t wait_ns = UINT64_MAX) const noexcept
    {
        return wis::ImplVKFence::Wait(value, wait_ns);
    }
    /**
     * @brief Signal the fence from CPU.
     * @param value Value to signal.
     * */
    [[nodiscard]] inline wis::Result Signal(uint64_t value) const noexcept
    {
        return wis::ImplVKFence::Signal(value);
    }
};
#pragma endregion VKFence

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_fence.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // VK_FENCE_H
