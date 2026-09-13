// This file is generated. Do not edit directly.
#ifndef WISDOM_CORE_HPP
#define WISDOM_CORE_HPP

#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#ifndef FORCEVK_SWITCH
#    if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#        define FORCEVK_SWITCH 1
#    else
#        define FORCEVK_SWITCH 0
#    endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "generated/cpp_api.hpp"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

namespace wis {
static constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::DXIL;

//==============================================================
// Handles
//==============================================================

using Texture = wis::DX12Texture;
using Buffer = wis::DX12Buffer;
using Swapchain = wis::DX12Swapchain;
using Surface = wis::DX12Surface;
using ViewHeap = wis::DX12ViewHeap;
using Pipeline = wis::DX12Pipeline;
using Shader = wis::DX12Shader;
using PipelineCache = wis::DX12PipelineCache;
using DescriptorHeap = wis::DX12DescriptorHeap;
using RootSignature = wis::DX12RootSignature;
using ResourceAllocator = wis::DX12ResourceAllocator;
using Fence = wis::DX12Fence;
using CommandList = wis::DX12CommandList;
using CommandAllocator = wis::DX12CommandAllocator;
using CommandQueue = wis::DX12CommandQueue;
using Device = wis::DX12Device;
using AdapterQuery = wis::DX12AdapterQuery;
using Instance = wis::DX12Instance;
using TextureView = wis::DX12TextureView;
using BufferView = wis::DX12BufferView;
using SurfaceView = wis::DX12SurfaceView;
using PipelineView = wis::DX12PipelineView;
using ShaderView = wis::DX12ShaderView;
using PipelineCacheView = wis::DX12PipelineCacheView;
using RootSignatureView = wis::DX12RootSignatureView;
using FenceView = wis::DX12FenceView;
using CommandListView = wis::DX12CommandListView;

//==============================================================
// Variants
//==============================================================

using DeviceExtensionHeader = wis::DX12DeviceExtensionHeader;
using InstanceExtensionHeader = wis::DX12InstanceExtensionHeader;
using DeviceRequirements = wis::DX12DeviceRequirements;
using BufferBarrier = wis::DX12BufferBarrier;
using GlobalBarrier = wis::DX12GlobalBarrier;
using TextureBarrier = wis::DX12TextureBarrier;
using BarrierGroup = wis::DX12BarrierGroup;
using ComputePipelineDesc = wis::DX12ComputePipelineDesc;
using GraphicsPipelineDesc = wis::DX12GraphicsPipelineDesc;
using VertexBufferDesc = wis::DX12VertexBufferDesc;
using IndexBufferDesc = wis::DX12IndexBufferDesc;

//==============================================================
// Functions
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc describes a pointer to wis::DebugDesc, which defines debug callback and debug layer usage. If
 * `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to
 * wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::Instance CreateInstance(
    const wis::DebugDesc* debug_desc,
    wis::span<wis::InstanceExtensionHeader*> extensions,
    wis::Result& out_result
) noexcept
{
    wis::DX12Instance instance{};
    const WisResult wis_result = ::wisDX12CreateInstance(
        reinterpret_cast<const WisDebugDesc*>(debug_desc),
        reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
        extensions.size(),
        instance.GetStorage()
    );
    out_result = wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    return instance;
}

} // namespace wis

#elif defined(WISDOM_VULKAN)

namespace wis {
static constexpr wis::ShaderIntermediate shader_intermediate = wis::ShaderIntermediate::SPIRV;

//==============================================================
// Handles
//==============================================================

using Texture = wis::VKTexture;
using Buffer = wis::VKBuffer;
using Swapchain = wis::VKSwapchain;
using Surface = wis::VKSurface;
using ViewHeap = wis::VKViewHeap;
using Pipeline = wis::VKPipeline;
using Shader = wis::VKShader;
using PipelineCache = wis::VKPipelineCache;
using DescriptorHeap = wis::VKDescriptorHeap;
using RootSignature = wis::VKRootSignature;
using ResourceAllocator = wis::VKResourceAllocator;
using Fence = wis::VKFence;
using CommandList = wis::VKCommandList;
using CommandAllocator = wis::VKCommandAllocator;
using CommandQueue = wis::VKCommandQueue;
using Device = wis::VKDevice;
using AdapterQuery = wis::VKAdapterQuery;
using Instance = wis::VKInstance;
using TextureView = wis::VKTextureView;
using BufferView = wis::VKBufferView;
using SurfaceView = wis::VKSurfaceView;
using PipelineView = wis::VKPipelineView;
using ShaderView = wis::VKShaderView;
using PipelineCacheView = wis::VKPipelineCacheView;
using RootSignatureView = wis::VKRootSignatureView;
using FenceView = wis::VKFenceView;
using CommandListView = wis::VKCommandListView;

//==============================================================
// Variants
//==============================================================

using DeviceExtensionHeader = wis::VKDeviceExtensionHeader;
using InstanceExtensionHeader = wis::VKInstanceExtensionHeader;
using DeviceRequirements = wis::VKDeviceRequirements;
using BufferBarrier = wis::VKBufferBarrier;
using GlobalBarrier = wis::VKGlobalBarrier;
using TextureBarrier = wis::VKTextureBarrier;
using BarrierGroup = wis::VKBarrierGroup;
using ComputePipelineDesc = wis::VKComputePipelineDesc;
using GraphicsPipelineDesc = wis::VKGraphicsPipelineDesc;
using VertexBufferDesc = wis::VKVertexBufferDesc;
using IndexBufferDesc = wis::VKIndexBufferDesc;

//==============================================================
// Functions
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc describes a pointer to wis::DebugDesc, which defines debug callback and debug layer usage. If
 * `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to
 * wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::Instance CreateInstance(
    const wis::DebugDesc* debug_desc,
    wis::span<wis::InstanceExtensionHeader*> extensions,
    wis::Result& out_result
) noexcept
{
    wis::VKInstance instance{};
    const WisResult wis_result = ::wisVKCreateInstance(
        reinterpret_cast<const WisDebugDesc*>(debug_desc),
        reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
        extensions.size(),
        instance.GetStorage()
    );
    out_result = wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    return instance;
}

} // namespace wis
#else
#    error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_CORE_HPP
