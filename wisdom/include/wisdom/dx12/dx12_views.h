#pragma once

struct IDXGIAdapter1;
struct ID3D12Device10;
struct ID3D12PipelineState;
struct ID3D12CommandQueue;
struct ID3D12Fence1;
struct ID3D12GraphicsCommandList9;
struct ID3D12Resource;
struct ID3D12RootSignature;

WIS_EXPORT namespace wis
{
	using DX12AdapterView = IDXGIAdapter1*;
	using DX12DeviceView = ID3D12Device10*;
	using DX12PipelineStateView = ID3D12PipelineState*;
	using DX12CommandQueueView = ID3D12CommandQueue*;
	using DX12FenceView = ID3D12Fence1*;
	using DX12CommandListView = ID3D12GraphicsCommandList9*;
	using DX12BufferView = ID3D12Resource*;
	using DX12TextureView = ID3D12Resource*;
	using DX12RootSignatureView = ID3D12RootSignature*;

}
