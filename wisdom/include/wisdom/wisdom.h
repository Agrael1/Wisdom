#pragma once

// Select default API
#if WISDOMDX12
#include <wisdom/dx12/dx12_factory.h>

//dx12 
namespace wis
{
	using Factory = DX12Factory;
	using Adapter = DX12Adapter;
	//using Device = DX12Device;
	//using CommandQueue = DX12CommandQueue;
	//using SwapChain = DX12SwapChain;
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