#ifndef VK_DEVICE_H
#define VK_DEVICE_H
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_queue_residency.h>
#include <wisdom/vulkan/vk_command_queue.h>
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_shader.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_descriptor_buffer.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>

namespace wis {
constexpr static inline std::array required_extensions{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, // for Swapchain
    VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME, // for Fence
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME, // for barriers

    VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, // for Allocator
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, // for Allocator
    VK_KHR_BIND_MEMORY_2_EXTENSION_NAME, // for Allocator
    VK_KHR_MAINTENANCE_4_EXTENSION_NAME, // for Allocator

    VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, // for PushDescriptor
    VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME, // for Tessellation control point count
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, // for dynamic render pass

    VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME, // for Mutable Descriptor Type
    VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,

    VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME, // for Border Color

    // VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    // VK_KHR_RAY_QUERY_EXTENSION_NAME,
    // VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
};

struct InternalFeatures {
    bool has_descriptor_buffer : 1 = false;
    bool push_descriptor_bufferless : 1 = false;
    bool dynamic_rendering : 1 = false;
    bool has_mutable_descriptor : 1 = false;
    uint32_t max_ia_attributes = 0;
};

struct FeatureDetails {
    VkPhysicalDeviceDescriptorBufferPropertiesEXT descriptor_buffer_properties;
    VkDescriptorType biggest_descriptor;
    uint32_t mutable_descriptor_size = 0;
};

class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::VKAdapter adapter;
    wis::SharedDevice device;
    InternalFeatures ifeatures;

    mutable wis::shared_handle<VmaAllocator> allocator;
    mutable std::shared_ptr<VmaVulkanFunctions> allocator_functions;

    detail::QueueResidency queues;

    std::unique_ptr<FeatureDetails> feature_details;

public:
    auto& GetInstanceTable() const noexcept
    {
        return adapter.GetInternal().instance.table();
    }
};

class VKDevice : public QueryInternal<VKDevice>
{
    friend wis::ResultValue<wis::VKDevice>
    VKCreateDevice(wis::VKAdapter in_adapter) noexcept;

public:
    VKDevice() noexcept = default;
    WIS_INLINE explicit VKDevice(wis::SharedDevice device,
                                 wis::VKAdapter adapter,
                                 std::unique_ptr<FeatureDetails> feature_details,
                                 wis::DeviceFeatures features = wis::DeviceFeatures::None,
                                 InternalFeatures ifeatures = {}) noexcept;

    operator bool() const noexcept
    {
        return bool(device);
    }
    operator VKDeviceHandle() const noexcept
    {
        return device;
    }

public:
    WIS_INLINE wis::Result
    WaitForMultipleFences(const VKFenceView* fences,
                          const uint64_t* values,
                          uint32_t count,
                          MutiWaitFlags wait_all = MutiWaitFlags::All,
                          uint64_t timeout = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKFence>
    CreateFence(uint64_t initial_value = 0ull) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKCommandQueue>
    CreateCommandQueue(wis::QueueType type, wis::QueuePriority priority = wis::QueuePriority::Common) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKCommandList>
    CreateCommandList(wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKPipelineState>
    CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRootSignature>
    CreateRootSignature(const RootConstant* constants = nullptr,
                        uint32_t constants_size = 0,
                        const wis::DescriptorTable* tables = nullptr,
                        uint32_t tables_count = 0) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKShader>
    CreateShader(void* bytecode, uint32_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKResourceAllocator>
    CreateAllocator() const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRenderTarget>
    CreateRenderTarget(VKTextureView texture, wis::RenderTargetDesc desc) const noexcept;


    [[nodiscard]] WIS_INLINE wis::ResultValue<VKSampler>
    CreateSampler(const wis::SamplerDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKShaderResource>
    CreateShaderResource(VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept;

    // Descriptor Buffer
    [[nodiscard]] uint32_t
    GetDescriptorBufferTableAlignment([[maybe_unused]] wis::DescriptorHeapType heap) const noexcept
    {
        return feature_details->descriptor_buffer_properties.descriptorBufferOffsetAlignment;
    }

    [[nodiscard]] uint32_t
    GetDescriptorBufferUnitSize(wis::DescriptorHeapType heap) const noexcept
    {
        return heap == wis::DescriptorHeapType::Descriptor
                ? feature_details->mutable_descriptor_size
                : feature_details->descriptor_buffer_properties.samplerDescriptorSize;
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKDescriptorBuffer>
    CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t memory_bytes) const noexcept;


public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    VKCreateSwapChain(wis::SharedSurface surface, const SwapchainDesc* desc, VkQueue graphics_queue) const noexcept;

private:
    [[nodiscard]] WIS_INLINE wis::ResultValue<VmaAllocator>
    CreateAllocatorI() const noexcept;

    [[nodiscard]] wis::ResultValue<VkDescriptorSetLayout>
    CreateDescriptorSetLayout(const wis::DescriptorTable* table) const noexcept
    {
        return table->type == wis::DescriptorHeapType::Descriptor
                ? CreateDescriptorSetDescriptorLayout(table)
                : CreateDescriptorSetSamplerLayout(table);
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<VkDescriptorSetLayout>
    CreateDummyDescriptorSetLayout(const VkDescriptorSetLayoutBinding& binding) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VkDescriptorSetLayout>
    CreateDescriptorSetDescriptorLayout(const wis::DescriptorTable* table) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VkDescriptorSetLayout>
    CreateDescriptorSetSamplerLayout(const wis::DescriptorTable* table) const noexcept;

private:
    wis::DeviceFeatures features{};
};

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKDevice>
VKCreateDevice(wis::VKAdapter in_adapter) noexcept;
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DEVICE_H
