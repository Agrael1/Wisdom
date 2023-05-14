#pragma once

// Select default API
// Override with WFORCEVK

#if WISDOMDX12 && !defined(WFORCEVK)
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
	using Resource = DX12Buffer;
	using RenderTargetView = DX12RenderTargetView;
	using Shader = DX12Shader;
	using RootSignature = DX12RootSignature;
	using PipelineState = DX12PipelineState;
	using ResourceAllocator = DX12ResourceAllocator;
	using GraphicsPipelineDesc = DX12GraphicsPipelineDesc;
	using VertexBufferView = DX12VertexBufferView;
	using RenderPass = DX12RenderPass;
}
#elif WISDOMMTL  //MAC
//metal
namespace wis
{
}
#else
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_state_builder.h>

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
	using RenderTargetView = VKRenderTargetView;
	using Shader = VKShader;
	using RootSignature = VKRootSignature;
	using PipelineState = VKPipelineState;
	using ResourceAllocator = VKResourceAllocator;
	using GraphicsPipelineDesc = VKGraphicsPipelineDesc;
	using VertexBufferView = VKVertexBufferView;
	using RenderPass = VKRenderPass;
}
#endif