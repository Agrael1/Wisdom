#pragma once
#include <wisdom/vulkan/xvk_fence.h>
#include <wisdom/vulkan/xvk_command_queue.h>
#include <wisdom/vulkan/vk_queue_residency.h>
#include <vector>

namespace wis {

class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::shared_handle<VkInstance> instance;
    wis::SharedDevice device;
    wis::VKAdapterHandle adapter;

public:
    auto* GetAdapter() const noexcept
    {
        return std::get<0>(adapter);
    }
    auto* GetInstanceTable() const noexcept
    {
        return std::get<1>(adapter);
    }
};

class VKDevice : public QueryInternal<VKDevice>
{
public:
    VKDevice() noexcept = default;
    WIS_INLINE explicit VKDevice(wis::shared_handle<VkInstance> instance,
                                 wis::SharedDevice device,
                                 wis::VKAdapterHandle adapter) noexcept;
    operator bool() const noexcept { return bool(device); }
    operator VKDeviceHandle() const noexcept { return device; }

public:
    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKFence>
    CreateFence(uint64_t initial_value = 0ull) const noexcept;

private:
    detail::QueueResidency queues;
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::VKDevice>
VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept;

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/impl/vk_device.h>
#endif // !WISDOM_HEADER_ONLY