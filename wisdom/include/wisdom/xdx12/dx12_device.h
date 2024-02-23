#pragma once
#include <wisdom/xdx12/dx12_fence.h>
#include <wisdom/xdx12/dx12_command_queue.h>
#include <wisdom/xdx12/dx12_command_list.h>
#include <wisdom/xdx12/dx12_pipeline_state.h>
#include <wisdom/xdx12/dx12_root_signature.h>
#include <wisdom/xdx12/dx12_shader.h>
#include <wisdom/xdx12/dx12_allocator.h>
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
    operator bool() const noexcept { return bool(device); }

public:
    [[nodicard]] WIS_INLINE wis::Result
    WaitForMultipleFences(const DX12FenceView* fences,
                          const uint64_t* values,
                          uint32_t count,
                          MutiWaitFlags wait_all = MutiWaitFlags::All,
                          uint64_t timeout = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Fence>
    CreateFence(uint64_t initial_value = 0ull) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12CommandQueue>
    CreateCommandQueue(wis::QueueType type, wis::QueuePriority priority = wis::QueuePriority::Common) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12CommandList>
    CreateCommandList(wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12PipelineState>
    CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12RootSignature>
    CreateRootSignature(RootConstant* root_constants = nullptr, uint32_t constants_size = 0) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Shader>
    CreateShader(void* data, size_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12ResourceAllocator>
    CreateAllocator() const noexcept;
};

WIS_INLINE [[nodiscard]] wis::ResultValue<wis::DX12Device>
DX12CreateDevice(wis::DX12FactoryHandle factory, wis::DX12AdapterHandle adapter) noexcept;
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/dx12_device.cpp"
#endif // !WISDOM_HEADER_ONLY
