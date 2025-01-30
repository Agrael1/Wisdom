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
#include <wisdom/vulkan/vk_descriptor_storage.h>
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
    friend wis::VKDevice
    ImplVKCreateDevice(wis::Result& result, wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept;

public:
    ImplVKDevice() noexcept = default;
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

    [[nodiscard]] WIS_INLINE wis::VKFence
    CreateFence(wis::Result& result, uint64_t initial_value = 0ull, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKCommandQueue
    CreateCommandQueue(wis::Result& result, wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKCommandList
    CreateCommandList(wis::Result& result, wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKPipelineState
    CreateGraphicsPipeline(wis::Result& result, const wis::VKGraphicsPipelineDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKPipelineState
    CreateComputePipeline(wis::Result& result, const wis::VKComputePipelineDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKShader
    CreateShader(wis::Result& result, void* bytecode, uint32_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKResourceAllocator
    CreateAllocator(wis::Result& result) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKRenderTarget
    CreateRenderTarget(wis::Result& result, VKTextureView texture, wis::RenderTargetDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKRenderTarget
    CreateDepthStencilTarget(wis::Result& result, VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return CreateRenderTarget(result, texture, desc);
    }

    [[nodiscard]] WIS_INLINE wis::VKSampler
    CreateSampler(wis::Result& result, const wis::SamplerDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKShaderResource
    CreateShaderResource(wis::Result& result, wis::VKTextureView texture, const wis::ShaderResourceDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKUnorderedAccessTexture
    CreateUnorderedAccessTexture(wis::Result& result, wis::VKTextureView texture, const wis::UnorderedAccessDesc& desc) const noexcept
    {
        wis::ShaderResourceDesc re_desc{
            .format = desc.format,
            .view_type = desc.view_type,
            .subresource_range = desc.subresource_range,
        };
        return CreateShaderResource(result, texture, re_desc);
    }

    [[nodiscard]] WIS_INLINE bool
    QueryFeatureSupport(wis::DeviceFeature feature) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKDescriptorStorage
    CreateDescriptorStorage(wis::Result& result,
                            const wis::DescriptorBindingDesc* descriptor_bindings = nullptr,
                            uint32_t descriptor_bindings_count = 0,
                            wis::DescriptorMemory = wis::DescriptorMemory::ShaderVisible) const noexcept;

    [[nodiscard]] WIS_INLINE wis::VKRootSignature
    CreateRootSignature(wis::Result& result, const wis::PushConstant* push_constants = nullptr,
                        uint32_t constants_count = 0,
                        const wis::PushDescriptor* push_descriptors = nullptr,
                        uint32_t push_descriptors_count = 0,
                        const wis::DescriptorBindingDesc* descriptor_bindings = nullptr,
                        uint32_t descriptor_bindings_count = 0) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::VKSwapChain
    VKCreateSwapChain(wis::Result& result, wis::SharedSurface surface, const SwapchainDesc& desc, VkQueue graphics_queue, void* pNext = nullptr) const noexcept;

protected:
    [[nodiscard]] WIS_INLINE wis::shared_handle<VmaAllocator>
    VKCreateAllocator(wis::Result& result, bool interop = false) const noexcept;
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
    [[nodiscard]] inline wis::VKFence CreateFence(wis::Result& result, uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ImplVKDevice::CreateFence(result, initial_value, flags);
    }
    /**
     * @brief Creates a fence with initial value and flags.
     * @param initial_value The initial value of the fence.
     * @param flags The flags of the fence.
     * @return wis::VKFence on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKFence> CreateFence(uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ResultValue<wis::VKFence> { &wis::ImplVKDevice::CreateFence, this, initial_value, flags };
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::VKCommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKCommandQueue CreateCommandQueue(wis::Result& result, wis::QueueType type) const noexcept
    {
        return wis::ImplVKDevice::CreateCommandQueue(result, type);
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::VKCommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKCommandQueue> CreateCommandQueue(wis::QueueType type) const noexcept
    {
        return wis::ResultValue<wis::VKCommandQueue> { &wis::ImplVKDevice::CreateCommandQueue, this, type };
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::VKCommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKCommandList CreateCommandList(wis::Result& result, wis::QueueType type) const noexcept
    {
        return wis::ImplVKDevice::CreateCommandList(result, type);
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::VKCommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKCommandList> CreateCommandList(wis::QueueType type) const noexcept
    {
        return wis::ResultValue<wis::VKCommandList> { &wis::ImplVKDevice::CreateCommandList, this, type };
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::VKPipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKPipelineState CreateGraphicsPipeline(wis::Result& result, const wis::VKGraphicsPipelineDesc& desc) const noexcept
    {
        return wis::ImplVKDevice::CreateGraphicsPipeline(result, desc);
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::VKPipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKPipelineState> CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::VKPipelineState> { &wis::ImplVKDevice::CreateGraphicsPipeline, this, desc };
    }
    /**
     * @brief Creates a compute pipeline state object.
     * @param desc The description of the compute pipeline to create.
     * @return wis::VKPipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKPipelineState CreateComputePipeline(wis::Result& result, const wis::VKComputePipelineDesc& desc) const noexcept
    {
        return wis::ImplVKDevice::CreateComputePipeline(result, desc);
    }
    /**
     * @brief Creates a compute pipeline state object.
     * @param desc The description of the compute pipeline to create.
     * @return wis::VKPipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKPipelineState> CreateComputePipeline(const wis::VKComputePipelineDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::VKPipelineState> { &wis::ImplVKDevice::CreateComputePipeline, this, desc };
    }
    /**
     * @brief Creates a root signature object for use with DescriptorStorage.
     * DescriptorStorage is used for bindless and non-uniform bindings. Don't combine with Descriptor buffers, this may reduce performance.
     * Push constants and push descriptors are used for fast changing data.
     * Spaces may not overlap, but can be in any order. Push descriptors always have space0 and [[vk::binding(x,0)]].
     * That means that all the binding numbers are off by 1. Meaning that if you have Descriptor Storage with 1 binding, it will be [[vk::binding(0,1)]]
     * even though it is supposed to be binding 0. This is done for consistency.
     * Set number is the position of binding in bindings array. e.g. bindings[5] is set 5 and on HLSL side it is [[vk::binding(0,5)]].
     * For several overlapping types e.g. 2D and 3D textures, use different spaces.
     * Those are specified in the bindings array. Space overlap count means how many consecutive spaces are used by the binding.
     * @param push_constants The root constants to create the root signature with.
     * @param push_constant_count The number of push constants. Max is 5.
     * @param push_descriptors The root descriptors to create the root signature with.
     * In shader will appear in order of submission. e.g. push_descriptors[5] is [[vk::binding(5,0)]] ... : register(b5/t5/u5)
     * @param push_descriptor_count The number of push descriptors. Max is 8.
     * @param bindings The bindings to allocate. Order matters, binding count is ignored.
     * One block of bindings can contain up to 4096 descriptors. For Sampler blocks, max amount of samplers across all bindings is 2048.
     * @param binding_count Count of bindings to allocate. Max is 64 - push_constant_count - push_descriptor_count * 2.
     * @return wis::VKRootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKRootSignature CreateRootSignature(wis::Result& result, const wis::PushConstant* push_constants = nullptr, uint32_t push_constant_count = 0, const wis::PushDescriptor* push_descriptors = nullptr, uint32_t push_descriptor_count = 0, const wis::DescriptorBindingDesc* bindings = nullptr, uint32_t binding_count = 0) const noexcept
    {
        return wis::ImplVKDevice::CreateRootSignature(result, push_constants, push_constant_count, push_descriptors, push_descriptor_count, bindings, binding_count);
    }
    /**
     * @brief Creates a root signature object for use with DescriptorStorage.
     * DescriptorStorage is used for bindless and non-uniform bindings. Don't combine with Descriptor buffers, this may reduce performance.
     * Push constants and push descriptors are used for fast changing data.
     * Spaces may not overlap, but can be in any order. Push descriptors always have space0 and [[vk::binding(x,0)]].
     * That means that all the binding numbers are off by 1. Meaning that if you have Descriptor Storage with 1 binding, it will be [[vk::binding(0,1)]]
     * even though it is supposed to be binding 0. This is done for consistency.
     * Set number is the position of binding in bindings array. e.g. bindings[5] is set 5 and on HLSL side it is [[vk::binding(0,5)]].
     * For several overlapping types e.g. 2D and 3D textures, use different spaces.
     * Those are specified in the bindings array. Space overlap count means how many consecutive spaces are used by the binding.
     * @param push_constants The root constants to create the root signature with.
     * @param push_constant_count The number of push constants. Max is 5.
     * @param push_descriptors The root descriptors to create the root signature with.
     * In shader will appear in order of submission. e.g. push_descriptors[5] is [[vk::binding(5,0)]] ... : register(b5/t5/u5)
     * @param push_descriptor_count The number of push descriptors. Max is 8.
     * @param bindings The bindings to allocate. Order matters, binding count is ignored.
     * One block of bindings can contain up to 4096 descriptors. For Sampler blocks, max amount of samplers across all bindings is 2048.
     * @param binding_count Count of bindings to allocate. Max is 64 - push_constant_count - push_descriptor_count * 2.
     * @return wis::VKRootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRootSignature> CreateRootSignature(const wis::PushConstant* push_constants = nullptr, uint32_t push_constant_count = 0, const wis::PushDescriptor* push_descriptors = nullptr, uint32_t push_descriptor_count = 0, const wis::DescriptorBindingDesc* bindings = nullptr, uint32_t binding_count = 0) const noexcept
    {
        return wis::ResultValue<wis::VKRootSignature> { &wis::ImplVKDevice::CreateRootSignature, this, push_constants, push_constant_count, push_descriptors, push_descriptor_count, bindings, binding_count };
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::VKShader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKShader CreateShader(wis::Result& result, void* data, uint32_t size) const noexcept
    {
        return wis::ImplVKDevice::CreateShader(result, data, size);
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::VKShader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKShader> CreateShader(void* data, uint32_t size) const noexcept
    {
        return wis::ResultValue<wis::VKShader> { &wis::ImplVKDevice::CreateShader, this, data, size };
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::VKResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKResourceAllocator CreateAllocator(wis::Result& result) const noexcept
    {
        return wis::ImplVKDevice::CreateAllocator(result);
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::VKResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKResourceAllocator> CreateAllocator() const noexcept
    {
        return wis::ResultValue<wis::VKResourceAllocator> {
            &wis::ImplVKDevice::CreateAllocator,
            this,
        };
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::VKRenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKRenderTarget CreateRenderTarget(wis::Result& result, wis::VKTextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ImplVKDevice::CreateRenderTarget(result, std::move(texture), desc);
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::VKRenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRenderTarget> CreateRenderTarget(wis::VKTextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::VKRenderTarget> { &wis::ImplVKDevice::CreateRenderTarget, this, std::move(texture), desc };
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
    [[nodiscard]] inline wis::VKRenderTarget CreateDepthStencilTarget(wis::Result& result, wis::VKTextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ImplVKDevice::CreateDepthStencilTarget(result, std::move(texture), desc);
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
    [[nodiscard]] inline wis::ResultValue<wis::VKRenderTarget> CreateDepthStencilTarget(wis::VKTextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::VKRenderTarget> { &wis::ImplVKDevice::CreateDepthStencilTarget, this, std::move(texture), desc };
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::VKSampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKSampler CreateSampler(wis::Result& result, const wis::SamplerDesc& desc) const noexcept
    {
        return wis::ImplVKDevice::CreateSampler(result, desc);
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::VKSampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKSampler> CreateSampler(const wis::SamplerDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::VKSampler> { &wis::ImplVKDevice::CreateSampler, this, desc };
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::VKShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKShaderResource CreateShaderResource(wis::Result& result, wis::VKTextureView texture, const wis::ShaderResourceDesc& desc) const noexcept
    {
        return wis::ImplVKDevice::CreateShaderResource(result, std::move(texture), desc);
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::VKShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKShaderResource> CreateShaderResource(wis::VKTextureView texture, const wis::ShaderResourceDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::VKShaderResource> { &wis::ImplVKDevice::CreateShaderResource, this, std::move(texture), desc };
    }
    /**
     * @brief Creates a descriptor storage object with specified number of bindings to allocate.
     * Switching between several DescriptorStorage is slow, consider allocating one big set and copy descriptors to it.
     * @param bindings The bindings to allocate. Space and space overlap counts are ignored.
     * @param bindings_count The number of bindings to allocate.
     * @param memory The memory to allocate the descriptors in.
     * @return wis::VKDescriptorStorage on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKDescriptorStorage CreateDescriptorStorage(wis::Result& result, const wis::DescriptorBindingDesc* bindings, uint32_t bindings_count, wis::DescriptorMemory memory = wis::DescriptorMemory::ShaderVisible) const noexcept
    {
        return wis::ImplVKDevice::CreateDescriptorStorage(result, bindings, bindings_count, memory);
    }
    /**
     * @brief Creates a descriptor storage object with specified number of bindings to allocate.
     * Switching between several DescriptorStorage is slow, consider allocating one big set and copy descriptors to it.
     * @param bindings The bindings to allocate. Space and space overlap counts are ignored.
     * @param bindings_count The number of bindings to allocate.
     * @param memory The memory to allocate the descriptors in.
     * @return wis::VKDescriptorStorage on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKDescriptorStorage> CreateDescriptorStorage(const wis::DescriptorBindingDesc* bindings, uint32_t bindings_count, wis::DescriptorMemory memory = wis::DescriptorMemory::ShaderVisible) const noexcept
    {
        return wis::ResultValue<wis::VKDescriptorStorage> { &wis::ImplVKDevice::CreateDescriptorStorage, this, bindings, bindings_count, memory };
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

[[nodiscard]] WIS_INLINE wis::VKDevice
ImplVKCreateDevice(wis::Result& result, wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept;

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DEVICE_H
