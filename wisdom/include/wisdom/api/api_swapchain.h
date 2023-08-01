#pragma once
#ifndef WISDOM_MODULES

#ifdef WISDOM_WINDOWS
#include <Windows.h>
#endif // WISDOM_WINDOWS
#ifdef WISDOM_LINUX
#include <xcb/xproto.h>
struct wl_display;
struct wl_surface;
#endif // WISDOM_LINUX

#include <wisdom/api/api_common.h>

#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
    /// @brief Describes the format of a texture used to present to the screen.
    struct SwapchainOptions {
        constexpr static inline auto default_format = DataFormat::b8g8r8a8_unorm; //< Default format for swapchain images.
        constexpr static inline auto default_frames = 2u; //< Default number of swapchain images.
        constexpr static inline uint32_t default_width = 1280;
        constexpr static inline uint32_t default_height = 720;

        SwapchainOptions() = default;

        /// @brief Construct a swapchain options.
        /// @param width Width of the swapchain images.
        /// @param height Height of the swapchain images.
        /// @param frame_count Number of swapchain images.
        /// @param format Data format of the swapchain images.
        /// @param stereo Stereo support.
        SwapchainOptions(uint32_t width, uint32_t height, uint32_t frame_count = default_frames, DataFormat format = default_format, bool stereo = false)
            : width(width), height(height), frame_count(frame_count), format(format), stereo(stereo)
        {
        }

        uint32_t width = default_width; //< Width of the swapchain images.
        uint32_t height = default_height; //< Height of the swapchain images.
        uint32_t frame_count = default_frames; //< Number of swapchain images.
        DataFormat format = default_format; //< Format of the swapchain images.
        bool stereo = false; //< Ask the swapchain images for stereo support.
    };

    // TODO: Wayland and WinUI composition.
    /// @brief Describes the output surface parameters.
    struct SurfaceParameters {
        /// @brief Type of the surface. Depends on the platform.
        enum class Type {
            None,
            Win32,
            WinRT,
            X11,
            Metal,
            Wayland
        };

        SurfaceParameters() = default;
#if WISDOM_WINDOWS
        /// @brief Create a surface parameters for a Win32 window.
        /// @param hwnd Win32 window handle.
        explicit SurfaceParameters(HWND hwnd)
            : type(Type::Win32), hwnd(hwnd)
        {
        }
#endif
#if WISDOM_UWP
        /// @brief Create a surface parameters for a WinRT CoreWindow.
        /// @param core_window Pointer to the CoreWindow.
        explicit SurfaceParameters(IUnknown* core_window)
            : type(Type::WinRT), core_window(core_window)
        {
        }
#endif
#if WISDOM_LINUX
        /// @brief Create a surface parameters for a X11 window.
        /// @param connection X11 connection.
        /// @param window X11 window.
        explicit SurfaceParameters(xcb_connection_t* connection, xcb_window_t window)
            : type(Type::X11), x11{ connection, window }
        {
        }

        /// @brief Create a surface parameters struct for a wayland window.
        /// @param display wayland display handle
        /// @param surface wayland surface handle
        explicit SurfaceParameters(wl_display* display, wl_surface* surface)
            : type(Type::Wayland), wayland{ display, surface }
        {
        }
#endif
#if WISDOM_MACOS
        /// @brief Create a surface parameters for a Metal layer.
        /// @param layer Metal layer.
        explicit SurfaceParameters(CAMetalLayer* layer)
            : type(Type::Metal), layer(layer)
        {
        }
#endif
        Type type = Type::None; //< Type of the surface.
        union {
#if WISDOM_WINDOWS
            HWND hwnd; //< Win32 window handle.
#endif // WISDOM_WINDOWS
#if WISDOM_UWP
            IUnknown* core_window; //< WinRT CoreWindow.
#endif // WISDOM_UWP
#if WISDOM_LINUX
            struct
            {
                xcb_connection_t* connection; //< X11 connection.
                xcb_window_t window; //< X11 window handle.
            } x11; //< X11 window.
            struct {
                wl_display* display;
                wl_surface* surface;
            } wayland;
#endif // WISDOM_LINUX
#if WISDOM_MACOS
            CAMetalLayer* layer; //< Metal layer.
#endif // WISDOM_MACOS
        };
    };
}
