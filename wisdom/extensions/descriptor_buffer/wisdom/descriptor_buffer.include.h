#pragma once
#include <utility>

#if defined(WISDOM_VULKAN)
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <wisvk/vk_movable.hpp>
#include <wisdom/generated/vk_functions.hpp>
#include <wisvk/vk_managed_handles.hpp>
#endif

#if defined(WISDOM_DX12)
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12/d3dx12_root_signature.h>
#endif

#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <wisdom/global/definitions.h>
