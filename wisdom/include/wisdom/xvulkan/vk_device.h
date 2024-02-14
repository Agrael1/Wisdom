#pragma once
#include <wisdom/xvulkan/vk_fence.h>
#include <wisdom/xvulkan/vk_queue_residency.h>
//#include <wisdom/vulkan/xvk_allocator.h>
//#include <wisdom/vulkan/xvk_descriptors.h>
//#include <wisdom/vulkan/xvk_command_queue.h>
//#include <wisdom/vulkan/xvk_root_signature.h>
//#include <wisdom/vulkan/xvk_command_list.h>
//#include <wisdom/vulkan/xvk_pipeline_state.h>
//#include <wisdom/vulkan/vk_structs.hpp>
//#include <wisdom/vulkan/xvk_shader.h>
//#include <wisdom/vulkan/xvk_swapchain.h>
//#include <vector>

namespace wis {
struct InternalFeatures {
    bool has_descriptor_buffer : 1 = false;
    bool push_descriptor_bufferless : 1 = false;
    bool dynamic_rendering : 1 = false;
    uint32_t max_ia_attributes = 0;
};

class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::shared_handle<VkInstance> instance;
    wis::SharedDevice device;
    wis::VKAdapterHandle adapter;
    InternalFeatures ifeatures;

    std::shared_ptr<VmaVulkanFunctions> allocator_functions;

    detail::QueueResidency queues;

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

    /*[[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKResourceAllocator>
    CreateAllocator() const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKRootSignature>
    CreateRootSignature(RootConstant* constants = nullptr, uint32_t constants_size = 0) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKCommandQueue>
    CreateCommandQueue(wis::QueueType type, wis::QueuePriority priority = wis::QueuePriority::Common) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKShader>
    CreateShader(void* bytecode, uint32_t size) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKPipelineState>
    CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKCommandList>
    CreateCommandList(wis::QueueType type) const noexcept;

public:
    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::VKSwapChain>
    VKCreateSwapChain(wis::shared_handle<VkSurfaceKHR> surface, const SwapchainDesc* desc) const noexcept;*/

private:
    [[nodiscard]] WIS_INLINE std::pair<wis::Result, VkDescriptorSetLayout>
    CreatePushDescriptorLayout(wis::PushDescriptor desc) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, VmaAllocator>
    CreateAllocatorI() const noexcept;

private:
    wis::DeviceFeatures features;
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::VKDevice>
VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept;

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_device.cpp"
#endif // !WISDOM_HEADER_ONLY