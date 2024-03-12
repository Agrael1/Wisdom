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

#if WISDOM_VULKAN && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if WISDOM_DX12 && !FORCEVK_SWITCH
#include "wisdom_dx12.h"

namespace wis{

inline constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::DXIL;using CommandQueue = DX12CommandQueue;
using RootSignature = DX12RootSignature;
using Factory = DX12Factory;
using PipelineState = DX12PipelineState;
using Adapter = DX12Adapter;
using Device = DX12Device;
using UploadBuffer = DX12UploadBuffer;
using Fence = DX12Fence;
using ResourceAllocator = DX12ResourceAllocator;
using Shader = DX12Shader;
using CommandList = DX12CommandList;
using SwapChain = DX12SwapChain;
using Buffer = DX12Buffer;
using DebugMessenger = DX12DebugMessenger;
using RenderTarget = DX12RenderTarget;
constexpr auto CreateFactory = wis::DX12CreateFactory;
constexpr auto CreateDevice = wis::DX12CreateDevice;
using FenceView = DX12FenceView;
using BufferView = DX12BufferView;
using TextureView = DX12TextureView;
using RenderTargetView = DX12RenderTargetView;
using CommandListView = DX12CommandListView;
using ShaderView = DX12ShaderView;
using RootSignatureView = DX12RootSignatureView;
using BufferBarrier2 = DX12BufferBarrier2;
using TextureBarrier2 = DX12TextureBarrier2;
using GraphicsShaderStages = DX12GraphicsShaderStages;
using GraphicsPipelineDesc = DX12GraphicsPipelineDesc;
}

#elif WISDOM_VULKAN
#include "wisdom_vk.h"

namespace wis{

inline constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::SPIRV;using CommandQueue = VKCommandQueue;
using RootSignature = VKRootSignature;
using Factory = VKFactory;
using PipelineState = VKPipelineState;
using Adapter = VKAdapter;
using Device = VKDevice;
using UploadBuffer = VKUploadBuffer;
using Fence = VKFence;
using ResourceAllocator = VKResourceAllocator;
using Shader = VKShader;
using CommandList = VKCommandList;
using SwapChain = VKSwapChain;
using Buffer = VKBuffer;
using DebugMessenger = VKDebugMessenger;
using RenderTarget = VKRenderTarget;
constexpr auto CreateFactory = wis::VKCreateFactory;
constexpr auto CreateDevice = wis::VKCreateDevice;
using FenceView = VKFenceView;
using BufferView = VKBufferView;
using TextureView = VKTextureView;
using RenderTargetView = VKRenderTargetView;
using CommandListView = VKCommandListView;
using ShaderView = VKShaderView;
using RootSignatureView = VKRootSignatureView;
using BufferBarrier2 = VKBufferBarrier2;
using TextureBarrier2 = VKTextureBarrier2;
using GraphicsShaderStages = VKGraphicsShaderStages;
using GraphicsPipelineDesc = VKGraphicsPipelineDesc;
}

#else
#error "No API selected"
#endif
