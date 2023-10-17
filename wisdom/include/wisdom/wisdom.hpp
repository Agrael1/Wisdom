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

using Factory = DX12Factory;
using Adapter = DX12Adapter;
using Device = DX12Device;
using Fence = DX12Fence;
using ResourceAllocator = DX12ResourceAllocator;
constexpr auto CreateFactory = DX12CreateFactory;
constexpr auto CreateDevice = DX12CreateDevice;
}

#elif WISDOM_VULKAN_FOUND
#include "wisdom_vk.h"

namespace wis{

using Factory = VKFactory;
using Adapter = VKAdapter;
using Device = VKDevice;
using Fence = VKFence;
using ResourceAllocator = VKResourceAllocator;
constexpr auto CreateFactory = VKCreateFactory;
constexpr auto CreateDevice = VKCreateDevice;
}

#else
#error "No API selected"
#endif
