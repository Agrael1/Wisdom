#ifndef WIS_DX12_VIDEO_COMMAND_LIST_CPP
#define WIS_DX12_VIDEO_COMMAND_LIST_CPP

#include <wisdom/dx12/detail/dx12_detail.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <video/generated/cpp_api.hpp>

#ifndef DX12SDKVER
#    include <directx/d3d12video.h>
#else
#    include <d3d12video.h>
#endif // DX12SDKVER
#include <dxva.h>
#include <bit>

// Barrier helper functions moved to wis::detail in dx12_detail.hpp

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
        delete[] impl.scratch_memory;
        impl.command_list = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12VideoDecodeCommandListInsertBarriers(
    const WisDX12VideoDecodeCommandList* self,
    const WisDX12BarrierGroup* barriers
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(self);
    wis::detail::DX12InsertBarriers(impl, impl.command_list, barriers, WisCommandQueueTypeVideoDecode);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12VideoDecodeCommandListDecodeFrame(
    const WisDX12VideoDecodeCommandList* command_list,
    const WisDX12VideoDecoder* decoder,
    const WisDX12VideoDecoderParameters* parameters,
    const WisDX12VideoDecodeInputDesc* input_desc,
    const WisDX12VideoDecodeOutputDesc* output_desc,
    const WisDX12VideoDecodePictureDesc* picture_desc
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(command_list);
    auto& decoder_impl = wis::from_handle_ref<const wis::impl::DX12VideoDecoderImpl>(decoder);

    // Output stream arguments
    D3D12_VIDEO_DECODE_OUTPUT_STREAM_ARGUMENTS1 output_args{
        .pOutputTexture2D = std::bit_cast<ID3D12Resource*>(output_desc->output_texture),
        .OutputSubresource = output_desc->subresource,
    };

    // Codec-specific frame arguments
    D3D12_VIDEO_DECODE_FRAME_ARGUMENT frame_args[8]{};
    uint32_t frame_arg_count = 0;

    if (picture_desc->codec >= WisStdCodecProfileAV1Main && picture_desc->codec <= WisStdCodecProfileAV1Professional) {
        // AV1: no standard DX12 frame arguments defined yet via D3D12_VIDEO_DECODE_FRAME_ARGUMENT
        // The decode parameters are passed via the std picture info struct directly
        (void)picture_desc->av1_picture_info;
    } else if (
        picture_desc->codec >= WisStdCodecProfileH265Main && picture_desc->codec <= WisStdCodecProfileH265FormatRangeExt
    ) {
        // H.265 frame arguments if needed
        (void)picture_desc->h265_picture_info;
    }

    // Input stream arguments
    D3D12_VIDEO_DECODE_INPUT_STREAM_ARGUMENTS input_args{
        .ReferenceFrames =
            {
                .NumTexture2Ds = 0,
                .ppTexture2Ds = nullptr,
                .pSubresources = nullptr,
                .ppHeaps = nullptr,
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
