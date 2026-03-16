// This file is generated. Do not edit directly.
#ifndef WISDOM_C_PLATFORM_API_H
#define WISDOM_C_PLATFORM_API_H
#include <wisdom/global/definitions.h>
#include <wisdom_platform/generated/wisdom_exports.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef WISDOM_DX12
#include <wisdom/generated/dx12_api.h>
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#include <wisdom/generated/vk_api.h>
#endif // WISDOM_VULKAN

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. X11 platform using Xlib.
 *
 * */
#ifdef WIS_USE_PLATFORM_XLIB

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info. Uses opaque types to avoid Xlib.h inclusion in public headers.
 *
 * */
typedef struct WisXlibWindowDesc {
    void*    display; ///< Pointer to the X11 Display. Cast to Display* internally.
    uint64_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
} WisXlibWindowDesc;

#if defined(WISDOM_VULKAN)

/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(WisVKXlibExtension, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisXlibExtension handle.
 * @param self is a pointer to the valid WisXlibExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKDestroyXlibExtension(WisVKXlibExtension* self);

#endif // defined(WISDOM_VULKAN)

#endif // WIS_USE_PLATFORM_XLIB

/**
 * @brief Provided by Wisdom 0.7.0. X11 platform using XCB.
 *
 * */
#ifdef WIS_USE_PLATFORM_XCB

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info for XCB. Uses opaque types to avoid XCB headers in public headers.
 *
 * */
typedef struct WisXCBWindowDesc {
    void*    connection; ///< Pointer to the XCB connection. Cast to xcb_connection_t* internally.
    uint32_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
} WisXCBWindowDesc;

#if defined(WISDOM_VULKAN)

#endif // defined(WISDOM_VULKAN)

#endif // WIS_USE_PLATFORM_XCB

/**
 * @brief Provided by Wisdom 0.7.0. Wayland platform.
 *
 * */
#ifdef WIS_USE_PLATFORM_WAYLAND

/**
 * @brief Provided by Wisdom 0.7.0. Wayland surface creation info. Uses opaque types to avoid Wayland headers in public headers.
 *
 * */
typedef struct WisWaylandWindowDesc {
    void* display; ///< Pointer to the Wayland display. Cast to wl_display* internally.
    void* surface; ///< Pointer to the Wayland surface. Cast to wl_surface* internally.
} WisWaylandWindowDesc;

#if defined(WISDOM_VULKAN)

#endif // defined(WISDOM_VULKAN)

#endif // WIS_USE_PLATFORM_WAYLAND

/**
 * @brief Provided by Wisdom 0.7.0. Standard Windows platform.
 *
 * */
#ifdef WIS_USE_PLATFORM_WIN32

/**
 * @brief Provided by Wisdom 0.7.0. Win32 surface creation info.
 *
 * */
typedef struct WisWin32WindowDesc {
    void* hinstance; ///< HINSTANCE of the window. Cast to HINSTANCE internally.
    void* hwnd; ///< HWND of the window. Cast to HWND internally.
} WisWin32WindowDesc;

#if defined(WISDOM_DX12)

/**
 * @brief Provided by Wisdom 0.7.0. Extension for Win32 surface creation functions.
 *
 * */
WIS_DEFINE_DX12_INSTANCE_EXT_HANDLE(WisDX12Win32Extension, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisWin32Extension handle.
 * @param self is a pointer to the valid WisWin32Extension instance.
 *
 * */
WISDOM_PLATFORM_API void wisDX12DestroyWin32Extension(WisDX12Win32Extension* self);

#endif // defined(WISDOM_DX12)

#if defined(WISDOM_VULKAN)

/**
 * @brief Provided by Wisdom 0.7.0. Extension for Win32 surface creation functions.
 *
 * */
WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(WisVKWin32Extension, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisWin32Extension handle.
 * @param self is a pointer to the valid WisWin32Extension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKDestroyWin32Extension(WisVKWin32Extension* self);

#endif // defined(WISDOM_VULKAN)

#endif // WIS_USE_PLATFORM_WIN32

/**
 * @brief Provided by Wisdom 0.7.0. Universal Windows Platform.
 *
 * */
#ifdef WIS_USE_PLATFORM_UWP

/**
 * @brief Provided by Wisdom 0.7.0. UWP surface creation info. Uses opaque types to avoid Windows Runtime headers in public headers.
 *
 * */
typedef struct WisUWPWindowDesc {
    void* core_window; ///< Pointer to the UWP CoreWindow. Cast to ICoreWindow* internally.
} WisUWPWindowDesc;

#if defined(WISDOM_DX12)

#endif // defined(WISDOM_DX12)

#endif // WIS_USE_PLATFORM_UWP

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_PLATFORM_API_H
