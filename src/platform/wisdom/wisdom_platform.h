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

#include "../wisdom_platform/generated/c_platform_api.h"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

typedef struct WisDX12Win32Extension WisWin32Extension;
#define wisInitWin32Extension    wisDX12InitWin32Extension
#define wisDestroyWin32Extension wisDX12DestroyWin32Extension

typedef struct WisDX12UWPExtension WisUWPExtension;
#define wisInitUWPExtension    wisDX12InitUWPExtension
#define wisDestroyUWPExtension wisDX12DestroyUWPExtension

#elif defined(WISDOM_VULKAN)

typedef struct WisVKXlibExtension WisXlibExtension;
#define wisInitXlibExtension       wisVKInitXlibExtension
#define wisDestroyXlibExtension    wisVKDestroyXlibExtension

typedef struct WisVKXCBExtension WisXCBExtension;
#define wisInitXCBExtension        wisVKInitXCBExtension
#define wisDestroyXCBExtension     wisVKDestroyXCBExtension

typedef struct WisVKWaylandExtension WisWaylandExtension;
#define wisInitWaylandExtension    wisVKInitWaylandExtension
#define wisDestroyWaylandExtension wisVKDestroyWaylandExtension

typedef struct WisVKWin32Extension WisWin32Extension;
#define wisInitWin32Extension      wisVKInitWin32Extension
#define wisDestroyWin32Extension   wisVKDestroyWin32Extension

#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection

#endif // WISDOM_PLATFORM_H
