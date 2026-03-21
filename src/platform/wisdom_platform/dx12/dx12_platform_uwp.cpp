#ifndef WIS_DX12_PLATFORM_UWP_CPP
#define WIS_DX12_PLATFORM_UWP_CPP

#if defined(WISDOM_DX12) && defined(WIS_PLATFORM_WIN32_PRESENT)
#include <wisdom_platform/generated/backend_api.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

namespace wis::detail {
inline WisResult DX12UWPExtensionInit(wis::DX12InstanceExtensionHeader* self, const wis::impl::DX12InstanceImpl& instance) noexcept
{
    auto& impl   = wis::from_handle_ref<wis::impl::DX12UWPExtensionImpl>(self);
    impl.factory = instance.factory;
    impl.factory->AddRef(); // AddRef factory to ensure it lives as long as the extension
    return wis::detail::dx_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisDX12InitUWPExtension(WisDX12UWPExtension* self)
{
    new (self) wis::impl::DX12UWPExtensionImpl{
        .header  = { &wis::detail::DX12UWPExtensionInit },
        .factory = nullptr,
    };
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisDX12DestroyUWPExtension(WisDX12UWPExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12UWPExtensionImpl>(self);
    if (impl.factory) {
        impl.factory->Release();
        impl.factory = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API WisResult wisDX12UWPExtensionCreateSurface(WisDX12UWPExtension*    self,
                                                                            const WisUWPWindowDesc* info,
                                                                            WisDX12Surface*         surface)
{
    new (surface) wis::impl::DX12SurfaceImpl{
        .surface = info->core_window,
    };
    return wis::detail::dx_success;
}

#endif // defined(WISDOM_DX12) && defined(WIS_PLATFORM_WIN32_PRESENT)

#endif // WIS_DX12_PLATFORM_UWP_CPP
