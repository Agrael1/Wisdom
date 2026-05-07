// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_CPP_API_HPP
#define WISDOM_VIDEO_CPP_API_HPP
#ifndef __cplusplus
#    error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include "c_api.h"
#include "wisdom_exports.h"

namespace wis {

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Standard codec profiles. Used to specify the profile of a video codec
 * implementation.
 *
 * */
enum class StdCodecProfile {
    /**
     * @brief H.264 Baseline profile. Suitable for low-complexity applications such as video conferencing and mobile
     * streaming.
     * */
    H264Baseline = 0,
    /**
     * @brief H.264 Main profile. Offers better compression efficiency than Baseline, suitable for standard-definition
     * video streaming and broadcast.
     * */
    H264Main = 1,
    /**
     * @brief H.264 High profile. Provides the best compression efficiency among H.264 profiles, suitable for
     * high-definition video streaming and Blu-ray discs.
     * */
    H264High = 2,
    /**
     * @brief H.264 High Predictive profile. An extension of the High profile that adds support for additional features,
     * such as improved error resilience and better performance.
     * */
    H264HighPredictive = 3,
    H265Main = 32, ///< H.265 Main profile. Suitable for standard-definition video streaming and broadcast.
    /**
     * @brief H.265 Main 10 profile. Adds support for 10-bit video, providing better color depth and improved
     * compression efficiency compared to the Main profile.
     * */
    H265Main10 = 33,
    /**
     * @brief H.265 Main 12 profile. Adds support for 12-bit video, offering even greater color depth and improved
     * compression efficiency compared to the Main 10 profile.
     * */
    H265Main12 = 34,
    /**
     * @brief H.265 Main 16 profile. Adds support for 16-bit video, providing the highest color depth and best
     * compression efficiency among the Main profiles, suitable for professional video production and post-production
     * workflows.
     * */
    H265Main16 = 35,
    /**
     * @brief H.265 Format Range Extensions profile. Adds support for additional features such as higher bit depths,
     * wider color gamut, and improved compression efficiency, suitable for high-quality video streaming and broadcast.
     * */
    H265FormatRangeExt = 36,
    /**
     * @brief AV1 Main profile. Suitable for a wide range of applications, including web streaming and video
     * conferencing.
     * */
    AV1Main = 64,
    /**
     * @brief AV1 High profile. Adds support for additional features such as higher bit depths and improved compression
     * efficiency, suitable for high-quality video streaming and broadcast.
     * */
    AV1High = 65,
    /**
     * @brief AV1 Professional profile. Designed for professional video production and post-production workflows,
     * offering support for high bit depths, wide color gamut, and advanced features.
     * */
    AV1Professional = 66,
    /**
     * @brief VP9 Profile 0. Supports 8-bit video with 4:2:0 chroma subsampling, suitable for web streaming and
     * general-purpose video applications.
     * */
    VP9Profile0 = 96,
    /**
     * @brief VP9 Profile 1. Adds support for 8-bit video with 4:2:2 and 4:4:4 chroma subsampling, suitable for
     * professional video production and post-production workflows that require higher color fidelity.
     * */
    VP9Profile1 = 97,
    VP9Profile2 = 98, ///< VP9 Profile 2. Adds support for 10-bit and 12-bit video with 4:2:0 chroma subsampling.
    /**
     * @brief VP9 Profile 3. Adds support for 10-bit and 12-bit video with 4:2:2 and 4:4:4 chroma subsampling.
     * */
    VP9Profile3 = 99,
};

/**
 * @brief Provided by Wisdom 0.7.1. Video codec flags. Used to request and check supported codecs.
 *
 * */
enum class VideoCodecFlags : uint32_t {
    None = 0, ///< No video is requested. The extension will not initialize.
    H264 = (1u << 0), ///< H.264 video codec.
    H265 = (1u << 1), ///< H.265 video codec.
    AV1 = (1u << 2), ///< AV1 video codec.
    VP9 = (1u << 3), ///< VP9 video codec.
};
WISDOM_DEFINE_ENUM_OPERATORS(VideoCodecFlags)

/**
 * @brief Provided by Wisdom 0.7.1. Bit depth flags for video components. Used to specify supported bit depths for video
 * decoding.
 *
 * */
enum class ComponentBitDepth : uint32_t {
    None = 0, ///< No bit depth is specified.
    Bit8 = (1u << 0), ///< 8-bit video component.
    Bit10 = (1u << 1), ///< 10-bit video component.
    Bit12 = (1u << 2), ///< 12-bit video component.
    Bit16 = (1u << 3), ///< 16-bit video component.
};
WISDOM_DEFINE_ENUM_OPERATORS(ComponentBitDepth)

/**
 * @brief Provided by Wisdom 0.7.1. Chroma subsampling flags for video components. Used to specify supported chroma
 * subsampling formats for video decoding.
 *
 * */
enum class ChromaSubsampling : uint32_t {
    None = 0, ///< No chroma subsampling is specified.
    C420 = (1u << 1), ///< 4:2:0 chroma subsampling format.
    C422 = (1u << 2), ///< 4:2:2 chroma subsampling format.
    C444 = (1u << 3), ///< 4:4:4 chroma subsampling format.
};
WISDOM_DEFINE_ENUM_OPERATORS(ChromaSubsampling)

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Information about a supported video codec.
 *
 * */
struct VideoCodecDesc {
    wis::StdCodecProfile codec_profile; ///< The video codec to query capabilities for.
    /**
     * @brief The data format of the video frames for this codec. This field is used to specify the expected format of
     * the video frames that will be decoded using this codec, and can influence the supported bit depths and chroma
     * subsampling formats.
     * */
    wis::DataFormat data_format;
    std::uint32_t width; ///< Max width of the video frame in pixels.
    std::uint32_t height; ///< Max height of the video frame in pixels.
};

/**
 * @brief Provided by Wisdom 0.7.1. Information about a video decode operation.
 *
 * */
struct VideoDecoderDesc {
    std::uint32_t max_width; ///< Max width of the video frame in pixels.
    std::uint32_t max_height; ///< Max height of the video frame in pixels.
    /**
     * @brief The data format of the output video frames. This field specifies the expected format of the decoded video
     * frames that will be produced by the video decoder, and can influence the supported bit depths and chroma
     * subsampling formats.
     * */
    wis::DataFormat image_format;
    /**
     * @brief The video codec profile that the decoder will use for decoding. This field specifies the profile of the
     * video codec that the decoder will use for decoding video frames, and can influence the supported bit depths and
     * chroma subsampling formats.
     * */
    wis::StdCodecProfile codec_profile;
    /**
     * @brief The number of decode buffers that the decoder will use for decoding video frames.
     * */
    std::uint32_t decode_picture_buffer_count;
};

} // namespace wis

#ifdef WISDOM_DX12
#    include <video/dx12/dx12_types.hpp>

namespace wis {
struct DX12VideoDecoderDeleter {
    void operator()(WisDX12VideoDecoder* handle) noexcept { ::wisDX12DestroyVideoDecoder(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video decoder. Represents a video decoder instance that can be used to
 * decode video frames.
 *
 * */
class DX12VideoDecoder
    : public wis::impl::Implements<wis::impl::DX12VideoDecoderImpl, WisDX12VideoDecoder, wis::DX12VideoDecoderDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct DX12VideoDecodingExtensionDeleter {
    void operator()(WisDX12VideoDecodingExtension* handle) noexcept { ::wisDX12DestroyVideoDecodingExtension(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
class DX12VideoDecodingExtension : public wis::impl::Implements<
                                       wis::impl::DX12VideoDecodingExtensionImpl,
                                       WisDX12VideoDecodingExtension,
                                       wis::DX12VideoDecodingExtensionDeleter>
{
public:
    DX12VideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept
        : ImplType(wis::in_place)
    {
        ::wisDX12InitVideoDecodingExtension(GetStorage(), static_cast<WisVideoCodecFlags>(request_codecs));
    }
    // Operator & overload
    wis::DX12DeviceExtensionHeader* operator&() noexcept { return &GetMutableInternal().header; }

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
     * @param codec_desc Information about the video codec to query capabilities for. The 'codec' field should specify
     * the codec to check, and the function will fill in the supported bit depths and chroma subsampling formats for
     * that codec.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result QueryCodecCaps(const wis::VideoCodecDesc& codec_desc) noexcept
    {
        const WisResult wis_result = ::wisDX12VideoDecodingExtensionQueryCodecCaps(
            &_impl_storage,
            reinterpret_cast<const WisVideoCodecDesc*>(&codec_desc)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Creates a video decoder instance.
     * @param decoder_desc Information about the video decoder to create.
     * @param out_result denoting the outcome of operation.
     * @return video_decoder Output parameter that holds the created video decoder handle if the operation is
     * successful.
     *
     * */
    WIS_NODISCARD inline wis::DX12VideoDecoder CreateDecoder(
        const wis::VideoDecoderDesc& decoder_desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12VideoDecoder video_decoder{};
        const WisResult wis_result = ::wisDX12VideoDecodingExtensionCreateDecoder(
            &_impl_storage,
            reinterpret_cast<const WisVideoDecoderDesc*>(&decoder_desc),
            video_decoder.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return video_decoder;
    }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <video/vulkan/vk_types.hpp>

namespace wis {
struct VKVideoDecoderDeleter {
    void operator()(WisVKVideoDecoder* handle) noexcept { ::wisVKDestroyVideoDecoder(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video decoder. Represents a video decoder instance that can be used to
 * decode video frames.
 *
 * */
class VKVideoDecoder
    : public wis::impl::Implements<wis::impl::VKVideoDecoderImpl, WisVKVideoDecoder, wis::VKVideoDecoderDeleter>
{
public:
    using ImplType::ImplType;

public:
};

struct VKVideoDecodingExtensionDeleter {
    void operator()(WisVKVideoDecodingExtension* handle) noexcept { ::wisVKDestroyVideoDecodingExtension(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
class VKVideoDecodingExtension : public wis::impl::Implements<
                                     wis::impl::VKVideoDecodingExtensionImpl,
                                     WisVKVideoDecodingExtension,
                                     wis::VKVideoDecodingExtensionDeleter>
{
public:
    VKVideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept
        : ImplType(wis::in_place)
    {
        ::wisVKInitVideoDecodingExtension(GetStorage(), static_cast<WisVideoCodecFlags>(request_codecs));
    }
    // Operator & overload
    wis::VKDeviceExtensionHeader* operator&() noexcept { return &GetMutableInternal().header; }

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
     * @param codec_desc Information about the video codec to query capabilities for. The 'codec' field should specify
     * the codec to check, and the function will fill in the supported bit depths and chroma subsampling formats for
     * that codec.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result QueryCodecCaps(const wis::VideoCodecDesc& codec_desc) noexcept
    {
        const WisResult wis_result = ::wisVKVideoDecodingExtensionQueryCodecCaps(
            &_impl_storage,
            reinterpret_cast<const WisVideoCodecDesc*>(&codec_desc)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Creates a video decoder instance.
     * @param decoder_desc Information about the video decoder to create.
     * @param out_result denoting the outcome of operation.
     * @return video_decoder Output parameter that holds the created video decoder handle if the operation is
     * successful.
     *
     * */
    WIS_NODISCARD inline wis::VKVideoDecoder CreateDecoder(
        const wis::VideoDecoderDesc& decoder_desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKVideoDecoder video_decoder{};
        const WisResult wis_result = ::wisVKVideoDecodingExtensionCreateDecoder(
            &_impl_storage,
            reinterpret_cast<const WisVideoDecoderDesc*>(&decoder_desc),
            video_decoder.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return video_decoder;
    }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_VIDEO_CPP_API_HPP
