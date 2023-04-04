#pragma once

// Select default API
#if WISDOMDX12
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_info.h>

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

	using GraphicsInfo = class DX12Info;

	using TransitionBarrier = BasicTransitionBarrier<Resource>;
}
#elif WISDOMMTL  //MAC
//metal
namespace wis
{
}
#else
//vulkan
namespace wis
{
}
#endif