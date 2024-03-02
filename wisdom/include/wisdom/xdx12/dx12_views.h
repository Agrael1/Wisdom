#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <tuple>

namespace wis {
using DX12AdapterHandle = std::tuple<IDXGIAdapter1*>;
using DX12FactoryHandle = std::tuple<IDXGIFactory6*>;
using DX12PipelineHandle = std::tuple<ID3D12PipelineState*>;

using DX12FenceView = std::tuple<ID3D12Fence*>;
using DX12ShaderView = std::tuple<void*, uint32_t>;
using DX12RootSignatureView = std::tuple<ID3D12RootSignature*>;
using DX12CommandListView = std::tuple<ID3D12CommandList*>;
using DX12QueueView = std::tuple<ID3D12CommandQueue*>;
using DX12BufferView = std::tuple<ID3D12Resource*>;
} // namespace wis