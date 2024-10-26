#ifndef WIS_VK_DEVICE_H
#define WIS_VK_DEVICE_H
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

class ImplVKDevice : public QueryInternal<VKDevice>
{
    friend wis::ResultValue<wis::VKDevice>
    ImplVKCreateDevice(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept;

public:
    ImplVKDevice() noexcept = default;
    WIS_INLINE explicit ImplVKDevice(wis::SharedDevice device,
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
    CreateFence(uint64_t initial_value = 0ull, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept;

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

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRenderTarget>
    CreateDepthStencilTarget(VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return CreateRenderTarget(texture, desc);
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKSampler>
    CreateSampler(const wis::SamplerDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKShaderResource>
    CreateShaderResource(VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept;

    // Descriptor Buffer
    [[nodiscard]] uint32_t
    GetDescriptorTableAlignment([[maybe_unused]] wis::DescriptorHeapType heap) const noexcept
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

    [[nodiscard]] WIS_INLINE bool
    QueryFeatureSupport(wis::DeviceFeature feature) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    VKCreateSwapChain(wis::SharedSurface surface, const SwapchainDesc* desc, VkQueue graphics_queue, void* pNext = nullptr) const noexcept;

private:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::shared_handle<VmaAllocator>>
    VKCreateAllocator(bool interop = false) const noexcept;

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

#pragma region VKDevice
/**
 * @brief Represents logical device.
 * Creates all the resources and commands for rendering.
 * */
class VKDevice : public wis::ImplVKDevice
{
public:
    using wis::ImplVKDevice::ImplVKDevice;

public:
    /**
     * @brief Waits on multiple fences simultaneously.
     * If wait_all is wis::MutiWaitFlags::All, waits for all fences to be signaled.
     * Otherwise waits for any fence to be signaled.
     * @param fences Array of fence views to wait on.
     * @param fence_values Fence values to wait fences to reach.
     * Array must have fence_count values.
     * @param fence_count How many fences to wait on.
     * @param wait_all Specifies the kind of wait.
     * All - waits for all fences to be signaled.
     * Any - waits for any fence to be signaled.
     * Default is wis::MutiWaitFlags::All
     * @param timeout The timeout in nanoseconds. If UINT64_MAX, waits indefinitely.
     * */
    [[nodiscard]] inline wis::Result WaitForMultipleFences(const wis::VKFenceView* fences, const uint64_t* fence_values, uint32_t fence_count, wis::MutiWaitFlags wait_all = wis::MutiWaitFlags::All, uint64_t timeout = UINT64_MAX) const noexcept
    {
        return wis::ImplVKDevice::WaitForMultipleFences(fences, fence_values, fence_count, wait_all, timeout);
    }
    /**
     * @brief Creates a fence with initial value and flags.
     * @param initial_value The initial value of the fence.
     * @param flags The flags of the fence.
     * @return wis::VKFence on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKFence> CreateFence(uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ImplVKDevice::CreateFence(initial_value, flags);
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::VKCommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKCommandQueue> CreateCommandQueue(wis::QueueType type) const noexcept
    {
        return wis::ImplVKDevice::CreateCommandQueue(type);
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::VKCommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKCommandList> CreateCommandList(wis::QueueType type) const noexcept
    {
        return wis::ImplVKDevice::CreateCommandList(type);
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::VKPipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKPipelineState> CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept
    {
        return wis::ImplVKDevice::CreateGraphicsPipeline(desc);
    }
    /**
     * @brief Creates a root signature object.
     * @param root_constants The root constants to create the root signature with.
     * @param constants_size The number of root constants.
     * @param tables The descriptor tables to create the root signature with.
     * @param tables_count The number of descriptor tables.
     * @return wis::VKRootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRootSignature> CreateRootSignature(const wis::RootConstant* root_constants = nullptr, uint32_t constants_size = 0, const wis::DescriptorTable* tables = nullptr, uint32_t tables_count = 0) const noexcept
    {
        return wis::ImplVKDevice::CreateRootSignature(root_constants, constants_size, tables, tables_count);
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::VKShader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKShader> CreateShader(void* data, uint32_t size) const noexcept
    {
        return wis::ImplVKDevice::CreateShader(data, size);
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::VKResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKResourceAllocator> CreateAllocator() const noexcept
    {
        return wis::ImplVKDevice::CreateAllocator();
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::VKRenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRenderTarget> CreateRenderTarget(wis::VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return wis::ImplVKDevice::CreateRenderTarget(std::move(texture), desc);
    }
    /**
     * @brief Creates a depth stencil target object.
     * Works only with depth formats.
     * Used with render passes.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * Does not work with 3D textures.
     * @return wis::VKRenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRenderTarget> CreateDepthStencilTarget(wis::VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return wis::ImplVKDevice::CreateDepthStencilTarget(std::move(texture), desc);
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::VKSampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKSampler> CreateSampler(const wis::SamplerDesc* desc) const noexcept
    {
        return wis::ImplVKDevice::CreateSampler(desc);
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::VKShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKShaderResource> CreateShaderResource(wis::VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept
    {
        return wis::ImplVKDevice::CreateShaderResource(std::move(texture), desc);
    }
    /**
     * @brief Returns the alignment of the descriptor table in bytes.
     * The value is used to correctly determine descriptor page alignment for descriptor buffer.
     * @param heap The type of the descriptor heap to get the alignment for.
     * @return The alignment of the descriptor table in bytes.
     * */
    inline uint32_t GetDescriptorTableAlignment(wis::DescriptorHeapType heap) const noexcept
    {
        return wis::ImplVKDevice::GetDescriptorTableAlignment(heap);
    }
    /**
     * @brief Returns the size of the descriptor buffer unit in bytes.
     * @param heap The type of the descriptor heap to get the unit size for.
     * @return The size of the descriptor buffer unit in bytes. Descriptor unit is the size of one descriptor.
     * */
    inline uint32_t GetDescriptorBufferUnitSize(wis::DescriptorHeapType heap) const noexcept
    {
        return wis::ImplVKDevice::GetDescriptorBufferUnitSize(heap);
    }
    /**
     * @brief Creates a descriptor buffer object.
     * @param heap_type The type of the descriptor heap to create the descriptor buffer with.
     * @param memory_type The type of the descriptor memory to create the descriptor buffer with.
     * @param size_bytes The number of bytes to allocate for the descriptor buffer.
     * @return wis::VKDescriptorBuffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKDescriptorBuffer> CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t size_bytes) const noexcept
    {
        return wis::ImplVKDevice::CreateDescriptorBuffer(heap_type, memory_type, size_bytes);
    }
    /**
     * @brief Queries if the device supports the feature.
     * @param feature The feature to query.
     * @return true if feature is supported. false otherwise.
     * */
    inline bool QueryFeatureSupport(wis::DeviceFeature feature) const noexcept
    {
        return wis::ImplVKDevice::QueryFeatureSupport(feature);
    }
};
#pragma endregion VKDevice

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKDevice>
ImplVKCreateDevice(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept;

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DEVICE_H
