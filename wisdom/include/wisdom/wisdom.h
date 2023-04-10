#pragma once

// Select default API
#if WISDOMDX12
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_allocator.h>

//dx12 
namespace wis
{
	using Factory = class DX12Factory;
	using Adapter = class DX12Adapter;
	using Device = class DX12Device;
	using CommandQueue = class DX12CommandQueue;
	using SwapChain = class DX12SwapChain;
	using CommandList = class DX12CommandList;
	using Fence = class DX12Fence;
	using Resource = class DX12Resource;
	using RenderTargetView = DX12RenderTargetView;
	using Shader = DX12Shader;
	using RootSignature = DX12RootSignature;
	using PipelineState = DX12PipelineState;
	using ResourceAllocator = DX12ResourceAllocator;
	using GraphicsPipelineDesc = DX12GraphicsPipelineDesc;
	using VertexBufferView = DX12VertexBufferView;

	using GraphicsInfo = class DX12Info;

	using TransitionBarrier = BasicTransitionBarrier<Resource>;
}
#elif WISDOMMTL  //MAC
//metal
namespace wis
{
}
#else
#include <wisdom/vulkan/vk_factory.h>

//vulkan
namespace wis
{
	using Factory = VKFactory;
}
#endif