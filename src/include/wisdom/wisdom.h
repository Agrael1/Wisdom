// This file is generated. Do not edit directly.
#ifndef WISDOM_H
#define WISDOM_H

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
#include "generated/dx12_api.h"

//==============================================================
// Handles
//==============================================================

typedef struct WisDX12Texture           WisTexture;
typedef struct WisDX12Buffer            WisBuffer;
typedef struct WisDX12DescriptorHeap    WisDescriptorHeap;
typedef struct WisDX12PipelineLayout    WisPipelineLayout;
typedef struct WisDX12ResourceAllocator WisResourceAllocator;
typedef struct WisDX12Fence             WisFence;
typedef struct WisDX12CommandList       WisCommandList;
typedef struct WisDX12CommandAllocator  WisCommandAllocator;
typedef struct WisDX12CommandQueue      WisCommandQueue;
typedef struct WisDX12Device            WisDevice;
typedef struct WisDX12AdapterQuery      WisAdapterQuery;
typedef struct WisDX12Instance          WisInstance;
typedef struct WisDX12FenceView         WisFenceView;
typedef struct WisDX12CommandListView   WisCommandListView;

//==============================================================
// Variants
//==============================================================

typedef struct WisDX12DeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisDX12InstanceExtensionHeader WisInstanceExtensionHeader;
typedef struct WisDX12DeviceRequirements      WisDeviceRequirements;

//==============================================================
// Functions
//==============================================================

#define wisDestroyTexture                    wisDX12DestroyTexture
#define wisDestroyBuffer                     wisDX12DestroyBuffer
#define wisDestroyDescriptorHeap             wisDX12DestroyDescriptorHeap
#define wisDestroyPipelineLayout             wisDX12DestroyPipelineLayout
#define wisDestroyResourceAllocator          wisDX12DestroyResourceAllocator
#define wisDestroyFence                      wisDX12DestroyFence
#define wisDestroyCommandList                wisDX12DestroyCommandList
#define wisDestroyCommandAllocator           wisDX12DestroyCommandAllocator
#define wisDestroyCommandQueue               wisDX12DestroyCommandQueue
#define wisDestroyDevice                     wisDX12DestroyDevice
#define wisDestroyAdapterQuery               wisDX12DestroyAdapterQuery
#define wisDestroyInstance                   wisDX12DestroyInstance
#define wisCreateInstance                    wisDX12CreateInstance
#define wisInstanceQueryAdapters             wisDX12InstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount       wisDX12AdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc        wisDX12AdapterQueryGetAdapterDesc
#define wisAdapterQueryCreateDevice          wisDX12AdapterQueryCreateDevice
#define wisDeviceCreateCommandQueue          wisDX12DeviceCreateCommandQueue
#define wisDeviceCreateCommandAllocator      wisDX12DeviceCreateCommandAllocator
#define wisDeviceCreateFence                 wisDX12DeviceCreateFence
#define wisDeviceGetResourceAllocator        wisDX12DeviceGetResourceAllocator
#define wisDeviceCreatePipelineLayout        wisDX12DeviceCreatePipelineLayout
#define wisDeviceCreateDescriptorHeap        wisDX12DeviceCreateDescriptorHeap
#define wisDeviceQueryProperties             wisDX12DeviceQueryProperties
#define wisFenceGetCompletedValue            wisDX12FenceGetCompletedValue
#define wisFenceWait                         wisDX12FenceWait
#define wisFenceSignal                       wisDX12FenceSignal
#define wisCommandQueueSubmit                wisDX12CommandQueueSubmit
#define wisCommandQueueSignalFence           wisDX12CommandQueueSignalFence
#define wisCommandQueueWaitFence             wisDX12CommandQueueWaitFence
#define wisResourceAllocatorCreateBuffer     wisDX12ResourceAllocatorCreateBuffer
#define wisResourceAllocatorCreateTexture    wisDX12ResourceAllocatorCreateTexture
#define wisBufferMap                         wisDX12BufferMap
#define wisCommandAllocatorReset             wisDX12CommandAllocatorReset
#define wisCommandAllocatorCreateCommandList wisDX12CommandAllocatorCreateCommandList
#define wisCommandListClose                  wisDX12CommandListClose
#define wisGetFenceView                      wisGetDX12FenceView
#define wisGetCommandListView                wisGetDX12CommandListView

#define wisGetView(handle)                                        \
    _Generic((handle),                                            \
            const WisDX12Fence*: wisGetDX12FenceView,             \
            WisDX12Fence*: wisGetDX12FenceView,                   \
            const WisDX12CommandList*: wisGetDX12CommandListView, \
            WisDX12CommandList*: wisGetDX12CommandListView,       \
            default: (void)0)(handle)
#elif defined(WISDOM_VULKAN)
#include "generated/vk_api.h"

//==============================================================
// Handles
//==============================================================

typedef struct WisVKTexture           WisTexture;
typedef struct WisVKBuffer            WisBuffer;
typedef struct WisVKDescriptorHeap    WisDescriptorHeap;
typedef struct WisVKPipelineLayout    WisPipelineLayout;
typedef struct WisVKResourceAllocator WisResourceAllocator;
typedef struct WisVKFence             WisFence;
typedef struct WisVKCommandList       WisCommandList;
typedef struct WisVKCommandAllocator  WisCommandAllocator;
typedef struct WisVKCommandQueue      WisCommandQueue;
typedef struct WisVKDevice            WisDevice;
typedef struct WisVKAdapterQuery      WisAdapterQuery;
typedef struct WisVKInstance          WisInstance;
typedef struct WisVKFenceView         WisFenceView;
typedef struct WisVKCommandListView   WisCommandListView;

//==============================================================
// Variants
//==============================================================

typedef struct WisVKDeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisVKInstanceExtensionHeader WisInstanceExtensionHeader;
typedef struct WisVKDeviceRequirements      WisDeviceRequirements;

//==============================================================
// Functions
//==============================================================

#define wisDestroyTexture                    wisVKDestroyTexture
#define wisDestroyBuffer                     wisVKDestroyBuffer
#define wisDestroyDescriptorHeap             wisVKDestroyDescriptorHeap
#define wisDestroyPipelineLayout             wisVKDestroyPipelineLayout
#define wisDestroyResourceAllocator          wisVKDestroyResourceAllocator
#define wisDestroyFence                      wisVKDestroyFence
#define wisDestroyCommandList                wisVKDestroyCommandList
#define wisDestroyCommandAllocator           wisVKDestroyCommandAllocator
#define wisDestroyCommandQueue               wisVKDestroyCommandQueue
#define wisDestroyDevice                     wisVKDestroyDevice
#define wisDestroyAdapterQuery               wisVKDestroyAdapterQuery
#define wisDestroyInstance                   wisVKDestroyInstance
#define wisCreateInstance                    wisVKCreateInstance
#define wisInstanceQueryAdapters             wisVKInstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount       wisVKAdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc        wisVKAdapterQueryGetAdapterDesc
#define wisAdapterQueryCreateDevice          wisVKAdapterQueryCreateDevice
#define wisDeviceCreateCommandQueue          wisVKDeviceCreateCommandQueue
#define wisDeviceCreateCommandAllocator      wisVKDeviceCreateCommandAllocator
#define wisDeviceCreateFence                 wisVKDeviceCreateFence
#define wisDeviceGetResourceAllocator        wisVKDeviceGetResourceAllocator
#define wisDeviceCreatePipelineLayout        wisVKDeviceCreatePipelineLayout
#define wisDeviceCreateDescriptorHeap        wisVKDeviceCreateDescriptorHeap
#define wisDeviceQueryProperties             wisVKDeviceQueryProperties
#define wisFenceGetCompletedValue            wisVKFenceGetCompletedValue
#define wisFenceWait                         wisVKFenceWait
#define wisFenceSignal                       wisVKFenceSignal
#define wisCommandQueueSubmit                wisVKCommandQueueSubmit
#define wisCommandQueueSignalFence           wisVKCommandQueueSignalFence
#define wisCommandQueueWaitFence             wisVKCommandQueueWaitFence
#define wisResourceAllocatorCreateBuffer     wisVKResourceAllocatorCreateBuffer
#define wisResourceAllocatorCreateTexture    wisVKResourceAllocatorCreateTexture
#define wisBufferMap                         wisVKBufferMap
#define wisCommandAllocatorReset             wisVKCommandAllocatorReset
#define wisCommandAllocatorCreateCommandList wisVKCommandAllocatorCreateCommandList
#define wisCommandListClose                  wisVKCommandListClose
#define wisGetFenceView                      wisGetVKFenceView
#define wisGetCommandListView                wisGetVKCommandListView

#define wisGetView(handle)                                    \
    _Generic((handle),                                        \
            const WisVKFence*: wisGetVKFenceView,             \
            WisVKFence*: wisGetVKFenceView,                   \
            const WisVKCommandList*: wisGetVKCommandListView, \
            WisVKCommandList*: wisGetVKCommandListView,       \
            default: (void)0)(handle)
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_H
