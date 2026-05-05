#ifndef WIS_DX12_VIDEO_CPP
#define WIS_DX12_VIDEO_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
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
    wis::com_ptr<ID3D12VideoDevice> video_device;
    HRESULT hr = device.device->QueryInterface(IID_ID3D12VideoDevice, video_device.put_void_unchecked());

    if (!wis::detail::succeeded(hr) || !video_device) {
        impl.supported_codecs = {};
        return wis::detail::dx_success;
    }

    D3D12_FEATURE_DATA_VIDEO_DECODE_PROFILE_COUNT profile_count_data{};
    hr = video_device->CheckFeatureSupport(
        D3D12_FEATURE_VIDEO_DECODE_PROFILE_COUNT,
        &profile_count_data,
        sizeof(profile_count_data)
    );

    if (!wis::detail::succeeded(hr) || profile_count_data.ProfileCount == 0) {
        impl.supported_codecs = {};
        return wis::detail::dx_success;
    }

    if (impl.device) {
        impl.device->Release();
    }

    impl.device = video_device.detach();
    return wis::detail::dx_success;
}

inline constexpr WisComponentBitDepth DX12GetBitDepth(WisDataFormat format) noexcept
{
    switch (format) {
    case WisDataFormatNV12:
        return WisComponentBitDepthBit8;
    case WisDataFormatP010:
        return WisComponentBitDepthBit10;
    case WisDataFormatP012:
        return WisComponentBitDepthBit12;
    case WisDataFormatP016:
        return WisComponentBitDepthBit16;
    default:
        return {};
    }
}
inline constexpr WisChromaSubsampling DX12GetChromaSubsampling(WisDataFormat format) noexcept
{
    switch (format) {
    case WisDataFormatNV12:
    case WisDataFormatP010:
    case WisDataFormatP012:
    case WisDataFormatP016:
        return WisChromaSubsamplingC420;
    default:
        return {};
    }
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
    D3D12_FEATURE_DATA_VIDEO_DECODE_SUPPORT decode_support{
        .Width = codec_desc->width,
        .Height = codec_desc->height,
        .DecodeFormat = wis::detail::DX12Convert(codec_desc->data_format),
    };

    switch (codec_desc->codec_profile) {
    case WisStdCodecProfileH264HighPredictive:
        return wis::detail::make_result<
            wis::detail::Func(),
            "H.264 High Predictive profile is not supported by DirectX 12 video decoding "
            "extension.">(E_NOTIMPL);
    case WisStdCodecProfileH264Main:
    case WisStdCodecProfileH264Baseline:
    case WisStdCodecProfileH264High:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_H264;
        break;
    case WisStdCodecProfileH265Main:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN;
        break;
    case WisStdCodecProfileH265Main10: {
        auto subsampling = wis::detail::DX12GetChromaSubsampling(codec_desc->data_format);
        switch (subsampling) {
        default:
        case WisChromaSubsamplingC420:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10;
            break;
        case WisChromaSubsamplingC422:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10_422;
            break;
        case WisChromaSubsamplingC444:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10_444;
            break;
        }
    } break;
    case WisStdCodecProfileH265Main12: {
        auto subsampling = wis::detail::DX12GetChromaSubsampling(codec_desc->data_format);
        switch (subsampling) {
        default:
        case WisChromaSubsamplingC420:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN12;
            break;
        case WisChromaSubsamplingC422:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN12_422;
            break;
        case WisChromaSubsamplingC444:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN12_444;
            break;
        }
    } break;
    case WisStdCodecProfileH265Main16:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN16;
        break;
    case WisStdCodecProfileH265FormatRangeExt:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10_EXT;
        break;
    case WisStdCodecProfileVP9Profile0:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_VP9;
        break;
    case WisStdCodecProfileVP9Profile2:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_VP9_10BIT_PROFILE2;
        break;
    case WisStdCodecProfileAV1Main:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_AV1_PROFILE0;
        break;
    case WisStdCodecProfileAV1High:
        decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_AV1_PROFILE1;
        break;
    case WisStdCodecProfileAV1Professional: {
        auto bit_depth = wis::detail::DX12GetBitDepth(codec_desc->data_format);
        auto subsampling = wis::detail::DX12GetChromaSubsampling(codec_desc->data_format);

        switch (bit_depth) {
        case WisComponentBitDepthBit8:
        case WisComponentBitDepthBit10:
            decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_AV1_PROFILE2;
            break;
        case WisComponentBitDepthBit12:
            if (subsampling == WisChromaSubsamplingC420) {
                decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_AV1_12BIT_PROFILE2_420;
            } else {
                decode_support.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_AV1_12BIT_PROFILE2;
            }
            break;
        default:
            return wis::detail::make_result<wis::detail::Func(), "Unsupported bit depth for AV1 Professional profile.">(
                E_NOTIMPL
            );
        }
    } break;
    default:
        return wis::detail::make_result<wis::detail::Func(), "Provided profile configuration is unsupported.">(
            E_NOTIMPL
        );
    }

    // clang-format off
    HRESULT hr = impl.device->CheckFeatureSupport(
                         D3D12_FEATURE_VIDEO_DECODE_SUPPORT, 
                         &decode_support, 
                         sizeof(decode_support));
    // clang-format on
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to query video decode support.">(hr);
    }

    if ((decode_support.SupportFlags & D3D12_VIDEO_DECODE_SUPPORT_FLAG_SUPPORTED) == 0) {
        return wis::detail::make_result<
            wis::detail::Func(),
            "Hardware does not support this specific decode configuration.">(E_NOTIMPL);
    }

    return wis::detail::dx_success;
}

#endif // WIS_DX12_VIDEO_CPP
