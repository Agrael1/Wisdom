// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_H
#define WISDOM_RAYTRACING_H

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifndef FORCEVK_SWITCH
#    if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#        define FORCEVK_SWITCH 1
#    else
#        define FORCEVK_SWITCH 0
#    endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "../raytracing/generated/c_api.h"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

//==============================================================
// Handles
//==============================================================

typedef struct WisDX12AccelerationStructure WisAccelerationStructure;
typedef struct WisDX12RaytracingExtension WisRaytracingExtension;

//==============================================================
// Variants
//==============================================================

typedef struct WisDX12AccelerationStructureDesc WisAccelerationStructureDesc;

//==============================================================
// Functions
//==============================================================

#    define wisDestroyAccelerationStructure                   wisDX12DestroyAccelerationStructure
#    define wisDestroyRaytracingExtension                     wisDX12DestroyRaytracingExtension
#    define wisInitRaytracingExtension                        wisDX12InitRaytracingExtension
#    define wisRaytracingExtensionSupported                   wisDX12RaytracingExtensionSupported
#    define wisRaytracingExtensionCreateAccelerationStructure wisDX12RaytracingExtensionCreateAccelerationStructure
#    define wisAccelerationStructureGetGPUAddress             wisDX12AccelerationStructureGetGPUAddress

#elif defined(WISDOM_VULKAN)

//==============================================================
// Handles
//==============================================================

typedef struct WisVKAccelerationStructure WisAccelerationStructure;
typedef struct WisVKRaytracingExtension WisRaytracingExtension;

//==============================================================
// Variants
//==============================================================

typedef struct WisVKAccelerationStructureDesc WisAccelerationStructureDesc;

//==============================================================
// Functions
//==============================================================

#    define wisDestroyAccelerationStructure                   wisVKDestroyAccelerationStructure
#    define wisDestroyRaytracingExtension                     wisVKDestroyRaytracingExtension
#    define wisInitRaytracingExtension                        wisVKInitRaytracingExtension
#    define wisRaytracingExtensionSupported                   wisVKRaytracingExtensionSupported
#    define wisRaytracingExtensionCreateAccelerationStructure wisVKRaytracingExtensionCreateAccelerationStructure
#    define wisAccelerationStructureGetGPUAddress             wisVKAccelerationStructureGetGPUAddress

#else
#    error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection

#ifndef WISDOM_HANDLE_VALID_DEFINED
#    define WISDOM_HANDLE_VALID_DEFINED
static inline bool wisHandleValid(const void* handle)
{
    const uint64_t zero = 0;
    return memcmp(handle, &zero, sizeof(uint64_t)) != 0;
}

#endif // WISDOM_HANDLE_VALID_DEFINED

#endif // WISDOM_RAYTRACING_H
