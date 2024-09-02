#pragma once
#include <wisdom/generated/api/api.h>
#include <array>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <tuple>

namespace D3D12MA {
class Allocator;
class Allocation;
} // namespace D3D12MA

namespace wis {
using DX12AdapterHandle = std::tuple<IDXGIAdapter1*>;
using DX12FactoryHandle = std::tuple<IDXGIFactory6*>;
using DX12PipelineHandle = std::tuple<ID3D12PipelineState*>;

using DX12FenceView = std::tuple<ID3D12Fence*>;
using DX12ShaderView = std::tuple<void*, uint32_t>;
using DX12RootSignatureView = std::tuple<ID3D12RootSignature*, std::array<int8_t, size_t(wis::ShaderStages::Count)>, uint32_t>;
using DX12CommandListView = std::tuple<ID3D12CommandList*>;
using DX12QueueView = std::tuple<ID3D12CommandQueue*>;
using DX12BufferView = std::tuple<ID3D12Resource*>;
using DX12TextureView = DX12BufferView;
using DX12RenderTargetView = std::tuple<D3D12_CPU_DESCRIPTOR_HANDLE>;
using DX12DescriptorBufferView = std::tuple<ID3D12DescriptorHeap*>;
using DX12DescriptorBufferGPUView = std::tuple<D3D12_GPU_DESCRIPTOR_HANDLE, uint32_t>;
using DX12SamplerView = std::tuple<D3D12_CPU_DESCRIPTOR_HANDLE>;
using DX12ShaderResourceView = std::tuple<D3D12_CPU_DESCRIPTOR_HANDLE>;
using DX12MemoryView = std::tuple<D3D12MA::Allocator*, D3D12MA::Allocation*>;
} // namespace wis