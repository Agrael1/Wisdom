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
     * @brief H.265 Format Range Extensions profile. Adds support for additional features such as higher bit depths,
     * wider color gamut, and improved compression efficiency, suitable for high-quality video streaming and broadcast.
     * */
    H265FormatRangeExt = 34,
    /**
     * @brief H.265 Screen Content Coding Extensions profile. Designed for screen content coding, offering improved
     * compression efficiency for computer-generated content such as screen sharing and remote desktop applications.
     * */
    H265SCCExt = 35,
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
     * @brief Supported bit depths for this codec, represented as a bitmask of ComponentBitDepth flags.
     * */
    wis::ComponentBitDepth bit_depth;
    /**
     * @brief Supported chroma subsampling formats for this codec, represented as a bitmask of ChromaSubsampling flags.
     * */
    wis::ChromaSubsampling chroma_subsampling;
};

/**
 * @brief Provided by Wisdom 0.7.1. Information about a video decode operation.
 *
 * */
struct VideoDecodeInfo {
    std::uint32_t max_width; ///< Max width of the video frame in pixels.
    std::uint32_t max_height; ///< Max height of the video frame in pixels.
};

} // namespace wis

#ifdef WISDOM_DX12
#    include <video/dx12/dx12_types.hpp>

namespace wis {
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
     * @param out_result denoting the outcome of operation.
     * @return decode_info Information about the video decode capabilities of the current device, including maximum
     * supported video frame dimensions.
     *
     * */
    WIS_NODISCARD inline wis::VideoDecodeInfo QueryCodecCaps(
        const wis::VideoCodecDesc& codec_desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::VideoDecodeInfo decode_info{};
        const WisResult wis_result = ::wisDX12VideoDecodingExtensionQueryCodecCaps(
            &_impl_storage,
            reinterpret_cast<const WisVideoCodecDesc*>(&codec_desc),
            reinterpret_cast<WisVideoDecodeInfo*>(&decode_info)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return decode_info;
    }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <video/vulkan/vk_types.hpp>

namespace wis {
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
     * @param out_result denoting the outcome of operation.
     * @return decode_info Information about the video decode capabilities of the current device, including maximum
     * supported video frame dimensions.
     *
     * */
    WIS_NODISCARD inline wis::VideoDecodeInfo QueryCodecCaps(
        const wis::VideoCodecDesc& codec_desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::VideoDecodeInfo decode_info{};
        const WisResult wis_result = ::wisVKVideoDecodingExtensionQueryCodecCaps(
            &_impl_storage,
            reinterpret_cast<const WisVideoCodecDesc*>(&codec_desc),
            reinterpret_cast<WisVideoDecodeInfo*>(&decode_info)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return decode_info;
    }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_VIDEO_CPP_API_HPP
