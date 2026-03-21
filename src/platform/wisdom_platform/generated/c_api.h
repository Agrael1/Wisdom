// This file is generated. Do not edit directly.
#ifndef WISDOM_PLATFORM_C_API_H
#define WISDOM_PLATFORM_C_API_H
#include <wisdom/generated/c_api.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info. Uses opaque types to avoid Xlib.h inclusion in public headers.
 *
 * */
typedef struct WisXlibWindowDesc {
    void*    display; ///< Pointer to the X11 Display. Cast to Display* internally.
    uint64_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
} WisXlibWindowDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Win32 surface creation info.
 *
 * */
typedef struct WisWin32WindowDesc {
    void* hinstance; ///< HINSTANCE of the window. Cast to HINSTANCE internally.
    void* hwnd; ///< HWND of the window. Cast to HWND internally.
} WisWin32WindowDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Wayland surface creation info. Uses opaque types to avoid Wayland headers in public headers.
 *
 * */
typedef struct WisWaylandWindowDesc {
    void* display; ///< Pointer to the Wayland display. Cast to wl_display* internally.
    void* surface; ///< Pointer to the Wayland surface. Cast to wl_surface* internally.
} WisWaylandWindowDesc;

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info for XCB. Uses opaque types to avoid XCB headers in public headers.
 *
 * */
typedef struct WisXCBWindowDesc {
    void*    connection; ///< Pointer to the XCB connection. Cast to xcb_connection_t* internally.
    uint32_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
} WisXCBWindowDesc;

/**
 * @brief Provided by Wisdom 0.7.0. UWP surface creation info. Uses opaque types to avoid Windows Runtime headers in public headers.
 *
 * */
typedef struct WisUWPWindowDesc {
    void* core_window; ///< Pointer to the UWP CoreWindow. Cast to ICoreWindow* internally.
} WisUWPWindowDesc;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_PLATFORM_C_API_H
