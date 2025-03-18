/** \mainpage Wisdom API Documentation

<b>Version 0.6.6</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

module;
#include <wisdom/global/internal.h>
#define WISDOM_SILENCE_API_HEADERS
import wisdom.api;
import wisdom.internal;
#include <wisdom/wisdom_vk.hpp>
export module wisdom.vk;

export wis::VKCommandListView;
export wis::VKGraphicsPipelineDesc;
export wis::VKFenceView;
export wis::VKPipelineView;
export wis::VKRootSignatureView2;
export wis::VKRenderPassDepthStencilDesc;
export wis::VKBufferView;
export wis::VKTextureView;
export wis::VKShaderView;
export wis::VKRenderTargetView;
export wis::VKRootSignatureView;
export wis::VKTextureBarrier2;
export wis::VKMemoryView;
export wis::VKRaytracingPipeineDesc;
export wis::VKSamplerView;
export wis::VKShaderResourceView;
export wis::VKAcceleratedGeometryDesc;
export wis::VKDescriptorStorageView;
export wis::VKBottomLevelASBuildDesc;
export wis::VKBufferBarrier2;
export wis::VKGraphicsShaderStages;
export wis::VKComputePipelineDesc;
export wis::VKRenderPassRenderTargetDesc;
export wis::VKRenderPassDesc;
export wis::VKVertexBufferBinding;

//-------------------------------------------------------------------------

export wis::VKCommandQueue;
export wis::VKFactory;
export wis::VKDeviceExtension;
export wis::VKPipelineState;
export wis::VKAdapter;
export wis::VKDevice;
export wis::VKFactoryExtension;
export wis::VKResourceAllocator;
export wis::VKMemory;
export wis::VKFence;
export wis::VKCommandList;
export wis::VKSwapChain;
export wis::VKBuffer;
export wis::VKTexture;
export wis::VKDescriptorStorage;
export wis::VKRootSignature;
export wis::VKShader;
export wis::VKDebugMessenger;
export wis::VKRenderTarget;
export wis::VKSampler;
export wis::VKShaderResource;
export wis::VKUnorderedAccessTexture;

//-------------------------------------------------------------------------

export namespace wis {
using wis::VKCreateDevice;
using wis::VKCreateFactory;
} // namespace wis

//-------------------------------------------------------------------------
