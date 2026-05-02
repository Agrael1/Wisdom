// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_C_API_H
#define WISDOM_VIDEO_C_API_H
#include <wisdom/generated/c_api.h>
#include "wisdom_exports.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Standard codec profiles. Used to specify the profile of a video codec
 * implementation.
 *
 * */
typedef enum WisStdCodecProfile {
    /**
     * @brief H.264 Baseline profile. Suitable for low-complexity applications such as video conferencing and mobile
     * streaming.
     * */
    WisStdCodecProfileH264Baseline = 0,
    /**
     * @brief H.264 Main profile. Offers better compression efficiency than Baseline, suitable for standard-definition
     * video streaming and broadcast.
     * */
    WisStdCodecProfileH264Main = 1,
    /**
     * @brief H.264 High profile. Provides the best compression efficiency among H.264 profiles, suitable for
     * high-definition video streaming and Blu-ray discs.
     * */
    WisStdCodecProfileH264High = 2,
    /**
     * @brief H.264 High Predictive profile. An extension of the High profile that adds support for additional features,
     * such as improved error resilience and better performance.
     * */
    WisStdCodecProfileH264HighPredictive = 3,
    /**
     * @brief H.265 Main profile. Suitable for standard-definition video streaming and broadcast.
     * */
    WisStdCodecProfileH265Main = 32,
    /**
     * @brief H.265 Main 10 profile. Adds support for 10-bit video, providing better color depth and improved
     * compression efficiency compared to the Main profile.
     * */
    WisStdCodecProfileH265Main10 = 33,
    /**
     * @brief H.265 Format Range Extensions profile. Adds support for additional features such as higher bit depths,
     * wider color gamut, and improved compression efficiency, suitable for high-quality video streaming and broadcast.
     * */
    WisStdCodecProfileH265FormatRangeExt = 34,
    /**
     * @brief H.265 Screen Content Coding Extensions profile. Designed for screen content coding, offering improved
     * compression efficiency for computer-generated content such as screen sharing and remote desktop applications.
     * */
    WisStdCodecProfileH265SCCExt = 35,
    /**
     * @brief AV1 Main profile. Suitable for a wide range of applications, including web streaming and video
     * conferencing.
     * */
    WisStdCodecProfileAV1Main = 64,
    /**
     * @brief AV1 High profile. Adds support for additional features such as higher bit depths and improved compression
     * efficiency, suitable for high-quality video streaming and broadcast.
     * */
    WisStdCodecProfileAV1High = 65,
    /**
     * @brief AV1 Professional profile. Designed for professional video production and post-production workflows,
     * offering support for high bit depths, wide color gamut, and advanced features.
     * */
    WisStdCodecProfileAV1Professional = 66,
    /**
     * @brief VP9 Profile 0. Supports 8-bit video with 4:2:0 chroma subsampling, suitable for web streaming and
     * general-purpose video applications.
     * */
    WisStdCodecProfileVP9Profile0 = 96,
    /**
     * @brief VP9 Profile 1. Adds support for 8-bit video with 4:2:2 and 4:4:4 chroma subsampling, suitable for
     * professional video production and post-production workflows that require higher color fidelity.
     * */
    WisStdCodecProfileVP9Profile1 = 97,
    /**
     * @brief VP9 Profile 2. Adds support for 10-bit and 12-bit video with 4:2:0 chroma subsampling.
     * */
    WisStdCodecProfileVP9Profile2 = 98,
    /**
     * @brief VP9 Profile 3. Adds support for 10-bit and 12-bit video with 4:2:2 and 4:4:4 chroma subsampling.
     * */
    WisStdCodecProfileVP9Profile3 = 99,
} WisStdCodecProfile;

/**
 * @brief Provided by Wisdom 0.7.1. Video codec flags. Used to request and check supported codecs.
 *
 * */
typedef enum WisVideoCodecFlags {
    WisVideoCodecFlagsNone = 0, ///< No video is requested. The extension will not initialize.
    WisVideoCodecFlagsH264 = (1u << 0), ///< H.264 video codec.
    WisVideoCodecFlagsH265 = (1u << 1), ///< H.265 video codec.
    WisVideoCodecFlagsAV1 = (1u << 2), ///< AV1 video codec.
    WisVideoCodecFlagsVP9 = (1u << 3), ///< VP9 video codec.
} WisVideoCodecFlags;

/**
 * @brief Provided by Wisdom 0.7.1. Bit depth flags for video components. Used to specify supported bit depths for video
 * decoding.
 *
 * */
typedef enum WisComponentBitDepth {
    WisComponentBitDepthNone = 0, ///< No bit depth is specified.
    WisComponentBitDepthBit8 = (1u << 0), ///< 8-bit video component.
    WisComponentBitDepthBit10 = (1u << 1), ///< 10-bit video component.
    WisComponentBitDepthBit12 = (1u << 2), ///< 12-bit video component.
} WisComponentBitDepth;

/**
 * @brief Provided by Wisdom 0.7.1. Chroma subsampling flags for video components. Used to specify supported chroma
 * subsampling formats for video decoding.
 *
 * */
typedef enum WisChromaSubsampling {
    WisChromaSubsamplingNone = 0, ///< No chroma subsampling is specified.
    WisChromaSubsamplingC420 = (1u << 1), ///< 4:2:0 chroma subsampling format.
    WisChromaSubsamplingC422 = (1u << 2), ///< 4:2:2 chroma subsampling format.
    WisChromaSubsamplingC444 = (1u << 3), ///< 4:4:4 chroma subsampling format.
} WisChromaSubsampling;

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Information about a supported video codec.
 *
 * */
typedef struct WisVideoCodecDesc {
    WisStdCodecProfile codec_profile; ///< The video codec to query capabilities for.
    /**
     * @brief Supported bit depths for this codec, represented as a bitmask of ComponentBitDepth flags.
     * */
    WisComponentBitDepth bit_depth;
    /**
     * @brief Supported chroma subsampling formats for this codec, represented as a bitmask of ChromaSubsampling flags.
     * */
    WisChromaSubsampling chroma_subsampling;
} WisVideoCodecDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Information about a video decode operation.
 *
 * */
typedef struct WisVideoDecodeInfo {
    uint32_t max_width; ///< Max width of the video frame in pixels.
    uint32_t max_height; ///< Max height of the video frame in pixels.
} WisVideoDecodeInfo;

#ifdef WISDOM_DX12
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
WIS_DEFINE_DX12_DEVICE_EXT_HANDLE(WisDX12VideoDecodingExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecodingExtension handle.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 *
 * */
WISDOM_VIDEO_API void wisDX12DestroyVideoDecodingExtension(WisDX12VideoDecodingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Initializes WisVideoDecodingExtension with a request of codecs.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param request_codecs Bitmask of requested video codecs. The extension will attempt to initialize with support for
 * these codecs.
 *
 * */
WISDOM_VIDEO_API void wisDX12InitVideoDecodingExtension(
    WisDX12VideoDecodingExtension* self,
    WisVideoCodecFlags request_codecs
);

/**
 * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param codec_desc Information about the video codec to query capabilities for. The 'codec' field should specify the
 * codec to check, and the function will fill in the supported bit depths and chroma subsampling formats for that codec.
 * @param decode_info Information about the video decode capabilities of the current device, including maximum supported
 * video frame dimensions.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionQueryCodecCaps(
    WisDX12VideoDecodingExtension* self,
    const WisVideoCodecDesc* codec_desc,
    WisVideoDecodeInfo* decode_info
);

#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
WIS_DEFINE_VK_DEVICE_EXT_HANDLE(WisVKVideoDecodingExtension, 5);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecodingExtension handle.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 *
 * */
WISDOM_VIDEO_API void wisVKDestroyVideoDecodingExtension(WisVKVideoDecodingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Initializes WisVideoDecodingExtension with a request of codecs.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param request_codecs Bitmask of requested video codecs. The extension will attempt to initialize with support for
 * these codecs.
 *
 * */
WISDOM_VIDEO_API void wisVKInitVideoDecodingExtension(
    WisVKVideoDecodingExtension* self,
    WisVideoCodecFlags request_codecs
);

/**
 * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param codec_desc Information about the video codec to query capabilities for. The 'codec' field should specify the
 * codec to check, and the function will fill in the supported bit depths and chroma subsampling formats for that codec.
 * @param decode_info Information about the video decode capabilities of the current device, including maximum supported
 * video frame dimensions.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisVKVideoDecodingExtensionQueryCodecCaps(
    WisVKVideoDecodingExtension* self,
    const WisVideoCodecDesc* codec_desc,
    WisVideoDecodeInfo* decode_info
);

#endif // WISDOM_VULKAN

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_VIDEO_C_API_H
