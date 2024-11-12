#ifndef WIS_DX12_DEVICE_H
#define WIS_DX12_DEVICE_H
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_allocator.h>
#include <wisdom/dx12/dx12_descriptor_buffer.h>
#include <wisdom/dx12/dx12_descriptor_storage.h>
#include <wisdom/dx12/dx12_device_ext.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>

namespace wis {
class DX12Device;

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
    explicit ImplDX12Device(wis::com_ptr<ID3D12Device10> device, wis::com_ptr<IDXGIAdapter1> adapter, wis::com_ptr<IDXGIFactory4> factory) noexcept
        : QueryInternal(std::move(device), std::move(adapter), std::move(factory))
    {
    }
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

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Fence>
    CreateFence(uint64_t initial_value = 0ull, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12CommandQueue>
    CreateCommandQueue(wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12CommandList>
    CreateCommandList(wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12PipelineState>
    CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12RootSignature>
    CreateRootSignature(const RootConstant* root_constants = nullptr,
                        uint32_t constants_size = 0,
                        const wis::DescriptorTable* tables = nullptr,
                        uint32_t tables_count = 0) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Shader>
    CreateShader(void* data, size_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12ResourceAllocator>
    CreateAllocator() const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12RenderTarget>
    CreateRenderTarget(DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12RenderTarget>
    CreateDepthStencilTarget(DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Sampler>
    CreateSampler(const wis::SamplerDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12ShaderResource>
    CreateShaderResource(DX12TextureView texture, wis::ShaderResourceDesc desc) const noexcept;

    // Descriptor buffer
    [[nodiscard]] uint32_t
    GetDescriptorTableAlignment(wis::DescriptorHeapType heap) const noexcept
    {
        return device->GetDescriptorHandleIncrementSize(wis::convert_dx(heap));
    }
    [[nodiscard]] uint32_t
    GetDescriptorBufferUnitSize(wis::DescriptorHeapType heap) const noexcept
    {
        return device->GetDescriptorHandleIncrementSize(wis::convert_dx(heap));
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12DescriptorBuffer>
    CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t memory_bytes) const noexcept;

    // returns true only for now
    [[nodiscard]] WIS_INLINE bool
    QueryFeatureSupport(wis::DeviceFeature feature) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12DescriptorStorage>
    CreateDescriptorStorage(wis::DescriptorStorageDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12RootSignature>
    CreateDescriptorStorageRootSignature(const RootConstant* constants = nullptr,
                                         uint32_t constants_size = 0) const noexcept;
};

#pragma region DX12Device
/**
 * @brief Represents logical device.
 * Creates all the resources and commands for rendering.
 * */
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
    [[nodiscard]] inline wis::ResultValue<wis::DX12Fence> CreateFence(uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ImplDX12Device::CreateFence(initial_value, flags);
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::DX12CommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12CommandQueue> CreateCommandQueue(wis::QueueType type) const noexcept
    {
        return wis::ImplDX12Device::CreateCommandQueue(type);
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::DX12CommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12CommandList> CreateCommandList(wis::QueueType type) const noexcept
    {
        return wis::ImplDX12Device::CreateCommandList(type);
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::DX12PipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12PipelineState> CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc* desc) const noexcept
    {
        return wis::ImplDX12Device::CreateGraphicsPipeline(desc);
    }
    /**
     * @brief Creates a root signature object.
     * @param root_constants The root constants to create the root signature with.
     * @param constants_size The number of root constants.
     * @param tables The descriptor tables to create the root signature with.
     * @param tables_count The number of descriptor tables.
     * @return wis::DX12RootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12RootSignature> CreateRootSignature(const wis::RootConstant* root_constants = nullptr, uint32_t constants_size = 0, const wis::DescriptorTable* tables = nullptr, uint32_t tables_count = 0) const noexcept
    {
        return wis::ImplDX12Device::CreateRootSignature(root_constants, constants_size, tables, tables_count);
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::DX12Shader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Shader> CreateShader(void* data, uint32_t size) const noexcept
    {
        return wis::ImplDX12Device::CreateShader(data, size);
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::DX12ResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12ResourceAllocator> CreateAllocator() const noexcept
    {
        return wis::ImplDX12Device::CreateAllocator();
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::DX12RenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12RenderTarget> CreateRenderTarget(wis::DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return wis::ImplDX12Device::CreateRenderTarget(std::move(texture), desc);
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
    [[nodiscard]] inline wis::ResultValue<wis::DX12RenderTarget> CreateDepthStencilTarget(wis::DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return wis::ImplDX12Device::CreateDepthStencilTarget(std::move(texture), desc);
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::DX12Sampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Sampler> CreateSampler(const wis::SamplerDesc* desc) const noexcept
    {
        return wis::ImplDX12Device::CreateSampler(desc);
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::DX12ShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12ShaderResource> CreateShaderResource(wis::DX12TextureView texture, wis::ShaderResourceDesc desc) const noexcept
    {
        return wis::ImplDX12Device::CreateShaderResource(std::move(texture), desc);
    }
    /**
     * @brief Returns the alignment of the descriptor table in bytes.
     * The value is used to correctly determine descriptor page alignment for descriptor buffer.
     * @param heap The type of the descriptor heap to get the alignment for.
     * @return The alignment of the descriptor table in bytes.
     * */
    inline uint32_t GetDescriptorTableAlignment(wis::DescriptorHeapType heap) const noexcept
    {
        return wis::ImplDX12Device::GetDescriptorTableAlignment(heap);
    }
    /**
     * @brief Returns the size of the descriptor buffer unit in bytes.
     * @param heap The type of the descriptor heap to get the unit size for.
     * @return The size of the descriptor buffer unit in bytes. Descriptor unit is the size of one descriptor.
     * */
    inline uint32_t GetDescriptorBufferUnitSize(wis::DescriptorHeapType heap) const noexcept
    {
        return wis::ImplDX12Device::GetDescriptorBufferUnitSize(heap);
    }
    /**
     * @brief Creates a descriptor buffer object.
     * @param heap_type The type of the descriptor heap to create the descriptor buffer with.
     * @param memory_type The type of the descriptor memory to create the descriptor buffer with.
     * @param size_bytes The number of bytes to allocate for the descriptor buffer.
     * @return wis::DX12DescriptorBuffer on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12DescriptorBuffer> CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t size_bytes) const noexcept
    {
        return wis::ImplDX12Device::CreateDescriptorBuffer(heap_type, memory_type, size_bytes);
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
};
#pragma endregion DX12Device

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Device>
ImplDX12CreateDevice(wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions, uint32_t ext_count, bool force) noexcept;
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !DX12_DEVICE_H
