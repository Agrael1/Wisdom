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

    using DX12RenderTargetHandle = std::tuple<D3D12_CPU_DESCRIPTOR_HANDLE>;
    using DX12RenderPassView = std::tuple<const struct DX12RenderPassInternal*>;
    using DX12VertexBufferView = std::tuple<D3D12_VERTEX_BUFFER_VIEW>;
    using DX12CommandQueueView = std::tuple<ID3D12CommandQueue*>;
    using DX12PipelineStateView = std::tuple<ID3D12PipelineState*>;
    using DX12FenceView = std::tuple<ID3D12Fence1*>;
    using DX12CommandListView = std::tuple<ID3D12GraphicsCommandList9*>;

    using DX12BufferView = std::tuple<ID3D12Resource*>;
    using DX12TextureView = DX12BufferView;

    using DX12RootSignatureView = std::tuple<ID3D12RootSignature*>;
    using DX12DescriptorSetView = std::tuple<CD3DX12_CPU_DESCRIPTOR_HANDLE, CD3DX12_CPU_DESCRIPTOR_HANDLE, uint32_t>;
    using DX12DescriptorSetBindView = std::tuple<ID3D12DescriptorHeap*, D3D12_GPU_DESCRIPTOR_HANDLE>;
}
