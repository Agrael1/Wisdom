#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_views.h>
#include <wisdom/vulkan/vk_queue_residency.h>
#include <wisvk/vk_managed_handles.hpp>
#include <wisvk/vk_loader.hpp>
#include <memory>
#include <vector>

namespace wis {

class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::shared_handle<VkInstance> instance;
    wis::shared_handle<VkDevice> device;
    wis::VKAdapterHandle adapter;
    std::unique_ptr<VkDeviceTable> device_table;

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
                      wis::shared_handle<VkDevice> device,
                      wis::VKAdapterHandle adapter,
                      std::unique_ptr<VkDeviceTable> device_table) noexcept;
    operator bool() const noexcept { return bool(device); }

public:
private:
    detail::QueueResidency queues;
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::VKDevice>
VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept;

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/impl/vk_device.h>
#endif // !WISDOM_HEADER_ONLY