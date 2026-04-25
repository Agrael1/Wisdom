#ifndef WIS_DX12_RAYTRACING_CPP
#define WIS_DX12_RAYTRACING_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <raytracing/generated/cpp_api.hpp>

namespace wis::detail {
inline WisResult DX12RaytracingExtensionInit(
    wis::DX12DeviceExtensionHeader* self,
    const wis::impl::DX12DeviceImpl& device
) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);
    impl.device = device.device;
    impl.device->AddRef(); // AddRef factory to ensure it lives as long as the extension
    return wis::detail::dx_success;
}
} // namespace wis::detail

WIS_EXTERN_C WISDOM_RAYTRACING_API void wisDX12InitRaytracingExtension(WisDX12RaytracingExtension* self)
{
    new (self) wis::impl::DX12RaytracingExtensionImpl{
        .header = {&wis::detail::DX12RaytracingExtensionInit},
    };
}

WIS_EXTERN_C WISDOM_RAYTRACING_API void wisDX12DestroyRaytracingExtension(WisDX12RaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);
    if (impl.device) {
        impl.device->Release();
        impl.device = nullptr;
    }
    impl.header = {nullptr};
}

WIS_EXTERN_C WISDOM_RAYTRACING_API bool wisDX12RaytracingExtensionSupported(WisDX12RaytracingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12RaytracingExtensionImpl>(self);
    if (!impl.device) {
        return false;
    }
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5{};
    HRESULT hr = impl.device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5));
    if (FAILED(hr)) {
        return false;
    }
    return options5.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
}

#endif
