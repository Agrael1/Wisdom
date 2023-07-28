#pragma once

// Select default API
// Override with WISDOM_FORCE_VULKAN

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP&& _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS&& _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX&& __linux__, "Platform error");
#endif // WISDOM_LINUX

#if defined(WISDOM_VULKAN_FOUND) && defined(WISDOM_FORCE_VULKAN) 
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND



#if WISDOMDX12 && !FORCEVK_SWITCH

#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_allocator.h>
#include <wisdom/dx12/dx12_device.h>
#endif // WISDOMDX12 && !FORCEVK_SWITCH

//dx12
WIS_EXPORT namespace wis
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
	using DescriptorHeap = DX12DescriptorHeap;

	using DescriptorSet = DX12DescriptorSet;
	using DescriptorLayout = DX12DescriptorLayout;
}
#elif WISDOMMTL && !FORCEVK_SWITCH //MAC
//metal
WIS_EXPORT namespace wis
{
}
#elif WISDOM_VULKAN_FOUND
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_state_builder.h>
#endif // !WISDOM_MODULES

//vulkan
WIS_EXPORT namespace wis
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
	using DescriptorHeap = VKDescriptorHeap;

	using DescriptorSet = VKDescriptorSet;
	using DescriptorLayout = VKDescriptorLayout;
}
#else
#error "No API selected"
#endif