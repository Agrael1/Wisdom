#pragma once
#include <array>
#include <wisdom/global/definitions.h>
#include <d3d12.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <unordered_map>
#include <string>

#if defined(WISDOM_VULKAN)
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <vk_mem_alloc.h>
#endif