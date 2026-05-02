#ifndef WIS_DX12_VIDEO_CPP
#define WIS_DX12_VIDEO_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <video/generated/cpp_api.hpp>

#ifndef DX12SDKVER
#    include <directx/d3d12video.h>
#else
#    include <d3d12video.h>
#endif // DX12SDKVER

namespace wis::detail {
inline WisResult DX12VideoDecodingExtensionInit(
    wis::DX12DeviceExtensionHeader* self,
    const wis::impl::DX12DeviceImpl& device
) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodingExtensionImpl>(self);
    if (!impl.supported_codecs) {
        return wis::detail::dx_success; // Nothing requested
    }
    wis::com_ptr<ID3D12VideoDevice> videoDevice;
    HRESULT hr = impl.device->QueryInterface(IID_ID3D12VideoDevice, videoDevice.put_void_unchecked());

    if (!wis::detail::succeeded(hr) || !videoDevice) {
        impl.supported_codecs = {};
        return wis::detail::dx_success;
    }

    D3D12_FEATURE_DATA_VIDEO_DECODE_PROFILE_COUNT profileCountData = {};
    profileCountData.NodeIndex = 0; // 0 for single-GPU setups

    hr = videoDevice->CheckFeatureSupport(
        D3D12_FEATURE_VIDEO_DECODE_PROFILE_COUNT,
        &profileCountData,
        sizeof(profileCountData)
    );

    if (SUCCEEDED(hr) && profileCountData.ProfileCount > 0) {
        impl.supported_codecs = {};
        return wis::detail::dx_success;
    }

    if (impl.device) {
        impl.device->Release();
    }

    impl.device = device.device;
    impl.device->AddRef(); // AddRef factory to ensure it lives as long as the extension
    return wis::detail::dx_success;
}
} // namespace wis::detail

WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12InitVideoDecodingExtension(
    WisDX12VideoDecodingExtension* self,
    WisVideoCodecFlags request_codecs
)
{
    new (self) wis::impl::DX12VideoDecodingExtensionImpl{
        .header = {&wis::detail::DX12VideoDecodingExtensionInit},
        .supported_codecs = request_codecs,
        .device = nullptr
    };
}

WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12DestroyVideoDecodingExtension(WisDX12VideoDecodingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodingExtensionImpl>(self);
    if (impl.device) {
        impl.device->Release();
        impl.device = nullptr;
    }
    impl.header = {nullptr};
}

WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionQueryCodecCaps(
    WisDX12VideoDecodingExtension* self,
    const WisVideoCodecDesc* codec_desc,
    WisVideoDecodeInfo* decode_info
)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodingExtensionImpl>(self);

    switch (codec_desc->codec_profile) {
    case WisStdCodecProfileH264Baseline: {

    } break;
    }

    return wis::detail::dx_success;
}

#endif // WIS_DX12_VIDEO_CPP
