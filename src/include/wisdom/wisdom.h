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

typedef struct WisDX12DescriptorHeap    WisDescriptorHeap;
typedef struct WisDX12PipelineLayout    WisPipelineLayout;
typedef struct WisDX12ResourceAllocator WisResourceAllocator;
typedef struct WisDX12Fence             WisFence;
typedef struct WisDX12CommandList       WisCommandList;
typedef struct WisDX12CommandQueue      WisCommandQueue;
typedef struct WisDX12Device            WisDevice;
typedef struct WisDX12AdapterQuery      WisAdapterQuery;
typedef struct WisDX12Instance          WisInstance;

//==============================================================
// Variants
//==============================================================

typedef struct WisDX12DeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisDX12InstanceExtensionHeader WisInstanceExtensionHeader;

//==============================================================
// Functions
//==============================================================

#define wisDestroyDescriptorHeap         wisDX12DestroyDescriptorHeap
#define wisDestroyPipelineLayout         wisDX12DestroyPipelineLayout
#define wisDestroyResourceAllocator      wisDX12DestroyResourceAllocator
#define wisDestroyFence                  wisDX12DestroyFence
#define wisDestroyCommandList            wisDX12DestroyCommandList
#define wisDestroyCommandQueue           wisDX12DestroyCommandQueue
#define wisDestroyDevice                 wisDX12DestroyDevice
#define wisDestroyAdapterQuery           wisDX12DestroyAdapterQuery
#define wisDestroyInstance               wisDX12DestroyInstance
#define wisCreateInstance                wisDX12CreateInstance
#define wisInstanceQueryAdapters         wisDX12InstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount   wisDX12AdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc    wisDX12AdapterQueryGetAdapterDesc
#define wisAdapterQueryCreateDevice      wisDX12AdapterQueryCreateDevice
#define wisDeviceCreateCommandQueue      wisDX12DeviceCreateCommandQueue
#define wisDeviceCreateCommandList       wisDX12DeviceCreateCommandList
#define wisDeviceCreateFence             wisDX12DeviceCreateFence
#define wisDeviceCreateResourceAllocator wisDX12DeviceCreateResourceAllocator
#define wisDeviceCreatePipelineLayout    wisDX12DeviceCreatePipelineLayout
#define wisDeviceCreateDescriptorHeap    wisDX12DeviceCreateDescriptorHeap

#elif defined(WISDOM_VULKAN)
#include "generated/vk_api.h"

//==============================================================
// Handles
//==============================================================

typedef struct WisVKDescriptorHeap    WisDescriptorHeap;
typedef struct WisVKPipelineLayout    WisPipelineLayout;
typedef struct WisVKResourceAllocator WisResourceAllocator;
typedef struct WisVKFence             WisFence;
typedef struct WisVKCommandList       WisCommandList;
typedef struct WisVKCommandQueue      WisCommandQueue;
typedef struct WisVKDevice            WisDevice;
typedef struct WisVKAdapterQuery      WisAdapterQuery;
typedef struct WisVKInstance          WisInstance;

//==============================================================
// Variants
//==============================================================

typedef struct WisVKDeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisVKInstanceExtensionHeader WisInstanceExtensionHeader;

//==============================================================
// Functions
//==============================================================

#define wisDestroyDescriptorHeap         wisVKDestroyDescriptorHeap
#define wisDestroyPipelineLayout         wisVKDestroyPipelineLayout
#define wisDestroyResourceAllocator      wisVKDestroyResourceAllocator
#define wisDestroyFence                  wisVKDestroyFence
#define wisDestroyCommandList            wisVKDestroyCommandList
#define wisDestroyCommandQueue           wisVKDestroyCommandQueue
#define wisDestroyDevice                 wisVKDestroyDevice
#define wisDestroyAdapterQuery           wisVKDestroyAdapterQuery
#define wisDestroyInstance               wisVKDestroyInstance
#define wisCreateInstance                wisVKCreateInstance
#define wisInstanceQueryAdapters         wisVKInstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount   wisVKAdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc    wisVKAdapterQueryGetAdapterDesc
#define wisAdapterQueryCreateDevice      wisVKAdapterQueryCreateDevice
#define wisDeviceCreateCommandQueue      wisVKDeviceCreateCommandQueue
#define wisDeviceCreateCommandList       wisVKDeviceCreateCommandList
#define wisDeviceCreateFence             wisVKDeviceCreateFence
#define wisDeviceCreateResourceAllocator wisVKDeviceCreateResourceAllocator
#define wisDeviceCreatePipelineLayout    wisVKDeviceCreatePipelineLayout
#define wisDeviceCreateDescriptorHeap    wisVKDeviceCreateDescriptorHeap

#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_H
