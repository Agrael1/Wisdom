#pragma once
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_allocator.h>
#include <wisdom/dx12/dx12_descriptor_buffer.h>
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

class DX12Device : public QueryInternal<DX12Device>
{
public:
    DX12Device() noexcept = default;
    explicit DX12Device(wis::com_ptr<ID3D12Device10> device, wis::com_ptr<IDXGIAdapter1> adapter, wis::com_ptr<IDXGIFactory4> factory) noexcept
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
    CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint32_t descriptor_count) const noexcept;

    // returns true only for now
    [[nodiscard]] WIS_INLINE bool
    QueryFeatureSupport(wis::DeviceFeature feature) const noexcept;
};

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Device>
DX12CreateDevice(wis::DX12AdapterHandle adapter) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Device>
DX12CreateDeviceWithExtensions(wis::DX12AdapterHandle adapter, wis::DX12DeviceExtension** extensions, uint32_t ext_count) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Device>
DX12CreateDeviceWithExtensionsForce(wis::DX12AdapterHandle adapter, wis::DX12DeviceExtension** extensions, uint32_t ext_count, bool force) noexcept;
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_device.cpp"
#endif // !WISDOM_HEADER_ONLY
