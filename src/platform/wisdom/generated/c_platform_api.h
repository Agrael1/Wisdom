// This file is generated. Do not edit directly.
#ifndef WISDOM_C_PLATFORM_API_H
#define WISDOM_C_PLATFORM_API_H
#include <wisdom/global/definitions.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

#endif // WIS_USE_PLATFORM_UWP

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_PLATFORM_API_H
