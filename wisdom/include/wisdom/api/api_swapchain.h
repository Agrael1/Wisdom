#pragma once

#ifdef WISDOM_WINDOWS
#include <wisdom/util/winsetup.h>
#endif // WISDOM_WINDOWS
#ifdef WISDOM_LINUX
#include <xcb/xproto.h>
#endif // WISDOM_LINUX

#include <wisdom/api/api_common.h>


namespace wis
{
	struct SwapchainOptions
	{
		constexpr static inline auto default_format = DataFormat::b8g8r8a8_unorm;
		uint32_t width = 1280;
		uint32_t height = 720;
		uint32_t frame_count = 2;
		DataFormat format = default_format;
		bool stereo = false;
	};

	// TODO: linux and Wayland
	struct SurfaceParameters
	{
		enum class Type {
			None,
			Win32,
			WinRT,
			X11,
			Metal,
		};
	public:
		SurfaceParameters() = default;
#if WISDOM_WINDOWS
		explicit SurfaceParameters(HWND hwnd)
			:type(Type::Win32), hwnd(hwnd)
		{}
#endif
#if WISDOM_UWP
		explicit SurfaceParameters(IUnknown* core_window)
			:type(Type::WinRT), core_window(core_window)
		{}
#endif
#if WISDOM_LINUX
		explicit SurfaceParameters(xcb_connection_t* connection, xcb_window_t window)
			:type(Type::X11), x11{ connection, window }
		{}
#endif
#if WISDOM_MACOS
		explicit SurfaceParameters(CAMetalLayer* layer)
			:type(Type::Metal), layer(layer)
		{}
#endif
	public:
		bool IsWinRT()const noexcept
		{
			return type == Type::WinRT;
		}
	public:
		Type type = Type::None;
		union
		{
#if WISDOM_WINDOWS
			HWND hwnd;
#endif // WISDOM_WINDOWS
#if WISDOM_UWP
			IUnknown* core_window;
#endif // WISDOM_UWP
#if WISDOM_LINUX
			struct
			{
				xcb_connection_t* connection;
				xcb_window_t window;
			}x11;
#endif // WISDOM_LINUX
#if WISDOM_MACOS
			CAMetalLayer* layer;
#endif // WISDOM_MACOS
		};
	};
}