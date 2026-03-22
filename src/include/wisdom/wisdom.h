// This file is generated. Do not edit directly.
#ifndef WISDOM_CORE_H
#define WISDOM_CORE_H

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifndef FORCEVK_SWITCH
#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "generated/c_api.h"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

#define WIS_SHADER_INTERMEDIATE_DXIL 1

//==============================================================
// Handles
//==============================================================

typedef struct WisDX12Surface           WisSurface;
typedef struct WisDX12ViewHeap          WisViewHeap;
typedef struct WisDX12Pipeline          WisPipeline;
typedef struct WisDX12Shader            WisShader;
typedef struct WisDX12PipelineCache     WisPipelineCache;
typedef struct WisDX12Texture           WisTexture;
typedef struct WisDX12Buffer            WisBuffer;
typedef struct WisDX12DescriptorHeap    WisDescriptorHeap;
typedef struct WisDX12RootSignature     WisRootSignature;
typedef struct WisDX12ResourceAllocator WisResourceAllocator;
typedef struct WisDX12Fence             WisFence;
typedef struct WisDX12CommandList       WisCommandList;
typedef struct WisDX12CommandAllocator  WisCommandAllocator;
typedef struct WisDX12CommandQueue      WisCommandQueue;
typedef struct WisDX12Device            WisDevice;
typedef struct WisDX12AdapterQuery      WisAdapterQuery;
typedef struct WisDX12Instance          WisInstance;
typedef struct WisDX12SurfaceView       WisSurfaceView;
typedef struct WisDX12PipelineView      WisPipelineView;
typedef struct WisDX12ShaderView        WisShaderView;
typedef struct WisDX12PipelineCacheView WisPipelineCacheView;
typedef struct WisDX12TextureView       WisTextureView;
typedef struct WisDX12BufferView        WisBufferView;
typedef struct WisDX12RootSignatureView WisRootSignatureView;
typedef struct WisDX12FenceView         WisFenceView;
typedef struct WisDX12CommandListView   WisCommandListView;

//==============================================================
// Variants
//==============================================================

typedef struct WisDX12DeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisDX12InstanceExtensionHeader WisInstanceExtensionHeader;
typedef struct WisDX12DeviceRequirements      WisDeviceRequirements;
typedef struct WisDX12BufferBarrier           WisBufferBarrier;
typedef struct WisDX12GlobalBarrier           WisGlobalBarrier;
typedef struct WisDX12TextureBarrier          WisTextureBarrier;
typedef struct WisDX12BarrierGroup            WisBarrierGroup;
typedef struct WisDX12ComputePipelineDesc     WisComputePipelineDesc;
typedef struct WisDX12GraphicsPipelineDesc    WisGraphicsPipelineDesc;

//==============================================================
// Functions
//==============================================================

#define wisGetSurfaceView                           wisGetDX12SurfaceView
#define wisGetPipelineView                          wisGetDX12PipelineView
#define wisGetShaderView                            wisGetDX12ShaderView
#define wisGetPipelineCacheView                     wisGetDX12PipelineCacheView
#define wisGetTextureView                           wisGetDX12TextureView
#define wisGetBufferView                            wisGetDX12BufferView
#define wisGetRootSignatureView                     wisGetDX12RootSignatureView
#define wisGetFenceView                             wisGetDX12FenceView
#define wisGetCommandListView                       wisGetDX12CommandListView
#define wisDestroySurface                           wisDX12DestroySurface
#define wisDestroyViewHeap                          wisDX12DestroyViewHeap
#define wisDestroyPipeline                          wisDX12DestroyPipeline
#define wisDestroyShader                            wisDX12DestroyShader
#define wisDestroyPipelineCache                     wisDX12DestroyPipelineCache
#define wisDestroyTexture                           wisDX12DestroyTexture
#define wisDestroyBuffer                            wisDX12DestroyBuffer
#define wisDestroyDescriptorHeap                    wisDX12DestroyDescriptorHeap
#define wisDestroyRootSignature                     wisDX12DestroyRootSignature
#define wisDestroyResourceAllocator                 wisDX12DestroyResourceAllocator
#define wisDestroyFence                             wisDX12DestroyFence
#define wisDestroyCommandList                       wisDX12DestroyCommandList
#define wisDestroyCommandAllocator                  wisDX12DestroyCommandAllocator
#define wisDestroyCommandQueue                      wisDX12DestroyCommandQueue
#define wisDestroyDevice                            wisDX12DestroyDevice
#define wisDestroyAdapterQuery                      wisDX12DestroyAdapterQuery
#define wisDestroyInstance                          wisDX12DestroyInstance
#define wisCreateInstance                           wisDX12CreateInstance
#define wisInstanceQueryAdapters                    wisDX12InstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount              wisDX12AdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc               wisDX12AdapterQueryGetAdapterDesc
#define wisAdapterQueryGetSurfaceSupport            wisDX12AdapterQueryGetSurfaceSupport
#define wisAdapterQueryCreateDevice                 wisDX12AdapterQueryCreateDevice
#define wisDeviceCreateCommandQueue                 wisDX12DeviceCreateCommandQueue
#define wisDeviceCreateCommandAllocator             wisDX12DeviceCreateCommandAllocator
#define wisDeviceCreateFence                        wisDX12DeviceCreateFence
#define wisDeviceGetResourceAllocator               wisDX12DeviceGetResourceAllocator
#define wisDeviceCreateRootSignature                wisDX12DeviceCreateRootSignature
#define wisDeviceCreateDescriptorHeap               wisDX12DeviceCreateDescriptorHeap
#define wisDeviceCreateViewHeap                     wisDX12DeviceCreateViewHeap
#define wisDeviceQueryProperties                    wisDX12DeviceQueryProperties
#define wisDeviceWaitForMultipleFences              wisDX12DeviceWaitForMultipleFences
#define wisDeviceCreatePipelineCache                wisDX12DeviceCreatePipelineCache
#define wisDeviceCreateShader                       wisDX12DeviceCreateShader
#define wisDeviceCreateComputePipeline              wisDX12DeviceCreateComputePipeline
#define wisDeviceCreateGraphicsPipeline             wisDX12DeviceCreateGraphicsPipeline
#define wisFenceGetCompletedValue                   wisDX12FenceGetCompletedValue
#define wisFenceWait                                wisDX12FenceWait
#define wisFenceSignal                              wisDX12FenceSignal
#define wisCommandQueueSubmit                       wisDX12CommandQueueSubmit
#define wisCommandQueueSignalFence                  wisDX12CommandQueueSignalFence
#define wisCommandQueueWaitFence                    wisDX12CommandQueueWaitFence
#define wisResourceAllocatorCreateBuffer            wisDX12ResourceAllocatorCreateBuffer
#define wisResourceAllocatorCreateTexture           wisDX12ResourceAllocatorCreateTexture
#define wisBufferMap                                wisDX12BufferMap
#define wisBufferGetGPUAddress                      wisDX12BufferGetGPUAddress
#define wisTextureWriteSubresource                  wisDX12TextureWriteSubresource
#define wisDescriptorHeapGetCPUHandle               wisDX12DescriptorHeapGetCPUHandle
#define wisDescriptorHeapWriteConstantBuffer        wisDX12DescriptorHeapWriteConstantBuffer
#define wisDescriptorHeapWriteStructuredBuffer      wisDX12DescriptorHeapWriteStructuredBuffer
#define wisDescriptorHeapWriteRWStructuredBuffer    wisDX12DescriptorHeapWriteRWStructuredBuffer
#define wisDescriptorHeapWriteSampler               wisDX12DescriptorHeapWriteSampler
#define wisDescriptorHeapWriteTexture               wisDX12DescriptorHeapWriteTexture
#define wisDescriptorHeapWriteRWTexture             wisDX12DescriptorHeapWriteRWTexture
#define wisDescriptorHeapWriteAccelerationStructure wisDX12DescriptorHeapWriteAccelerationStructure
#define wisDescriptorHeapCopyDescriptors            wisDX12DescriptorHeapCopyDescriptors
#define wisViewHeapWriteRenderTarget                wisDX12ViewHeapWriteRenderTarget
#define wisViewHeapWriteDepthStencil                wisDX12ViewHeapWriteDepthStencil
#define wisViewHeapGetViewAddress                   wisDX12ViewHeapGetViewAddress
#define wisViewHeapCopyViews                        wisDX12ViewHeapCopyViews
#define wisViewHeapGetCPUHandle                     wisDX12ViewHeapGetCPUHandle
#define wisCommandAllocatorReset                    wisDX12CommandAllocatorReset
#define wisCommandAllocatorCreateCommandList        wisDX12CommandAllocatorCreateCommandList
#define wisCommandListBegin                         wisDX12CommandListBegin
#define wisCommandListEnd                           wisDX12CommandListEnd
#define wisCommandListSetDescriptorHeaps            wisDX12CommandListSetDescriptorHeaps
#define wisCommandListSetRootSignature              wisDX12CommandListSetRootSignature
#define wisCommandListSetPushConstants              wisDX12CommandListSetPushConstants
#define wisCommandListSetPushDescriptor             wisDX12CommandListSetPushDescriptor
#define wisCommandListSetDescriptorTable            wisDX12CommandListSetDescriptorTable
#define wisCommandListInsertBarriers                wisDX12CommandListInsertBarriers
#define wisCommandListSetPipeline                   wisDX12CommandListSetPipeline
#define wisCommandListSetViewports                  wisDX12CommandListSetViewports
#define wisCommandListSetScissors                   wisDX12CommandListSetScissors
#define wisCommandListSetPrimitiveTopology          wisDX12CommandListSetPrimitiveTopology
#define wisCommandListSetDepthBias                  wisDX12CommandListSetDepthBias
#define wisCommandListSetPrimitiveRestartValue      wisDX12CommandListSetPrimitiveRestartValue
#define wisCommandListDispatch                      wisDX12CommandListDispatch
#define wisPipelineCacheSerialize                   wisDX12PipelineCacheSerialize
#define wisPipelineCacheGetSerializedSize           wisDX12PipelineCacheGetSerializedSize

#elif defined(WISDOM_VULKAN)

#define WIS_SHADER_INTERMEDIATE_SPIRV 1

//==============================================================
// Handles
//==============================================================

typedef struct WisVKSurface           WisSurface;
typedef struct WisVKViewHeap          WisViewHeap;
typedef struct WisVKPipeline          WisPipeline;
typedef struct WisVKShader            WisShader;
typedef struct WisVKPipelineCache     WisPipelineCache;
typedef struct WisVKTexture           WisTexture;
typedef struct WisVKBuffer            WisBuffer;
typedef struct WisVKDescriptorHeap    WisDescriptorHeap;
typedef struct WisVKRootSignature     WisRootSignature;
typedef struct WisVKResourceAllocator WisResourceAllocator;
typedef struct WisVKFence             WisFence;
typedef struct WisVKCommandList       WisCommandList;
typedef struct WisVKCommandAllocator  WisCommandAllocator;
typedef struct WisVKCommandQueue      WisCommandQueue;
typedef struct WisVKDevice            WisDevice;
typedef struct WisVKAdapterQuery      WisAdapterQuery;
typedef struct WisVKInstance          WisInstance;
typedef struct WisVKSurfaceView       WisSurfaceView;
typedef struct WisVKPipelineView      WisPipelineView;
typedef struct WisVKShaderView        WisShaderView;
typedef struct WisVKPipelineCacheView WisPipelineCacheView;
typedef struct WisVKTextureView       WisTextureView;
typedef struct WisVKBufferView        WisBufferView;
typedef struct WisVKRootSignatureView WisRootSignatureView;
typedef struct WisVKFenceView         WisFenceView;
typedef struct WisVKCommandListView   WisCommandListView;

//==============================================================
// Variants
//==============================================================

typedef struct WisVKDeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisVKInstanceExtensionHeader WisInstanceExtensionHeader;
typedef struct WisVKDeviceRequirements      WisDeviceRequirements;
typedef struct WisVKBufferBarrier           WisBufferBarrier;
typedef struct WisVKGlobalBarrier           WisGlobalBarrier;
typedef struct WisVKTextureBarrier          WisTextureBarrier;
typedef struct WisVKBarrierGroup            WisBarrierGroup;
typedef struct WisVKComputePipelineDesc     WisComputePipelineDesc;
typedef struct WisVKGraphicsPipelineDesc    WisGraphicsPipelineDesc;

//==============================================================
// Functions
//==============================================================

#define wisGetSurfaceView                           wisGetVKSurfaceView
#define wisGetPipelineView                          wisGetVKPipelineView
#define wisGetShaderView                            wisGetVKShaderView
#define wisGetPipelineCacheView                     wisGetVKPipelineCacheView
#define wisGetTextureView                           wisGetVKTextureView
#define wisGetBufferView                            wisGetVKBufferView
#define wisGetRootSignatureView                     wisGetVKRootSignatureView
#define wisGetFenceView                             wisGetVKFenceView
#define wisGetCommandListView                       wisGetVKCommandListView
#define wisDestroySurface                           wisVKDestroySurface
#define wisDestroyViewHeap                          wisVKDestroyViewHeap
#define wisDestroyPipeline                          wisVKDestroyPipeline
#define wisDestroyShader                            wisVKDestroyShader
#define wisDestroyPipelineCache                     wisVKDestroyPipelineCache
#define wisDestroyTexture                           wisVKDestroyTexture
#define wisDestroyBuffer                            wisVKDestroyBuffer
#define wisDestroyDescriptorHeap                    wisVKDestroyDescriptorHeap
#define wisDestroyRootSignature                     wisVKDestroyRootSignature
#define wisDestroyResourceAllocator                 wisVKDestroyResourceAllocator
#define wisDestroyFence                             wisVKDestroyFence
#define wisDestroyCommandList                       wisVKDestroyCommandList
#define wisDestroyCommandAllocator                  wisVKDestroyCommandAllocator
#define wisDestroyCommandQueue                      wisVKDestroyCommandQueue
#define wisDestroyDevice                            wisVKDestroyDevice
#define wisDestroyAdapterQuery                      wisVKDestroyAdapterQuery
#define wisDestroyInstance                          wisVKDestroyInstance
#define wisCreateInstance                           wisVKCreateInstance
#define wisInstanceQueryAdapters                    wisVKInstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount              wisVKAdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc               wisVKAdapterQueryGetAdapterDesc
#define wisAdapterQueryGetSurfaceSupport            wisVKAdapterQueryGetSurfaceSupport
#define wisAdapterQueryCreateDevice                 wisVKAdapterQueryCreateDevice
#define wisDeviceCreateCommandQueue                 wisVKDeviceCreateCommandQueue
#define wisDeviceCreateCommandAllocator             wisVKDeviceCreateCommandAllocator
#define wisDeviceCreateFence                        wisVKDeviceCreateFence
#define wisDeviceGetResourceAllocator               wisVKDeviceGetResourceAllocator
#define wisDeviceCreateRootSignature                wisVKDeviceCreateRootSignature
#define wisDeviceCreateDescriptorHeap               wisVKDeviceCreateDescriptorHeap
#define wisDeviceCreateViewHeap                     wisVKDeviceCreateViewHeap
#define wisDeviceQueryProperties                    wisVKDeviceQueryProperties
#define wisDeviceWaitForMultipleFences              wisVKDeviceWaitForMultipleFences
#define wisDeviceCreatePipelineCache                wisVKDeviceCreatePipelineCache
#define wisDeviceCreateShader                       wisVKDeviceCreateShader
#define wisDeviceCreateComputePipeline              wisVKDeviceCreateComputePipeline
#define wisDeviceCreateGraphicsPipeline             wisVKDeviceCreateGraphicsPipeline
#define wisFenceGetCompletedValue                   wisVKFenceGetCompletedValue
#define wisFenceWait                                wisVKFenceWait
#define wisFenceSignal                              wisVKFenceSignal
#define wisCommandQueueSubmit                       wisVKCommandQueueSubmit
#define wisCommandQueueSignalFence                  wisVKCommandQueueSignalFence
#define wisCommandQueueWaitFence                    wisVKCommandQueueWaitFence
#define wisResourceAllocatorCreateBuffer            wisVKResourceAllocatorCreateBuffer
#define wisResourceAllocatorCreateTexture           wisVKResourceAllocatorCreateTexture
#define wisBufferMap                                wisVKBufferMap
#define wisBufferGetGPUAddress                      wisVKBufferGetGPUAddress
#define wisTextureWriteSubresource                  wisVKTextureWriteSubresource
#define wisDescriptorHeapGetCPUHandle               wisVKDescriptorHeapGetCPUHandle
#define wisDescriptorHeapWriteConstantBuffer        wisVKDescriptorHeapWriteConstantBuffer
#define wisDescriptorHeapWriteStructuredBuffer      wisVKDescriptorHeapWriteStructuredBuffer
#define wisDescriptorHeapWriteRWStructuredBuffer    wisVKDescriptorHeapWriteRWStructuredBuffer
#define wisDescriptorHeapWriteSampler               wisVKDescriptorHeapWriteSampler
#define wisDescriptorHeapWriteTexture               wisVKDescriptorHeapWriteTexture
#define wisDescriptorHeapWriteRWTexture             wisVKDescriptorHeapWriteRWTexture
#define wisDescriptorHeapWriteAccelerationStructure wisVKDescriptorHeapWriteAccelerationStructure
#define wisDescriptorHeapCopyDescriptors            wisVKDescriptorHeapCopyDescriptors
#define wisViewHeapWriteRenderTarget                wisVKViewHeapWriteRenderTarget
#define wisViewHeapWriteDepthStencil                wisVKViewHeapWriteDepthStencil
#define wisViewHeapGetViewAddress                   wisVKViewHeapGetViewAddress
#define wisViewHeapCopyViews                        wisVKViewHeapCopyViews
#define wisViewHeapGetCPUHandle                     wisVKViewHeapGetCPUHandle
#define wisCommandAllocatorReset                    wisVKCommandAllocatorReset
#define wisCommandAllocatorCreateCommandList        wisVKCommandAllocatorCreateCommandList
#define wisCommandListBegin                         wisVKCommandListBegin
#define wisCommandListEnd                           wisVKCommandListEnd
#define wisCommandListSetDescriptorHeaps            wisVKCommandListSetDescriptorHeaps
#define wisCommandListSetRootSignature              wisVKCommandListSetRootSignature
#define wisCommandListSetPushConstants              wisVKCommandListSetPushConstants
#define wisCommandListSetPushDescriptor             wisVKCommandListSetPushDescriptor
#define wisCommandListSetDescriptorTable            wisVKCommandListSetDescriptorTable
#define wisCommandListInsertBarriers                wisVKCommandListInsertBarriers
#define wisCommandListSetPipeline                   wisVKCommandListSetPipeline
#define wisCommandListSetViewports                  wisVKCommandListSetViewports
#define wisCommandListSetScissors                   wisVKCommandListSetScissors
#define wisCommandListSetPrimitiveTopology          wisVKCommandListSetPrimitiveTopology
#define wisCommandListSetDepthBias                  wisVKCommandListSetDepthBias
#define wisCommandListSetPrimitiveRestartValue      wisVKCommandListSetPrimitiveRestartValue
#define wisCommandListDispatch                      wisVKCommandListDispatch
#define wisPipelineCacheSerialize                   wisVKPipelineCacheSerialize
#define wisPipelineCacheGetSerializedSize           wisVKPipelineCacheGetSerializedSize

#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection

#ifndef WISDOM_HANDLE_VALID_DEFINED
#define WISDOM_HANDLE_VALID_DEFINED
static inline bool wisHandleValid(const void* handle)
{
    const uint64_t zero = 0;
    return memcmp(handle, &zero, sizeof(uint64_t)) != 0;
}

#endif // WISDOM_HANDLE_VALID_DEFINED

#endif // WISDOM_CORE_H
