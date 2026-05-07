#ifndef WIS_DX12_VIDEO_TYPES_HPP
#define WIS_DX12_VIDEO_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

struct ID3D12VideoDevice;
struct ID3D12VideoDecoder;
struct ID3D12VideoDecoderHeap;

namespace wis {
//----------------------------------------------------------------------------------------------------------------------
namespace detail {} // namespace detail

namespace impl {
struct DX12VideoDecodingExtensionImpl {
    DX12DeviceExtensionHeader header;
    WisVideoCodecFlags supported_codecs;
    ID3D12VideoDevice* device;
};
struct DX12VideoDecoderImpl {
    ID3D12VideoDecoder* decoder;
    ID3D12VideoDecoderHeap* decoder_heap;
};

} // namespace impl
} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20
#    include "dx12_video.cpp"

#endif // WISDOM_HEADER_ONLY
#endif // WIS_DX12_VIDEO_TYPES_HPP
