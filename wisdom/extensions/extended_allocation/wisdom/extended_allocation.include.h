#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <wisdom/global/definitions.h>

#if defined(WISDOM_VULKAN)
#include <vulkan/vulkan.h>
#include <wisvk/vk_movable.hpp>
#endif 

#if defined(WISDOM_DX12)
#include <d3d12.h>
#include <D3D12MemAlloc.h>
#include <d3dx12/d3dx12_property_format_table.h>
#include <d3dx12/d3dx12_core.h>
#endif