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

typedef struct WisDX12AdapterQuery WisAdapterQuery;
typedef struct WisDX12Instance     WisInstance;

//==============================================================
// Variants
//==============================================================

typedef struct WisDX12DeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisDX12InstanceExtensionHeader WisInstanceExtensionHeader;

//==============================================================
// Functions
//==============================================================

#define wisDestroyAdapterQuery         wisDX12DestroyAdapterQuery
#define wisDestroyInstance             wisDX12DestroyInstance
#define wisCreateInstance              wisDX12CreateInstance
#define wisInstanceQueryAdapters       wisDX12InstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount wisDX12AdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc  wisDX12AdapterQueryGetAdapterDesc

#elif defined(WISDOM_VULKAN)
#include "generated/vk_api.h"

//==============================================================
// Handles
//==============================================================

typedef struct WisVKAdapterQuery WisAdapterQuery;
typedef struct WisVKInstance     WisInstance;

//==============================================================
// Variants
//==============================================================

typedef struct WisVKDeviceExtensionHeader   WisDeviceExtensionHeader;
typedef struct WisVKInstanceExtensionHeader WisInstanceExtensionHeader;

//==============================================================
// Functions
//==============================================================

#define wisDestroyAdapterQuery         wisVKDestroyAdapterQuery
#define wisDestroyInstance             wisVKDestroyInstance
#define wisCreateInstance              wisVKCreateInstance
#define wisInstanceQueryAdapters       wisVKInstanceQueryAdapters
#define wisAdapterQueryGetAdapterCount wisVKAdapterQueryGetAdapterCount
#define wisAdapterQueryGetAdapterDesc  wisVKAdapterQueryGetAdapterDesc

#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_H
