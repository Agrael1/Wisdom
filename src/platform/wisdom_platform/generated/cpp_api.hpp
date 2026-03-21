// This file is generated. Do not edit directly.
#ifndef WISDOM_PLATFORM_CPP_API_HPP
#define WISDOM_PLATFORM_CPP_API_HPP
#ifndef __cplusplus
#error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>

namespace wis {

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info. Uses opaque types to avoid Xlib.h inclusion in public headers.
 *
 * */
struct XlibWindowDesc {
    void*         display; ///< Pointer to the X11 Display. Cast to Display* internally.
    std::uint64_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
};

/**
 * @brief Provided by Wisdom 0.7.0. Win32 surface creation info.
 *
 * */
struct Win32WindowDesc {
    void* hinstance; ///< HINSTANCE of the window. Cast to HINSTANCE internally.
    void* hwnd; ///< HWND of the window. Cast to HWND internally.
};

/**
 * @brief Provided by Wisdom 0.7.0. Wayland surface creation info. Uses opaque types to avoid Wayland headers in public headers.
 *
 * */
struct WaylandWindowDesc {
    void* display; ///< Pointer to the Wayland display. Cast to wl_display* internally.
    void* surface; ///< Pointer to the Wayland surface. Cast to wl_surface* internally.
};

/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info for XCB. Uses opaque types to avoid XCB headers in public headers.
 *
 * */
struct XCBWindowDesc {
    void*         connection; ///< Pointer to the XCB connection. Cast to xcb_connection_t* internally.
    std::uint32_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
};

/**
 * @brief Provided by Wisdom 0.7.0. UWP surface creation info. Uses opaque types to avoid Windows Runtime headers in public headers.
 *
 * */
struct UWPWindowDesc {
    void* core_window; ///< Pointer to the UWP CoreWindow. Cast to ICoreWindow* internally.
};

} // namespace wis
#endif // WISDOM_PLATFORM_CPP_API_HPP
