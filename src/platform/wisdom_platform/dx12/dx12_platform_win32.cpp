#ifndef WIS_DX12_PLATFORM_WIN32_CPP
#define WIS_DX12_PLATFORM_WIN32_CPP

#if defined(WISDOM_DX12) && defined(WIS_PLATFORM_WIN32_PRESENT)
#include <wisdom_platform/generated/cpp_api.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

namespace wis::detail {
inline WisResult DX12Win32ExtensionInit(wis::DX12InstanceExtensionHeader* self, const wis::impl::DX12InstanceImpl& instance) noexcept
{
    auto& impl   = wis::from_handle_ref<wis::impl::DX12Win32ExtensionImpl>(self);
    impl.factory = instance.factory;
    impl.factory->AddRef(); // AddRef factory to ensure it lives as long as the extension
    return wis::detail::dx_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisDX12InitWin32Extension(WisDX12Win32Extension* self)
{
    new (self) wis::impl::DX12Win32ExtensionImpl{
        .header  = { &wis::detail::DX12Win32ExtensionInit },
        .factory = nullptr,
    };
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisDX12DestroyWin32Extension(WisDX12Win32Extension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12Win32ExtensionImpl>(self);
    if (impl.factory) {
        impl.factory->Release();
        impl.factory = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API WisResult wisDX12Win32ExtensionCreateSurface(WisDX12Win32Extension*    self,
                                                                              const WisWin32WindowDesc* info,
                                                                              WisDX12Surface*           surface)
{
    new (surface) wis::impl::DX12SurfaceImpl{
        .surface = info->hwnd,
        .uwp     = false,
    };
    return wis::detail::dx_success;
}

#endif // defined(WISDOM_DX12) && defined(WIS_PLATFORM_WIN32_PRESENT)

#endif // WIS_DX12_PLATFORM_WIN32_CPP
