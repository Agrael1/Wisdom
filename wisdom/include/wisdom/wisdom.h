#pragma once

// Select default API
#if WISDOMDX12
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_allocator.h>

//dx12 
namespace wis
{
	using Factory = DX12Factory;
	using Adapter = DX12Adapter;
	using Device = DX12Device;
	using CommandQueue = DX12CommandQueue;
	using SwapChain = DX12SwapChain;
	using CommandList = DX12CommandList;
	using Fence = DX12Fence;
	using Resource = DX12Resource;
	using RenderTargetView = DX12RenderTargetView;
	using Shader = DX12Shader;
	using RootSignature = DX12RootSignature;
	using PipelineState = DX12PipelineState;
	using ResourceAllocator = DX12ResourceAllocator;
	using GraphicsPipelineDesc = DX12GraphicsPipelineDesc;
	using VertexBufferView = DX12VertexBufferView;

	using TransitionBarrier = BasicTransitionBarrier<Resource>;
}
#elif WISDOMMTL  //MAC
//metal
namespace wis
{
}
#else
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_allocator.h>

//vulkan
namespace wis
{
	using Factory = VKFactory;
	using Adapter = VKAdapter;
	using Device = VKDevice;
	using CommandQueue = VKCommandQueue;
	using SwapChain = VKSwapChain;
	using CommandList = VKCommandList;
	using Fence = VKFence;
	using Resource = VKBuffer;
	using ResourceAllocator = VKResourceAllocator;
}
#endif