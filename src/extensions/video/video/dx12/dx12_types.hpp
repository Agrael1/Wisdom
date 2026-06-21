#ifndef WIS_DX12_VIDEO_TYPES_HPP
#define WIS_DX12_VIDEO_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

struct ID3D12VideoDevice;
struct ID3D12VideoDecoder;
struct ID3D12VideoDecoderHeap;
struct ID3D12VideoDecodeCommandList3;

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
    WisVideoCodecFlags codec;
};
struct DX12VideoDecodeCommandListImpl {
    ID3D12VideoDecodeCommandList3* command_list;
    ID3D12CommandAllocator* allocator;
    mutable uint32_t scratch_memory_size;
    mutable uint8_t* scratch_memory;
};

} // namespace impl
} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20
#    include "dx12_video.cpp"
#    include "dx12_video_list.cpp"

#endif // WISDOM_HEADER_ONLY
#endif // WIS_DX12_VIDEO_TYPES_HPP
