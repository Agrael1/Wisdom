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
#include <wisdom/vulkan/vk_device_ext.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>

namespace wis {
class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::VKAdapter adapter;
    wis::SharedDevice device;
    wis::VKDeviceExtensionEmbedded1 ext1;

    wis::shared_handle<VmaAllocator> allocator;
    detail::QueueResidency queues;

public:
    auto& GetInstanceTable() const noexcept
    {
        return adapter.GetInternal().instance.table();
    }
    auto& GetDeviceTable() const noexcept
    {
        return device.table();
    }

    template<typename PFN>
    [[nodiscard]] PFN GetInstanceProcAddr(const char* name) const noexcept
    {
        return reinterpret_cast<PFN>(device.gtable().vkGetInstanceProcAddr(adapter.GetInternal().instance.get(), name));
    }
    template<typename PFN>
    [[nodiscard]] PFN GetDeviceProcAddr(const char* name) const noexcept
    {
        return device.GetDeviceProcAddr<PFN>(name);
    }
};

class VKDevice : public QueryInternal<VKDevice>
{
    friend wis::ResultValue<wis::VKDevice>
    VKCreateDeviceWithExtensions(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size) noexcept;

public:
    VKDevice() noexcept = default;
    WIS_INLINE explicit VKDevice(wis::SharedDevice device,
                                 wis::VKAdapter adapter,
                                 wis::VKDeviceExtensionEmbedded1 ext1) noexcept;

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
    CreateCommandQueue(wis::QueueType type) const noexcept;

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
        return ext1.GetInternal().descriptor_buffer_features.offset_alignment;
    }

    [[nodiscard]] uint32_t
    GetDescriptorBufferUnitSize(wis::DescriptorHeapType heap) const noexcept
    {
        auto& heap_features = ext1.GetInternal().descriptor_buffer_features;
        return heap == wis::DescriptorHeapType::Descriptor
               ? heap_features.mutable_descriptor_size
               : heap_features.sampler_size;
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKDescriptorBuffer>
    CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t memory_bytes) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    VKCreateSwapChain(wis::SharedSurface surface, const SwapchainDesc* desc, VkQueue graphics_queue) const noexcept;

private:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::shared_handle<VmaAllocator>>
            VKCreateAllocator(bool interop = false)const noexcept;

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
};

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKDevice>
VKCreateDevice(wis::VKAdapter in_adapter) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKDevice>
VKCreateDeviceWithExtensions(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size) noexcept;

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DEVICE_H
