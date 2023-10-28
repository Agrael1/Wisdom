#pragma once
#include <wisdom/vulkan/xvk_fence.h>
#include <wisdom/vulkan/xvk_allocator.h>
#include <wisdom/vulkan/xvk_descriptors.h>
#include <wisdom/vulkan/xvk_command_queue.h>
#include <wisdom/vulkan/xvk_root_signature.h>
#include <wisdom/vulkan/vk_queue_residency.h>
#include <vector>

namespace wis {
struct InternalFeatures {
    bool has_descriptor_buffer : 1 = false;
    bool push_descriptor_bufferless : 1 = false;
};
;

class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::shared_handle<VkInstance> instance;
    wis::SharedDevice device;
    wis::VKAdapterHandle adapter;
    InternalFeatures ifeatures;

    std::shared_ptr<VmaVulkanFunctions> allocator_functions;
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
    friend std::pair<wis::Result, wis::VKDevice>
    VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept;

public:
    VKDevice() noexcept = default;
    WIS_INLINE explicit VKDevice(wis::shared_handle<VkInstance> instance,
                                 wis::SharedDevice device,
                                 wis::VKAdapterHandle adapter,
                                 wis::DeviceFeatures features = wis::DeviceFeatures::None,
                                 InternalFeatures ifeatures = {}) noexcept;

    operator bool() const noexcept { return bool(device); }
    operator VKDeviceHandle() const noexcept { return device; }

public:
    [[nodicard]] WIS_INLINE wis::Result
    WaitForMultipleFences(const VKFenceView* fences,
                          const uint64_t* values,
                          uint32_t count,
                          MutiWaitFlags wait_all = MutiWaitFlags::All,
                          uint64_t timeout = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKFence>
    CreateFence(uint64_t initial_value = 0ull) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKResourceAllocator>
    CreateAllocator() const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKRootSignature>
    CreateRootSignature(RootConstant* constants = nullptr, uint32_t constants_size = 0) const noexcept;

private:
    std::pair<wis::Result, VkDescriptorSetLayout>
    CreatePushDescriptorLayout(wis::PushDescriptor desc) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, VmaAllocator>
    CreateAllocatorI() const noexcept;
private:
    detail::QueueResidency queues;
    wis::DeviceFeatures features;
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::VKDevice>
VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept;

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/impl/vk_device.h>
#endif // !WISDOM_HEADER_ONLY