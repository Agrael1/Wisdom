#ifndef WIS_DX12_DEVICE_H
#define WIS_DX12_DEVICE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_allocator.h>
#include <wisdom/dx12/dx12_descriptor_storage.h>
#include <wisdom/dx12/dx12_device_ext.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#endif

namespace wis {
class DX12Device;

WISDOM_EXPORT
template<>
struct Internal<DX12Device> {
    wis::com_ptr<ID3D12Device10> device;
    wis::com_ptr<IDXGIAdapter1> adapter;
    wis::com_ptr<IDXGIFactory4> factory;
};

class ImplDX12Device : public QueryInternal<DX12Device>
{
public:
    ImplDX12Device() noexcept = default;
    ImplDX12Device(ImplDX12Device&&) noexcept = default;
    ImplDX12Device& operator=(ImplDX12Device&&) noexcept = default;

    operator bool() const noexcept
    {
        return bool(device);
    }

public:
    [[nodiscard]] WIS_INLINE wis::Result
    WaitForMultipleFences(const DX12FenceView* fences,
                          const uint64_t* values,
                          uint32_t count,
                          MutiWaitFlags wait_all = MutiWaitFlags::All,
                          uint64_t timeout = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12Fence
    CreateFence(wis::Result& result, uint64_t initial_value = 0ull, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12CommandQueue
    CreateCommandQueue(wis::Result& result, wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12CommandList
    CreateCommandList(wis::Result& result, wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12PipelineState
    CreateGraphicsPipeline(wis::Result& result, const wis::DX12GraphicsPipelineDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12PipelineState
    CreateComputePipeline(wis::Result& result, const wis::DX12ComputePipelineDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12Shader
    CreateShader(wis::Result& result, void* data, size_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12ResourceAllocator
    CreateAllocator(wis::Result& result) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12RenderTarget
    CreateRenderTarget(wis::Result& result, DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12RenderTarget
    CreateDepthStencilTarget(wis::Result& result, DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12Sampler
    CreateSampler(wis::Result& result, const wis::SamplerDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12ShaderResource
    CreateShaderResource(wis::Result& result, DX12TextureView texture, const wis::ShaderResourceDesc& desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12UnorderedAccessTexture
    CreateUnorderedAccessTexture(wis::Result& result, DX12TextureView buffer, const wis::UnorderedAccessDesc& desc) const noexcept;

    // returns true only for now
    [[nodiscard]] WIS_INLINE bool
    QueryFeatureSupport(wis::DeviceFeature feature) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DeviceConstants
    QueryDeviceConsts() const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12DescriptorStorage
    CreateDescriptorStorage(wis::Result& result,
                            const wis::DescriptorBindingDesc* descriptor_bindings = nullptr,
                            uint32_t descriptor_bindings_count = 0,
                            wis::DescriptorMemory = wis::DescriptorMemory::ShaderVisible) const noexcept;

    [[nodiscard]] WIS_INLINE wis::DX12RootSignature
    CreateRootSignature(wis::Result& result,
                        const wis::PushConstant* push_constants = nullptr,
                        uint32_t constants_count = 0,
                        const wis::PushDescriptor* push_descriptors = nullptr,
                        uint32_t push_descriptors_count = 0,
                        const wis::DescriptorBindingDesc* descriptor_bindings = nullptr,
                        uint32_t descriptor_bindings_count = 0) const noexcept;
};

#pragma region DX12Device
/**
 * @brief Represents logical device.
 * Creates all the resources and commands for rendering.
 * */
WISDOM_EXPORT
class DX12Device : public wis::ImplDX12Device
{
public:
    using wis::ImplDX12Device::ImplDX12Device;

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
    [[nodiscard]] inline wis::Result WaitForMultipleFences(const wis::DX12FenceView* fences, const uint64_t* fence_values, uint32_t fence_count, wis::MutiWaitFlags wait_all = wis::MutiWaitFlags::All, uint64_t timeout = UINT64_MAX) const noexcept
    {
        return wis::ImplDX12Device::WaitForMultipleFences(fences, fence_values, fence_count, wait_all, timeout);
    }
    /**
     * @brief Creates a fence with initial value and flags.
     * @param initial_value The initial value of the fence.
     * @param flags The flags of the fence.
     * @return wis::DX12Fence on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12Fence CreateFence(wis::Result& result, uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ImplDX12Device::CreateFence(result, initial_value, flags);
    }
    /**
     * @brief Creates a fence with initial value and flags.
     * @param initial_value The initial value of the fence.
     * @param flags The flags of the fence.
     * @return wis::DX12Fence on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Fence> CreateFence(uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ResultValue<wis::DX12Fence>{ &wis::ImplDX12Device::CreateFence, this, initial_value, flags };
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::DX12CommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12CommandQueue CreateCommandQueue(wis::Result& result, wis::QueueType type) const noexcept
    {
        return wis::ImplDX12Device::CreateCommandQueue(result, type);
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::DX12CommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12CommandQueue> CreateCommandQueue(wis::QueueType type) const noexcept
    {
        return wis::ResultValue<wis::DX12CommandQueue>{ &wis::ImplDX12Device::CreateCommandQueue, this, type };
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::DX12CommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12CommandList CreateCommandList(wis::Result& result, wis::QueueType type) const noexcept
    {
        return wis::ImplDX12Device::CreateCommandList(result, type);
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::DX12CommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12CommandList> CreateCommandList(wis::QueueType type) const noexcept
    {
        return wis::ResultValue<wis::DX12CommandList>{ &wis::ImplDX12Device::CreateCommandList, this, type };
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::DX12PipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12PipelineState CreateGraphicsPipeline(wis::Result& result, const wis::DX12GraphicsPipelineDesc& desc) const noexcept
    {
        return wis::ImplDX12Device::CreateGraphicsPipeline(result, desc);
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::DX12PipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12PipelineState> CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::DX12PipelineState>{ &wis::ImplDX12Device::CreateGraphicsPipeline, this, desc };
    }
    /**
     * @brief Creates a compute pipeline state object.
     * @param desc The description of the compute pipeline to create.
     * @return wis::DX12PipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12PipelineState CreateComputePipeline(wis::Result& result, const wis::DX12ComputePipelineDesc& desc) const noexcept
    {
        return wis::ImplDX12Device::CreateComputePipeline(result, desc);
    }
    /**
     * @brief Creates a compute pipeline state object.
     * @param desc The description of the compute pipeline to create.
     * @return wis::DX12PipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12PipelineState> CreateComputePipeline(const wis::DX12ComputePipelineDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::DX12PipelineState>{ &wis::ImplDX12Device::CreateComputePipeline, this, desc };
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
     * @return wis::DX12RootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12RootSignature CreateRootSignature(wis::Result& result, const wis::PushConstant* push_constants = nullptr, uint32_t push_constant_count = 0, const wis::PushDescriptor* push_descriptors = nullptr, uint32_t push_descriptor_count = 0, const wis::DescriptorBindingDesc* bindings = nullptr, uint32_t binding_count = 0) const noexcept
    {
        return wis::ImplDX12Device::CreateRootSignature(result, push_constants, push_constant_count, push_descriptors, push_descriptor_count, bindings, binding_count);
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
     * @return wis::DX12RootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12RootSignature> CreateRootSignature(const wis::PushConstant* push_constants = nullptr, uint32_t push_constant_count = 0, const wis::PushDescriptor* push_descriptors = nullptr, uint32_t push_descriptor_count = 0, const wis::DescriptorBindingDesc* bindings = nullptr, uint32_t binding_count = 0) const noexcept
    {
        return wis::ResultValue<wis::DX12RootSignature>{ &wis::ImplDX12Device::CreateRootSignature, this, push_constants, push_constant_count, push_descriptors, push_descriptor_count, bindings, binding_count };
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::DX12Shader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12Shader CreateShader(wis::Result& result, void* data, uint32_t size) const noexcept
    {
        return wis::ImplDX12Device::CreateShader(result, data, size);
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::DX12Shader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Shader> CreateShader(void* data, uint32_t size) const noexcept
    {
        return wis::ResultValue<wis::DX12Shader>{ &wis::ImplDX12Device::CreateShader, this, data, size };
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::DX12ResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12ResourceAllocator CreateAllocator(wis::Result& result) const noexcept
    {
        return wis::ImplDX12Device::CreateAllocator(result);
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::DX12ResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12ResourceAllocator> CreateAllocator() const noexcept
    {
        return wis::ResultValue<wis::DX12ResourceAllocator>{
            &wis::ImplDX12Device::CreateAllocator,
            this,
        };
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::DX12RenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12RenderTarget CreateRenderTarget(wis::Result& result, wis::DX12TextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ImplDX12Device::CreateRenderTarget(result, std::move(texture), desc);
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::DX12RenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12RenderTarget> CreateRenderTarget(wis::DX12TextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::DX12RenderTarget>{ &wis::ImplDX12Device::CreateRenderTarget, this, std::move(texture), desc };
    }
    /**
     * @brief Creates a depth stencil target object.
     * Works only with depth formats.
     * Used with render passes.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * Does not work with 3D textures.
     * @return wis::DX12RenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12RenderTarget CreateDepthStencilTarget(wis::Result& result, wis::DX12TextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ImplDX12Device::CreateDepthStencilTarget(result, std::move(texture), desc);
    }
    /**
     * @brief Creates a depth stencil target object.
     * Works only with depth formats.
     * Used with render passes.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * Does not work with 3D textures.
     * @return wis::DX12RenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12RenderTarget> CreateDepthStencilTarget(wis::DX12TextureView texture, const wis::RenderTargetDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::DX12RenderTarget>{ &wis::ImplDX12Device::CreateDepthStencilTarget, this, std::move(texture), desc };
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::DX12Sampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12Sampler CreateSampler(wis::Result& result, const wis::SamplerDesc& desc) const noexcept
    {
        return wis::ImplDX12Device::CreateSampler(result, desc);
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::DX12Sampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Sampler> CreateSampler(const wis::SamplerDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::DX12Sampler>{ &wis::ImplDX12Device::CreateSampler, this, desc };
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::DX12ShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12ShaderResource CreateShaderResource(wis::Result& result, wis::DX12TextureView texture, const wis::ShaderResourceDesc& desc) const noexcept
    {
        return wis::ImplDX12Device::CreateShaderResource(result, std::move(texture), desc);
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::DX12ShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12ShaderResource> CreateShaderResource(wis::DX12TextureView texture, const wis::ShaderResourceDesc& desc) const noexcept
    {
        return wis::ResultValue<wis::DX12ShaderResource>{ &wis::ImplDX12Device::CreateShaderResource, this, std::move(texture), desc };
    }
    /**
     * @brief Creates a descriptor storage object with specified number of bindings to allocate.
     * Switching between several DescriptorStorage is slow, consider allocating one big set and copy descriptors to it.
     * @param bindings The bindings to allocate. Space and space overlap counts are ignored.
     * @param bindings_count The number of bindings to allocate.
     * @param memory The memory to allocate the descriptors in.
     * @return wis::DX12DescriptorStorage on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12DescriptorStorage CreateDescriptorStorage(wis::Result& result, const wis::DescriptorBindingDesc* bindings, uint32_t bindings_count, wis::DescriptorMemory memory = wis::DescriptorMemory::ShaderVisible) const noexcept
    {
        return wis::ImplDX12Device::CreateDescriptorStorage(result, bindings, bindings_count, memory);
    }
    /**
     * @brief Creates a descriptor storage object with specified number of bindings to allocate.
     * Switching between several DescriptorStorage is slow, consider allocating one big set and copy descriptors to it.
     * @param bindings The bindings to allocate. Space and space overlap counts are ignored.
     * @param bindings_count The number of bindings to allocate.
     * @param memory The memory to allocate the descriptors in.
     * @return wis::DX12DescriptorStorage on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12DescriptorStorage> CreateDescriptorStorage(const wis::DescriptorBindingDesc* bindings, uint32_t bindings_count, wis::DescriptorMemory memory = wis::DescriptorMemory::ShaderVisible) const noexcept
    {
        return wis::ResultValue<wis::DX12DescriptorStorage>{ &wis::ImplDX12Device::CreateDescriptorStorage, this, bindings, bindings_count, memory };
    }
    /**
     * @brief Queries if the device supports the feature.
     * @param feature The feature to query.
     * @return true if feature is supported. false otherwise.
     * */
    inline bool QueryFeatureSupport(wis::DeviceFeature feature) const noexcept
    {
        return wis::ImplDX12Device::QueryFeatureSupport(feature);
    }
    /**
     * @brief Queries the device constants.
     * @return The device constants.
     * */
    inline wis::DeviceConstants QueryDeviceConsts() const noexcept
    {
        return wis::ImplDX12Device::QueryDeviceConsts();
    }
};
#pragma endregion DX12Device

[[nodiscard]] WIS_INLINE wis::DX12Device
ImplDX12CreateDevice(wis::Result& result, wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions, uint32_t ext_count, bool force) noexcept;
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !DX12_DEVICE_H
