#pragma once
#ifndef WISDOM_MODULES
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/global/definitions.h>
#include <tuple>
#include <span>
#endif

struct IDXGIAdapter1;
struct IDXGIFactory4;
// struct ID3D12Device10;
// struct ID3D12PipelineState;
// struct ID3D12CommandQueue;
// struct ID3D12Fence1;
// struct ID3D12DescriptorHeap;
// struct ID3D12GraphicsCommandList9;
// struct ID3D12Resource;
// struct ID3D12RootSignature;

WIS_EXPORT namespace wis
{
using DX12AdapterView = std::tuple<IDXGIAdapter1*>;
using DX12FactoryView = std::tuple<IDXGIFactory4*>;

using DX12RenderPassView = std::tuple<const struct DX12RenderPassInternal*>;

using DX12DeviceView = ID3D12Device10*;
using DX12PipelineStateView = ID3D12PipelineState*;
using DX12CommandQueueView = ID3D12CommandQueue*;
using DX12FenceView = ID3D12Fence1*;
using DX12CommandListView = ID3D12GraphicsCommandList9*;
using DX12BufferView = ID3D12Resource*;
using DX12TextureView = ID3D12Resource*;
using DX12RootSignatureView = ID3D12RootSignature*;

using DX12DescriptorSetView = std::tuple<CD3DX12_CPU_DESCRIPTOR_HANDLE, CD3DX12_CPU_DESCRIPTOR_HANDLE, uint32_t>;
using DX12DescriptorSetBindView = std::tuple<ID3D12DescriptorHeap*, D3D12_GPU_DESCRIPTOR_HANDLE>;
}
