#pragma once
// Select default API
// Override with WISDOM_FORCE_VULKAN

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

#if defined(WISDOM_VULKAN_FOUND) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if WISDOMDX12 && !FORCEVK_SWITCH
#include "wisdom_dx12.h"

namespace wis{

using CommandQueue = DX12CommandQueue;
using RootSignature = DX12RootSignature;
using Factory = DX12Factory;
using PipelineState = DX12PipelineState;
using Adapter = DX12Adapter;
using Device = DX12Device;
using Fence = DX12Fence;
using ResourceAllocator = DX12ResourceAllocator;
using Shader = DX12Shader;
using RenderPass = DX12RenderPass;
constexpr auto CreateFactory = wis::DX12CreateFactory;
constexpr auto CreateDevice = wis::DX12CreateDevice;
}

#elif WISDOM_VULKAN_FOUND
#include "wisdom_vk.h"

namespace wis{

using CommandQueue = VKCommandQueue;
using RootSignature = VKRootSignature;
using Factory = VKFactory;
using PipelineState = VKPipelineState;
using Adapter = VKAdapter;
using Device = VKDevice;
using Fence = VKFence;
using ResourceAllocator = VKResourceAllocator;
using Shader = VKShader;
using RenderPass = VKRenderPass;
constexpr auto CreateFactory = wis::VKCreateFactory;
constexpr auto CreateDevice = wis::VKCreateDevice;
}

#else
#error "No API selected"
#endif
