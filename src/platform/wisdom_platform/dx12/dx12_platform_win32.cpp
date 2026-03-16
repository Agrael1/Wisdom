#ifndef WIS_DX12_PLATFORM_WIN32_CPP
#define WIS_DX12_PLATFORM_WIN32_CPP

#if defined(WISDOM_DX12) && defined(WIS_PLATFORM_WIN32_PRESENT)

#ifndef WIS_USE_PLATFORM_WIN32
#define WIS_USE_PLATFORM_WIN32 1
#endif
#include <wisdom_platform/generated/cpp_platform_api.hpp>

WIS_EXTERN_C WISDOM_PLATFORM_API void wisDX12DestroyWin32Extension(WisDX12Win32Extension* self)
{
    (void)self;
}


#endif // defined(WISDOM_DX12) && defined(WIS_PLATFORM_WIN32_PRESENT)

#endif // WIS_DX12_PLATFORM_WIN32_CPP
