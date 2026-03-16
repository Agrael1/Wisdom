// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_PLATFORM_API_HPP
#define WISDOM_CPP_PLATFORM_API_HPP
#ifdef __cplusplus
#include <wisdom/global/definitions.h>
#include <wisdom/bridge/span.hpp>

namespace wis {
/**
 * @brief Provided by Wisdom 0.7.0. X11 platform using Xlib.
 *
 * */
#ifdef WIS_USE_PLATFORM_XLIB

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info. Uses opaque types to avoid Xlib.h inclusion in public headers.
 *
 * */
struct XlibWindowDesc {
    void*         display; ///< Pointer to the X11 Display. Cast to Display* internally.
    std::uint64_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
};

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
struct XCBWindowDesc {
    void*         connection; ///< Pointer to the XCB connection. Cast to xcb_connection_t* internally.
    std::uint32_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
};

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
struct WaylandWindowDesc {
    void* display; ///< Pointer to the Wayland display. Cast to wl_display* internally.
    void* surface; ///< Pointer to the Wayland surface. Cast to wl_surface* internally.
};

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
struct Win32WindowDesc {
    void* hinstance; ///< HINSTANCE of the window. Cast to HINSTANCE internally.
    void* hwnd; ///< HWND of the window. Cast to HWND internally.
};

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
struct UWPWindowDesc {
    void* core_window; ///< Pointer to the UWP CoreWindow. Cast to ICoreWindow* internally.
};

#endif // WIS_USE_PLATFORM_UWP

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_PLATFORM_API_HPP
