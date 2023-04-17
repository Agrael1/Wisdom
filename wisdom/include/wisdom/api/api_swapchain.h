#pragma once
#include <wisdom/util/winsetup.h>
#include <wisdom/api/api_common.h>

namespace wis
{
	struct SwapchainOptions
	{
		uint32_t width = 1280;
		uint32_t height = 720;
		uint32_t frame_count = 2;
        DataFormat format = DataFormat::b8g8r8a8_unorm;
        bool stereo = false;
	};

    // TODO: vulkan and winrt
    struct SurfaceParameters
    {
        enum class Type {
            Win32,
            WinRT,
        };
    public:
        explicit SurfaceParameters(HWND hwnd)
            :type(Type::Win32), hwnd(hwnd)
        {}
        explicit SurfaceParameters(IUnknown* core_window)
            :type(Type::WinRT), core_window(core_window)
        {}
    public:
        Type type;
        union
        {
            HWND hwnd;
            IUnknown* core_window;
        };
    };
}