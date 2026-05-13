#ifndef WIS_DX12_VIDEO_COMMAND_LIST_CPP
#define WIS_DX12_VIDEO_COMMAND_LIST_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <video/dx12/detail/dx12_detail.hpp>
#include <video/generated/cpp_api.hpp>

#ifndef DX12SDKVER
#    include <directx/d3d12video.h>
#else
#    include <d3d12video.h>
#endif // DX12SDKVER
#include <dxva.h>
#include <bit>

namespace wis::detail {
inline DXVA_PicParams_AV1 ConvertToDXVAPicParamsAV1(
    const WisStdVideoDecodeAV1PictureInfo& params,
    const wis::detail::DX12AV1DecoderParameters& seq_hdr
) noexcept
{
    DXVA_PicParams_AV1 dxva{
        .width = seq_hdr.width,
        .height = seq_hdr.height,

        .max_width = seq_hdr.max_width,
        .max_height = seq_hdr.max_height,

        .bitdepth = seq_hdr.bitdepth,
        .seq_profile = seq_hdr.seq_profile,
        .coding = {.CodingParamToolFlags = seq_hdr.coding.CodingParamToolFlags},
        .format = {.FormatAndPictureInfoFlags = seq_hdr.format.FormatAndPictureInfoFlags},

        .order_hint_bits = seq_hdr.order_hint_bits,
    };

    return dxva;
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisDX12VideoDecodeCommandListBegin(const WisDX12VideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(self);
    auto hr = impl.command_list->Reset(impl.allocator);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command list for recording">(hr);
    }

    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisDX12VideoDecodeCommandListEnd(const WisDX12VideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(self);
    auto hr = impl.command_list->Close();
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command list for recording">(hr);
    }

    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12DestroyVideoDecodeCommandList(WisDX12VideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodeCommandListImpl>(self);
    if (impl.command_list) {
        impl.command_list->Release();
        impl.allocator->Release();
        impl.command_list = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12VideoDecodeCommandListDecodeFrame(
    const WisDX12VideoDecodeCommandList* command_list,
    const WisDX12VideoDecoder* decoder,
    const WisDX12VideoDecoderParameters* decoder_parameters,
    const WisDX12VideoDecodeInputDesc* input_desc
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(command_list);
    auto& decoder_impl = wis::from_handle_ref<const wis::impl::DX12VideoDecoderImpl>(decoder);
    auto& params = wis::from_handle_ref<const wis::impl::DX12VideoDecoderParametersImpl>(decoder_parameters);

    // TODO: Add frame header
    DXVA_PicParams_AV1 dxva_pic_params = wis::detail::ConvertToDXVAPicParamsAV1(
        {},
        *static_cast<const wis::detail::DX12AV1DecoderParameters*>(params.filler)
    );

    // TODO: Add WisDX12VideoDecodeOutputDesc to the function signature which will provide the
    // target texture (pSurface), subresource indexes, and color space conversion parameters.
    D3D12_VIDEO_DECODE_OUTPUT_STREAM_ARGUMENTS1 output_args{};

    // TODO: Add WisDX12VideoDecoderParameters to the function signature. WisDX12VideoDecoderParameters will
    // contain the sequence headers, picture info, and other codec-specific configuration. These standard (Std)
    // structs will need to be converted to DXVA structures (e.g., DXVA_PicParams_AV1) and passed in FrameArguments.
    D3D12_VIDEO_DECODE_INPUT_STREAM_ARGUMENTS input_args{
        .NumFrameArguments = 1,
        .FrameArguments = {{
            .Type = D3D12_VIDEO_DECODE_ARGUMENT_TYPE_PICTURE_PARAMETERS,
            .Size = sizeof(DXVA_PicParams_AV1),
            .pData = &dxva_pic_params,
        }},
        .ReferenceFrames =
            {
                .NumTexture2Ds = 0, // TODO: Fill from output descriptor and/or parameters
                .ppTexture2Ds = nullptr, // TODO: Fill from output descriptor and/or parameters
                .pSubresources = nullptr, // TODO: Fill from output descriptor and/or parameters
                .ppHeaps = nullptr, // TODO: Fill from output descriptor and/or parameters
            },
        .CompressedBitstream =
            {
                .pBuffer = std::bit_cast<ID3D12Resource*>(input_desc->bitstream_buffer),
                .Offset = input_desc->offset,
                .Size = input_desc->size,
            },
        .pHeap = decoder_impl.decoder_heap,
    };
    impl.command_list->DecodeFrame1(decoder_impl.decoder, &output_args, &input_args);
}

#endif // WIS_DX12_VIDEO_COMMAND_LIST_CPP
