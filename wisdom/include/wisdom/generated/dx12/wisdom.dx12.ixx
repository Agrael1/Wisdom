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
#include <wisdom/wisdom_dx12.hpp>
export module wisdom.dx12;

export wis::DX12CommandListView;
export wis::DX12GraphicsPipelineDesc;
export wis::DX12FenceView;
export wis::DX12PipelineView;
export wis::DX12RootSignatureView2;
export wis::DX12RenderPassDepthStencilDesc;
export wis::DX12BufferView;
export wis::DX12TextureView;
export wis::DX12ShaderView;
export wis::DX12RenderTargetView;
export wis::DX12RootSignatureView;
export wis::DX12TextureBarrier2;
export wis::DX12MemoryView;
export wis::DX12RaytracingPipeineDesc;
export wis::DX12SamplerView;
export wis::DX12ShaderResourceView;
export wis::DX12AcceleratedGeometryDesc;
export wis::DX12DescriptorStorageView;
export wis::DX12BottomLevelASBuildDesc;
export wis::DX12BufferBarrier2;
export wis::DX12GraphicsShaderStages;
export wis::DX12ComputePipelineDesc;
export wis::DX12RenderPassRenderTargetDesc;
export wis::DX12RenderPassDesc;
export wis::DX12VertexBufferBinding;

//-------------------------------------------------------------------------

export wis::DX12CommandQueue;
export wis::DX12Factory;
export wis::DX12DeviceExtension;
export wis::DX12PipelineState;
export wis::DX12Adapter;
export wis::DX12Device;
export wis::DX12FactoryExtension;
export wis::DX12ResourceAllocator;
export wis::DX12Memory;
export wis::DX12Fence;
export wis::DX12CommandList;
export wis::DX12SwapChain;
export wis::DX12Buffer;
export wis::DX12Texture;
export wis::DX12DescriptorStorage;
export wis::DX12RootSignature;
export wis::DX12Shader;
export wis::DX12DebugMessenger;
export wis::DX12RenderTarget;
export wis::DX12Sampler;
export wis::DX12ShaderResource;
export wis::DX12UnorderedAccessTexture;

//-------------------------------------------------------------------------

export namespace wis {
using wis::DX12CreateDevice;
using wis::DX12CreateFactory;
} // namespace wis

//-------------------------------------------------------------------------
