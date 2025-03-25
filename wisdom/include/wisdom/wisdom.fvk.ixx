module;
// This module is forced to use Vulkan, regardless of the default API
#include <wisdom/config.h>
#include <wisdom/module.h>

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

export module wisdom.fvk;

export import wisdom.api;

#if defined(WISDOM_VULKAN)
import wisdom.vk;

export namespace wis {

inline constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::SPIRV;

using CommandQueue = VKCommandQueue;
using Factory = VKFactory;
using DeviceExtension = VKDeviceExtension;
using PipelineState = VKPipelineState;
using Adapter = VKAdapter;
using Device = VKDevice;
using FactoryExtension = VKFactoryExtension;
using ResourceAllocator = VKResourceAllocator;
using Memory = VKMemory;
using Fence = VKFence;
using CommandList = VKCommandList;
using SwapChain = VKSwapChain;
using Buffer = VKBuffer;
using Texture = VKTexture;
using DescriptorStorage = VKDescriptorStorage;
using RootSignature = VKRootSignature;
using Shader = VKShader;
using DebugMessenger = VKDebugMessenger;
using RenderTarget = VKRenderTarget;
using Sampler = VKSampler;
using ShaderResource = VKShaderResource;
using UnorderedAccessTexture = VKUnorderedAccessTexture;

//-------------------------------------------------------------------------

/**
 * @brief Creates the wis::Factory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::Factory on success (wis::Status::Ok).
 * */
inline wis::Factory CreateFactory(wis::Result& result, bool debug_layer = false, wis::FactoryExtension** extensions = nullptr, uint32_t extension_count = 0)
{
    return VKCreateFactory(result, debug_layer, extensions, extension_count);
}
/**
 * @brief Creates the wis::Factory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::Factory on success (wis::Status::Ok).
 * */
inline wis::ResultValue<wis::Factory> CreateFactory(bool debug_layer = false, wis::FactoryExtension** extensions = nullptr, uint32_t extension_count = 0)
{
    return VKCreateFactory(debug_layer, extensions, extension_count);
}
/**
 * @brief Creates the wis::Device with extensions, specified in extension array.
 * @param adapter The adapter to create the logical device on. Must not be NULL.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @param force Create logical device even if some core functionality is absent.
 * The presence of core functionality is checked by the query function.
 * @return wis::Device on success (wis::Status::Ok).
 * */
inline wis::Device CreateDevice(wis::Result& result, wis::Adapter adapter, wis::DeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false)
{
    return VKCreateDevice(result, std::move(adapter), extensions, extension_count, force);
}
/**
 * @brief Creates the wis::Device with extensions, specified in extension array.
 * @param adapter The adapter to create the logical device on. Must not be NULL.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @param force Create logical device even if some core functionality is absent.
 * The presence of core functionality is checked by the query function.
 * @return wis::Device on success (wis::Status::Ok).
 * */
inline wis::ResultValue<wis::Device> CreateDevice(wis::Adapter adapter, wis::DeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false)
{
    return VKCreateDevice(std::move(adapter), extensions, extension_count, force);
}

//-------------------------------------------------------------------------

using FenceView = VKFenceView;
using BufferView = VKBufferView;
using TextureView = VKTextureView;
using RenderTargetView = VKRenderTargetView;
using CommandListView = VKCommandListView;
using ShaderView = VKShaderView;
using RootSignatureView = VKRootSignatureView;
using RootSignatureView2 = VKRootSignatureView2;
using MemoryView = VKMemoryView;
using PipelineView = VKPipelineView;
using SamplerView = VKSamplerView;
using ShaderResourceView = VKShaderResourceView;
using AcceleratedGeometryDesc = VKAcceleratedGeometryDesc;
using DescriptorStorageView = VKDescriptorStorageView;
using BottomLevelASBuildDesc = VKBottomLevelASBuildDesc;
using BufferBarrier2 = VKBufferBarrier2;
using TextureBarrier2 = VKTextureBarrier2;
using GraphicsShaderStages = VKGraphicsShaderStages;
using RaytracingPipeineDesc = VKRaytracingPipeineDesc;
using GraphicsPipelineDesc = VKGraphicsPipelineDesc;
using ComputePipelineDesc = VKComputePipelineDesc;
using RenderPassRenderTargetDesc = VKRenderPassRenderTargetDesc;
using RenderPassDepthStencilDesc = VKRenderPassDepthStencilDesc;
using RenderPassDesc = VKRenderPassDesc;
using VertexBufferBinding = VKVertexBufferBinding;
} // namespace wis
#endif
