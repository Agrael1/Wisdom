#ifndef WIS_DX12_VIDEO_CPP
#define WIS_DX12_VIDEO_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/dx12/detail/dx12_detail.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <video/generated/cpp_api.hpp>
#include <video/dx12/detail/dx12_video_parameters.hpp>

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

inline constexpr WisResult DX12GetDecodeProfile(
    WisStdCodecProfile codec_profile,
    WisDataFormat data_format,
    GUID* decode_profile
) noexcept
{
    switch (codec_profile) {
    case WisStdCodecProfileH264HighPredictive:
        return wis::detail::make_result<
            wis::detail::Func(),
            "H.264 High Predictive profile is not supported by DirectX 12 video decoding "
            "extension.">(E_NOTIMPL);
    case WisStdCodecProfileH264Main:
    case WisStdCodecProfileH264Baseline:
    case WisStdCodecProfileH264High:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_H264;
        break;
    case WisStdCodecProfileH265Main:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN;
        break;
    case WisStdCodecProfileH265Main10: {
        auto subsampling = wis::detail::DX12GetChromaSubsampling(data_format);
        switch (subsampling) {
        default:
        case WisChromaSubsamplingC420:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10;
            break;
        case WisChromaSubsamplingC422:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10_422;
            break;
        case WisChromaSubsamplingC444:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10_444;
            break;
        }
    } break;
    case WisStdCodecProfileH265Main12: {
        auto subsampling = wis::detail::DX12GetChromaSubsampling(data_format);
        switch (subsampling) {
        default:
        case WisChromaSubsamplingC420:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN12;
            break;
        case WisChromaSubsamplingC422:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN12_422;
            break;
        case WisChromaSubsamplingC444:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN12_444;
            break;
        }
    } break;
    case WisStdCodecProfileH265Main16:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN16;
        break;
    case WisStdCodecProfileH265FormatRangeExt:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN10_EXT;
        break;
    case WisStdCodecProfileVP9Profile0:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_VP9;
        break;
    case WisStdCodecProfileVP9Profile2:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_VP9_10BIT_PROFILE2;
        break;
    case WisStdCodecProfileAV1Main:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_AV1_PROFILE0;
        break;
    case WisStdCodecProfileAV1High:
        *decode_profile = D3D12_VIDEO_DECODE_PROFILE_AV1_PROFILE1;
        break;
    case WisStdCodecProfileAV1Professional: {
        auto bit_depth = wis::detail::DX12GetBitDepth(data_format);
        auto subsampling = wis::detail::DX12GetChromaSubsampling(data_format);

        switch (bit_depth) {
        case WisComponentBitDepthBit8:
        case WisComponentBitDepthBit10:
            *decode_profile = D3D12_VIDEO_DECODE_PROFILE_AV1_PROFILE2;
            break;
        case WisComponentBitDepthBit12:
            if (subsampling == WisChromaSubsamplingC420) {
                *decode_profile = D3D12_VIDEO_DECODE_PROFILE_AV1_12BIT_PROFILE2_420;
            } else {
                *decode_profile = D3D12_VIDEO_DECODE_PROFILE_AV1_12BIT_PROFILE2;
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
    return wis::detail::dx_success;
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12DestroyVideoDecodingExtension(WisDX12VideoDecodingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodingExtensionImpl>(self);
    if (impl.device) {
        impl.device->Release();
        impl.device = nullptr;
    }
    impl.header = {nullptr};
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult
wisDX12VideoDecodingExtensionQueryCodecCaps(WisDX12VideoDecodingExtension* self, const WisVideoCodecDesc* codec_desc)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodingExtensionImpl>(self);
    D3D12_FEATURE_DATA_VIDEO_DECODE_SUPPORT decode_support{
        .Width = codec_desc->width,
        .Height = codec_desc->height,
        .DecodeFormat = wis::detail::DX12Convert(codec_desc->image_format),
    };

    WisResult decode_profile_result = wis::detail::DX12GetDecodeProfile(
        codec_desc->codec_profile,
        codec_desc->image_format,
        &decode_support.Configuration.DecodeProfile
    );

    if (!wis::detail::succeeded(decode_profile_result.platform_code)) {
        return decode_profile_result;
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

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionCreateDecoder(
    const WisDX12VideoDecodingExtension* self,
    const WisVideoDecoderDesc* decoder_desc,
    WisDX12VideoDecoder* video_decoder
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodingExtensionImpl>(self);

    D3D12_VIDEO_DECODER_DESC dx_decoder_desc{};
    WisResult decode_profile_result = wis::detail::DX12GetDecodeProfile(
        decoder_desc->codec_profile,
        decoder_desc->image_format,
        &dx_decoder_desc.Configuration.DecodeProfile
    );
    if (!wis::detail::succeeded(decode_profile_result.platform_code)) {
        return decode_profile_result;
    }

    wis::com_ptr<ID3D12VideoDecoder> decoder;
    auto hr = impl.device->CreateVideoDecoder(&dx_decoder_desc, IID_ID3D12VideoDecoder, decoder.put_void_unchecked());
    if (!wis::detail::succeeded(hr) || !decoder) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create video decoder.">(hr);
    }

    // Create heap for decoder.
    D3D12_VIDEO_DECODER_HEAP_DESC heap_desc{
        .Configuration = dx_decoder_desc.Configuration,
        .DecodeWidth = decoder_desc->width,
        .DecodeHeight = decoder_desc->height,
        .Format = wis::detail::DX12Convert(decoder_desc->image_format),
        .MaxDecodePictureBufferCount = decoder_desc->decode_picture_buffer_count,
    };
    wis::com_ptr<ID3D12VideoDecoderHeap> decoder_heap;
    hr = impl.device->CreateVideoDecoderHeap(&heap_desc, IID_ID3D12VideoDecoderHeap, decoder_heap.put_void_unchecked());
    if (!wis::detail::succeeded(hr) || !decoder) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create video decoder heap.">(hr);
    }

    new (video_decoder) wis::impl::DX12VideoDecoderImpl{
        .decoder = decoder.detach(),
        .decoder_heap = decoder_heap.detach(),
        .codec = static_cast<WisVideoCodecFlags>(1 << (decoder_desc->codec_profile / 32)),
    };
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12DestroyVideoDecoder(WisDX12VideoDecoder* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecoderImpl>(self);
    if (impl.decoder) {
        impl.decoder_heap->Release();
        impl.decoder->Release();
        impl.decoder = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionCreateParameters(
    const WisDX12VideoDecodingExtension* self,
    const WisDX12VideoDecoder* decoder,
    const WisVideoDecodeParameterDesc* params,
    WisDX12VideoDecoderParameters* decoder_parameters
)
{
    auto& decoder_impl = wis::from_handle_ref<const wis::impl::DX12VideoDecoderImpl>(decoder);
    void* allocation = nullptr;

    switch (decoder_impl.codec) {
    case WisVideoCodecFlagsAV1: {
        auto& sequence_header = params->av1->sequence_header;
        std::unique_ptr<wis::detail::DX12AV1DecoderParameters>
            params = wis::make_unique<wis::detail::DX12AV1DecoderParameters>(sequence_header);
        if (!params) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for decoder parameters.">(
                E_OUTOFMEMORY
            );
        }

        allocation = params.release();
        break;
    }

    case WisVideoCodecFlagsH265: {
        std::unique_ptr<wis::detail::DX12H265DecoderParameters>
            h265_params = wis::make_unique<wis::detail::DX12H265DecoderParameters>(*params->h265);
        if (!h265_params) {
            return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for decoder parameters.">(
                E_OUTOFMEMORY
            );
        }
        allocation = h265_params.release();
        break;
    }

    default:
        return wis::detail::make_result<
            wis::detail::Func(),
            "Creating video decoder parameters for the specified codec is not yet implemented.">(E_NOTIMPL);
    }

    new (decoder_parameters) wis::impl::DX12VideoDecoderParametersImpl{
        .filler = allocation,
        .codec = decoder_impl.codec,
    };
    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12DestroyVideoDecoderParameters(WisDX12VideoDecoderParameters* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecoderParametersImpl>(self);
    if (impl.filler) {
        switch (impl.codec) {
        case WisVideoCodecFlagsAV1:
            delete static_cast<wis::detail::DX12AV1DecoderParameters*>(impl.filler);
            break;
        case WisVideoCodecFlagsH265:
            delete static_cast<wis::detail::DX12H265DecoderParameters*>(impl.filler);
            break;
        default:
            break;
        }
    }
    impl.filler = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionCreateCommandList(
    WisDX12VideoDecodingExtension* self,
    const WisDX12CommandAllocator* command_allocator,
    WisDX12VideoDecodeCommandList* command_list
)
{
    auto& [allocator, device, type] = wis::from_handle_ref<const wis::impl::DX12CommandAllocatorImpl>(
        command_allocator
    );
    if (type != WisCommandQueueTypeVideoDecode) {
        return wis::detail::make_result<wis::detail::Func(), "Provided command allocator is not for video decoding.">(
            E_INVALIDARG
        );
    }

    wis::com_ptr<ID3D12VideoDecodeCommandList3> dx_command_list;
    auto hr = device->CreateCommandList1(
        0,
        wis::detail::DX12Convert(type),
        D3D12_COMMAND_LIST_FLAG_NONE,
        IID_ID3D12VideoDecodeCommandList3,
        dx_command_list.put_void_unchecked()
    );

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create command list">(hr);
    }

    auto& internal = *new (command_list) wis::impl::DX12VideoDecodeCommandListImpl{
        .command_list = dx_command_list.detach(),
        .allocator = allocator,
    };
    internal.allocator->AddRef();

    return wis::detail::dx_success;
}

#endif // WIS_DX12_VIDEO_CPP
