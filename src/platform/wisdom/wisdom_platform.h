// This file is generated. Do not edit directly.
#ifndef WISDOM_PLATFORM_H
#define WISDOM_PLATFORM_H

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

#include "../wisdom_platform/generated/c_api.h"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

//==============================================================
// Handles
//==============================================================

typedef struct WisDX12Win32Extension WisWin32Extension;
typedef struct WisDX12UWPExtension   WisUWPExtension;

//==============================================================
// Functions
//==============================================================

#define wisDestroyWin32Extension       wisDX12DestroyWin32Extension
#define wisInitWin32Extension          wisDX12InitWin32Extension
#define wisDestroyUWPExtension         wisDX12DestroyUWPExtension
#define wisInitUWPExtension            wisDX12InitUWPExtension
#define wisWin32ExtensionCreateSurface wisDX12Win32ExtensionCreateSurface
#define wisUWPExtensionCreateSurface   wisDX12UWPExtensionCreateSurface

#elif defined(WISDOM_VULKAN)

//==============================================================
// Handles
//==============================================================

typedef struct WisVKXlibExtension    WisXlibExtension;
typedef struct WisVKXCBExtension     WisXCBExtension;
typedef struct WisVKWaylandExtension WisWaylandExtension;
typedef struct WisVKWin32Extension   WisWin32Extension;

//==============================================================
// Functions
//==============================================================

#define wisDestroyXlibExtension          wisVKDestroyXlibExtension
#define wisInitXlibExtension             wisVKInitXlibExtension
#define wisDestroyXCBExtension           wisVKDestroyXCBExtension
#define wisInitXCBExtension              wisVKInitXCBExtension
#define wisDestroyWaylandExtension       wisVKDestroyWaylandExtension
#define wisInitWaylandExtension          wisVKInitWaylandExtension
#define wisDestroyWin32Extension         wisVKDestroyWin32Extension
#define wisInitWin32Extension            wisVKInitWin32Extension
#define wisXlibExtensionCreateSurface    wisVKXlibExtensionCreateSurface
#define wisXCBExtensionCreateSurface     wisVKXCBExtensionCreateSurface
#define wisWaylandExtensionCreateSurface wisVKWaylandExtensionCreateSurface
#define wisWin32ExtensionCreateSurface   wisVKWin32ExtensionCreateSurface

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

#endif // WISDOM_PLATFORM_H
