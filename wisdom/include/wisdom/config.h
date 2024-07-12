#pragma once

#ifdef WISDOM_MANUAL_DETECT
#if !defined(WISDOM_UWP) && !defined(WISDOM_WINDOWS) && !defined(WISDOM_LINUX)
// Try to detect the platform
#if defined(_WIN32)
#ifdef _WINRT_DLL // UWP
#define WISDOM_UWP 1
#else // _WINRT_DLL
#define WISDOM_WINDOWS 1
#endif // _WINRT_DLL
#elif defined(__linux__)
#define WISDOM_LINUX 1
#else
#error "Platform not supported"
#endif // _WIN32

#if !defined(WISDOM_VULKAN) && __has_include(<vulkan/vulkan.h>)
#define WISDOM_VULKAN 1
#endif // WISDOM_VULKAN

#if !defined(WISDOM_DX12) && defined(_WIN32) && __has_include(<d3d12.h>)
#define WISDOM_DX12 1
#endif // WISDOM_DX12

#ifndef DEBUG_MODE && defined(_DEBUG)
#define DEBUG_MODE 1
#endif // DEBUG_MODE

#define DEBUG_LAYER 0


#endif // !WISDOM_UWP && !WISDOM_WINDOWS && !WISDOM_LINUX
#endif // WISDOM_MANUAL_DETECT

