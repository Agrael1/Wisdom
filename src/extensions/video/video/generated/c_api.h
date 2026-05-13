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
 * @brief Provided by Wisdom 0.7.1. AV1 profiles as defined in the AV1 Bitstream Specification section 6.4.1.
 *
 * */
typedef enum WisStdVideoAV1Profile {
    WisStdVideoAV1ProfileMain = 0, ///< Main profile (8-bit or 10-bit color, 4:0:0 or 4:2:0).
    WisStdVideoAV1ProfileHigh = 1, ///< High profile (adds 8-bit or 10-bit 4:4:4).
    WisStdVideoAV1ProfileProfessional = 2, ///< Professional profile (adds 12-bit color, and 4:2:2).
    WisStdVideoAV1ProfileInvalid = 0x7FFFFFFF, ///< Invalid profile.
} WisStdVideoAV1Profile;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 levels as defined in the AV1 Bitstream Specification Annex A.3.
 *
 * */
typedef enum WisStdVideoAV1Level {
    WisStdVideoAV1LevelLevel2_0 = 0, ///< Level 2.0
    WisStdVideoAV1LevelLevel2_1 = 1, ///< Level 2.1
    WisStdVideoAV1LevelLevel2_2 = 2, ///< Level 2.2
    WisStdVideoAV1LevelLevel2_3 = 3, ///< Level 2.3
    WisStdVideoAV1LevelLevel3_0 = 4, ///< Level 3.0
    WisStdVideoAV1LevelLevel3_1 = 5, ///< Level 3.1
    WisStdVideoAV1LevelLevel3_2 = 6, ///< Level 3.2
    WisStdVideoAV1LevelLevel3_3 = 7, ///< Level 3.3
    WisStdVideoAV1LevelLevel4_0 = 8, ///< Level 4.0
    WisStdVideoAV1LevelLevel4_1 = 9, ///< Level 4.1
    WisStdVideoAV1LevelLevel4_2 = 10, ///< Level 4.2
    WisStdVideoAV1LevelLevel4_3 = 11, ///< Level 4.3
    WisStdVideoAV1LevelLevel5_0 = 12, ///< Level 5.0
    WisStdVideoAV1LevelLevel5_1 = 13, ///< Level 5.1
    WisStdVideoAV1LevelLevel5_2 = 14, ///< Level 5.2
    WisStdVideoAV1LevelLevel5_3 = 15, ///< Level 5.3
    WisStdVideoAV1LevelLevel6_0 = 16, ///< Level 6.0
    WisStdVideoAV1LevelLevel6_1 = 17, ///< Level 6.1
    WisStdVideoAV1LevelLevel6_2 = 18, ///< Level 6.2
    WisStdVideoAV1LevelLevel6_3 = 19, ///< Level 6.3
    WisStdVideoAV1LevelLevel7_0 = 20, ///< Level 7.0
    WisStdVideoAV1LevelLevel7_1 = 21, ///< Level 7.1
    WisStdVideoAV1LevelLevel7_2 = 22, ///< Level 7.2
    WisStdVideoAV1LevelLevel7_3 = 23, ///< Level 7.3
    WisStdVideoAV1LevelInvalid = 0x7FFFFFFF, ///< Invalid level.
} WisStdVideoAV1Level;

/**
 * @brief Provided by Wisdom 0.7.1. Specifies the AV1 frame type (AV1 Bitstream Specification Section 6.8.2).
 *
 * */
typedef enum WisStdVideoAV1FrameType {
    WisStdVideoAV1FrameTypeKey = 0, ///< A key frame contains only intra-coded blocks and is fully decipherable.
    WisStdVideoAV1FrameTypeInter = 1, ///< An inter frame may contain intra-coded blocks and inter-coded blocks.
    /**
     * @brief An intra-only frame contains only intra-coded blocks but acts otherwise as an inter frame.
     * */
    WisStdVideoAV1FrameTypeIntraOnly = 2,
    /**
     * @brief A switch frame is an inter frame that can be used as a switching point for adaptive streaming.
     * */
    WisStdVideoAV1FrameTypeSwitch = 3,
    WisStdVideoAV1FrameTypeInvalid = 0x7FFFFFFF, ///< Invalid frame type.
} WisStdVideoAV1FrameType;

/**
 * @brief Provided by Wisdom 0.7.1. Names of the reference frames used in AV1 (AV1 Bitstream Specification Section 6.1).
 *
 * */
typedef enum WisStdVideoAV1ReferenceName {
    WisStdVideoAV1ReferenceNameIntraFrame = 0, ///< Intra frame reference.
    WisStdVideoAV1ReferenceNameLastFrame = 1, ///< LAST_FRAME (1).
    WisStdVideoAV1ReferenceNameLast2Frame = 2, ///< LAST2_FRAME (2).
    WisStdVideoAV1ReferenceNameLast3Frame = 3, ///< LAST3_FRAME (3).
    WisStdVideoAV1ReferenceNameGoldenFrame = 4, ///< GOLDEN_FRAME (4).
    WisStdVideoAV1ReferenceNameBwdrefFrame = 5, ///< BWDREF_FRAME (5).
    WisStdVideoAV1ReferenceNameAltref2Frame = 6, ///< ALTREF2_FRAME (6).
    WisStdVideoAV1ReferenceNameAltrefFrame = 7, ///< ALTREF_FRAME (7).
    WisStdVideoAV1ReferenceNameInvalid = 0x7FFFFFFF, ///< Invalid reference name.
} WisStdVideoAV1ReferenceName;

/**
 * @brief Provided by Wisdom 0.7.1. Interpolation filter types (AV1 Bitstream Specification Section 6.8.9).
 *
 * */
typedef enum WisStdVideoAV1InterpolationFilter {
    WisStdVideoAV1InterpolationFilterEighttap = 0, ///< Eight-tap filter.
    WisStdVideoAV1InterpolationFilterEighttapSmooth = 1, ///< Eight-tap smooth filter.
    WisStdVideoAV1InterpolationFilterEighttapSharp = 2, ///< Eight-tap sharp filter.
    WisStdVideoAV1InterpolationFilterBilinear = 3, ///< Bilinear filter.
    WisStdVideoAV1InterpolationFilterSwitchable = 4, ///< Switchable interpolation filter at the block level.
    WisStdVideoAV1InterpolationFilterInvalid = 0x7FFFFFFF, ///< Invalid filter.
} WisStdVideoAV1InterpolationFilter;

/**
 * @brief Provided by Wisdom 0.7.1. Transform mode (AV1 Bitstream Specification Section 6.8.21).
 *
 * */
typedef enum WisStdVideoAV1TxMode {
    WisStdVideoAV1TxModeOnly4x4 = 0, ///< Only 4x4 transforms.
    WisStdVideoAV1TxModeLargest = 1, ///< Largest allowed transform for the partition.
    WisStdVideoAV1TxModeSelect = 2, ///< Select the transform mode.
    WisStdVideoAV1TxModeInvalid = 0x7FFFFFFF, ///< Invalid TxMode.
} WisStdVideoAV1TxMode;

/**
 * @brief Provided by Wisdom 0.7.1. Loop restoration types (AV1 Bitstream Specification Section 6.10.15).
 *
 * */
typedef enum WisStdVideoAV1FrameRestorationType {
    WisStdVideoAV1FrameRestorationTypeNone = 0, ///< No loop restoration.
    WisStdVideoAV1FrameRestorationTypeWiener = 1, ///< Wiener filter loop restoration.
    WisStdVideoAV1FrameRestorationTypeSgrproj = 2, ///< Self-guided filter loop restoration.
    WisStdVideoAV1FrameRestorationTypeSwitchable = 3, ///< Switchable between Wiener and Sgrproj.
    WisStdVideoAV1FrameRestorationTypeInvalid = 0x7FFFFFFF, ///< Invalid restoration type.
} WisStdVideoAV1FrameRestorationType;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 color primaries mapping to ISO/IEC 23000-2 / CICP.
 *
 * */
typedef enum WisStdVideoAV1ColorPrimaries {
    WisStdVideoAV1ColorPrimariesBt709 = 1, ///< Rec. ITU-R BT.709-6.
    WisStdVideoAV1ColorPrimariesUnspecified = 2, ///< Image characteristics are unknown or unspecified.
    WisStdVideoAV1ColorPrimariesBt470M = 4, ///< Rec. ITU-R BT.470-6 System M (historical).
    WisStdVideoAV1ColorPrimariesBt470BG = 5, ///< Rec. ITU-R BT.470-6 System B, G (historical).
    WisStdVideoAV1ColorPrimariesBt601 = 6, ///< Rec. ITU-R BT.601-7 525.
    WisStdVideoAV1ColorPrimariesSmpte240 = 7, ///< SMPTE 240M.
    WisStdVideoAV1ColorPrimariesGenericFilm = 8, ///< Generic film (color filters using Illuminant C).
    WisStdVideoAV1ColorPrimariesBt2020 = 9, ///< Rec. ITU-R BT.2020-2.
    WisStdVideoAV1ColorPrimariesXyz = 10, ///< SMPTE ST 428-1.
    WisStdVideoAV1ColorPrimariesSmpte431 = 11, ///< SMPTE RP 431-2.
    WisStdVideoAV1ColorPrimariesSmpte432 = 12, ///< SMPTE EG 432-1.
    WisStdVideoAV1ColorPrimariesEbu3213 = 22, ///< EBU Tech. 3213-E.
    WisStdVideoAV1ColorPrimariesInvalid = 0x7FFFFFFF, ///< Invalid.
} WisStdVideoAV1ColorPrimaries;

/**
 * @brief Provided by Wisdom 0.7.1. Transfer characteristics for AV1.
 *
 * */
typedef enum WisStdVideoAV1TransferCharacteristics {
    WisStdVideoAV1TransferCharacteristicsReserved0 = 0,
    WisStdVideoAV1TransferCharacteristicsBt709 = 1, ///< Rec. ITU-R BT.709-6.
    WisStdVideoAV1TransferCharacteristicsUnspecified = 2, ///< Unspecified.
    WisStdVideoAV1TransferCharacteristicsReserved3 = 3,
    WisStdVideoAV1TransferCharacteristicsBt470M = 4, ///< Rec. ITU-R BT.470-6 System M (historical).
    WisStdVideoAV1TransferCharacteristicsBt470BG = 5, ///< Rec. ITU-R BT.470-6 System B, G (historical).
    WisStdVideoAV1TransferCharacteristicsBt601 = 6, ///< Rec. ITU-R BT.601-7.
    WisStdVideoAV1TransferCharacteristicsSmpte240 = 7, ///< SMPTE 240M.
    WisStdVideoAV1TransferCharacteristicsLinear = 8, ///< Linear transfer characteristics.
    WisStdVideoAV1TransferCharacteristicsLog100 = 9, ///< Logarithmic transfer characteristic (100:1 range).
    /**
     * @brief Logarithmic transfer characteristic (100 * Sqrt(10) : 1 range).
     * */
    WisStdVideoAV1TransferCharacteristicsLog100Sqrt10 = 10,
    WisStdVideoAV1TransferCharacteristicsIec61966 = 11, ///< IEC 61966-2-4.
    /**
     * @brief Rec. ITU-R BT.1361-0 extended colour gamut system (historical).
     * */
    WisStdVideoAV1TransferCharacteristicsBt1361 = 12,
    WisStdVideoAV1TransferCharacteristicsSrgb = 13, ///< IEC 61966-2-1 sRGB.
    WisStdVideoAV1TransferCharacteristicsBt2020_10Bit = 14, ///< Rec. ITU-R BT.2020-2 (10-bit system).
    WisStdVideoAV1TransferCharacteristicsBt2020_12Bit = 15, ///< Rec. ITU-R BT.2020-2 (12-bit system).
    WisStdVideoAV1TransferCharacteristicsSmpte2084 = 16, ///< SMPTE ST 2084 (PQ).
    WisStdVideoAV1TransferCharacteristicsSmpte428 = 17, ///< SMPTE ST 428-1.
    WisStdVideoAV1TransferCharacteristicsHlg = 18, ///< ARIB STD-B67 (HLG).
    WisStdVideoAV1TransferCharacteristicsInvalid = 0x7FFFFFFF,
} WisStdVideoAV1TransferCharacteristics;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 matrix coefficients mapping to CICP.
 *
 * */
typedef enum WisStdVideoAV1MatrixCoefficients {
    WisStdVideoAV1MatrixCoefficientsIdentity = 0, ///< Identity matrix.
    WisStdVideoAV1MatrixCoefficientsBt709 = 1, ///< Rec. ITU-R BT.709-6.
    WisStdVideoAV1MatrixCoefficientsUnspecified = 2, ///< Matrix characteristics are unspecified.
    WisStdVideoAV1MatrixCoefficientsReserved3 = 3,
    WisStdVideoAV1MatrixCoefficientsFcc = 4, ///< FCC Title 47 Code of Federal Regulations.
    WisStdVideoAV1MatrixCoefficientsBt470BG = 5, ///< Rec. ITU-R BT.470-6 System B, G (historical).
    WisStdVideoAV1MatrixCoefficientsBt601 = 6, ///< Rec. ITU-R BT.601-7.
    WisStdVideoAV1MatrixCoefficientsSmpte240 = 7, ///< SMPTE 240M.
    WisStdVideoAV1MatrixCoefficientsSmpteYcgco = 8, ///< YCgCo.
    WisStdVideoAV1MatrixCoefficientsBt2020Ncl = 9, ///< Bt2020 non-constant luminance.
    WisStdVideoAV1MatrixCoefficientsBt2020Cl = 10, ///< Bt2020 constant luminance.
    WisStdVideoAV1MatrixCoefficientsSmpte2085 = 11, ///< SMPTE ST 2085.
    WisStdVideoAV1MatrixCoefficientsChromatNcl = 12, ///< Chromaticity-derived non-constant luminance.
    WisStdVideoAV1MatrixCoefficientsChromatCl = 13, ///< Chromaticity-derived constant luminance.
    WisStdVideoAV1MatrixCoefficientsIctcp = 14, ///< Rec. ITU-R BT.2100-0 ICtCp.
    WisStdVideoAV1MatrixCoefficientsInvalid = 0x7FFFFFFF,
} WisStdVideoAV1MatrixCoefficients;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 chroma sample position (AV1 Bitstream Specification Section 6.4.2).
 *
 * */
typedef enum WisStdVideoAV1ChromaSamplePosition {
    WisStdVideoAV1ChromaSamplePositionUnknown = 0, ///< Unknown chroma sample position.
    WisStdVideoAV1ChromaSamplePositionVertical = 1, ///< Horizontally co-located with luma, vertically shifted by 0.5.
    WisStdVideoAV1ChromaSamplePositionColocated = 2, ///< Co-located with luma.
    WisStdVideoAV1ChromaSamplePositionReserved = 3,
    WisStdVideoAV1ChromaSamplePositionInvalid = 0x7FFFFFFF,
} WisStdVideoAV1ChromaSamplePosition;

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
     * @brief H.265 Main 12 profile. Adds support for 12-bit video, offering even greater color depth and improved
     * compression efficiency compared to the Main 10 profile.
     * */
    WisStdCodecProfileH265Main12 = 34,
    /**
     * @brief H.265 Main 16 profile. Adds support for 16-bit video, providing the highest color depth and best
     * compression efficiency among the Main profiles, suitable for professional video production and post-production
     * workflows.
     * */
    WisStdCodecProfileH265Main16 = 35,
    /**
     * @brief H.265 Format Range Extensions profile. Adds support for additional features such as higher bit depths,
     * wider color gamut, and improved compression efficiency, suitable for high-quality video streaming and broadcast.
     * */
    WisStdCodecProfileH265FormatRangeExt = 36,
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
    WisComponentBitDepthBit16 = (1u << 3), ///< 16-bit video component.
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
 * @brief Provided by Wisdom 0.7.1. Color configuration flags (AV1 Bitstream Specification 6.4.2).
 *
 * */
typedef struct WisStdVideoAV1ColorConfigFlags {
    uint32_t mono_chrome                    : 1; ///< Indicates if the video does not contain U and V color planes.
    uint32_t color_range                    : 1; ///< Flag indicating if full color range is used.
    uint32_t separate_uv_delta_q            : 1; ///< Flag indicating U and V planes have separate delta quantization.
    uint32_t color_description_present_flag : 1; ///< Indicates if color description is present.
    uint32_t reserved                       : 28;
} WisStdVideoAV1ColorConfigFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Color Configuration (AV1 Bitstream Specification Section 6.4.2).
 *
 * */
typedef struct WisStdVideoAV1ColorConfig {
    WisStdVideoAV1ColorConfigFlags flags; ///< Color configuration flags.
    uint8_t BitDepth; ///< Bit depth of the color samples (8, 10, or 12).
    uint8_t subsampling_x; ///< Chroma subsampling x.
    uint8_t subsampling_y; ///< Chroma subsampling y.
    uint8_t reserved1;
    WisStdVideoAV1ColorPrimaries color_primaries; ///< Color primaries.
    WisStdVideoAV1TransferCharacteristics transfer_characteristics; ///< Transfer characteristics.
    WisStdVideoAV1MatrixCoefficients matrix_coefficients; ///< Matrix coefficients.
    WisStdVideoAV1ChromaSamplePosition chroma_sample_position; ///< Chroma sample position.
} WisStdVideoAV1ColorConfig;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Timing Info flags.
 *
 * */
typedef struct WisStdVideoAV1TimingInfoFlags {
    uint32_t equal_picture_interval : 1; ///< Indicates if pictures should be displayed with equal intervals.
    uint32_t reserved               : 31;
} WisStdVideoAV1TimingInfoFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Timing Info (AV1 Bitstream Specification Section 6.4.3).
 *
 * */
typedef struct WisStdVideoAV1TimingInfo {
    WisStdVideoAV1TimingInfoFlags flags; ///< Timing flags.
    uint32_t num_units_in_display_tick; ///< Number of units in a display tick.
    uint32_t time_scale; ///< Time scale.
    uint32_t num_ticks_per_picture_minus_1; ///< Ticks per picture minus 1.
} WisStdVideoAV1TimingInfo;

/**
 * @brief Provided by Wisdom 0.7.1. Loop filter flags (AV1 Bitstream Specification Section 6.8.10).
 *
 * */
typedef struct WisStdVideoAV1LoopFilterFlags {
    /**
     * @brief Indicates whether the filter level depends on the mode and reference frame used to predict a block.
     * */
    uint32_t loop_filter_delta_enabled : 1;
    /**
     * @brief Indicates whether additional syntax elements are present that specify which mode and reference frame
     * deltas are to be updated.
     * */
    uint32_t loop_filter_delta_update : 1;
    uint32_t reserved                 : 30;
} WisStdVideoAV1LoopFilterFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Loop Filter Parameters (AV1 Bitstream Specification Section 6.8.10).
 *
 * */
typedef struct WisStdVideoAV1LoopFilter {
    WisStdVideoAV1LoopFilterFlags flags; ///< Loop filter flags.
    uint8_t loop_filter_level[4]; ///< Array containing loop filter strength values.
    uint8_t loop_filter_sharpness; ///< Loop filter sharpness.
    uint8_t update_ref_delta; ///< Indicates that the loop filter ref deltas are to be updated.
    int8_t loop_filter_ref_deltas[8]; ///< Loop filter reference deltas.
    uint8_t update_mode_delta; ///< Indicates that the loop filter mode deltas are to be updated.
    int8_t loop_filter_mode_deltas[2]; ///< Loop filter mode deltas.
} WisStdVideoAV1LoopFilter;

/**
 * @brief Provided by Wisdom 0.7.1. Quantization flags (AV1 Bitstream Specification Section 6.8.11).
 *
 * */
typedef struct WisStdVideoAV1QuantizationFlags {
    uint32_t using_qmatrix : 1; ///< Specifies whether the quantizer matrix should be used.
    uint32_t diff_uv_delta : 1; ///< Specifies whether the U and V delta quantizer values are transmitted separately.
    uint32_t reserved      : 30;
} WisStdVideoAV1QuantizationFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Quantization Parameters (AV1 Bitstream Specification Section 6.8.11).
 *
 * */
typedef struct WisStdVideoAV1Quantization {
    WisStdVideoAV1QuantizationFlags flags; ///< Quantization flags.
    uint8_t base_q_idx; ///< Indicates the base frame qindex.
    int8_t DeltaQYDc; ///< Y DC quantizer relative to base_q_idx.
    int8_t DeltaQUDc; ///< U DC quantizer relative to base_q_idx.
    int8_t DeltaQUAc; ///< U AC quantizer relative to base_q_idx.
    int8_t DeltaQVDc; ///< V DC quantizer relative to base_q_idx.
    int8_t DeltaQVAc; ///< V AC quantizer relative to base_q_idx.
    /**
     * @brief Specifies the level in the quantizer matrix that should be used for luma plane decoding.
     * */
    uint8_t qm_y;
    /**
     * @brief Specifies the level in the quantizer matrix that should be used for chroma U plane decoding.
     * */
    uint8_t qm_u;
    /**
     * @brief Specifies the level in the quantizer matrix that should be used for chroma V plane decoding.
     * */
    uint8_t qm_v;
} WisStdVideoAV1Quantization;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Segmentation Parameters (AV1 Bitstream Specification Section 6.8.13).
 *
 * */
typedef struct WisStdVideoAV1Segmentation {
    uint8_t FeatureEnabled[8]; ///< Array specifying whether the feature is enabled for a segment.
    int16_t FeatureData[8 * 8]; ///< Array specifying the feature data for a segment feature.
} WisStdVideoAV1Segmentation;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Tile Info Flags (AV1 Bitstream Specification Section 6.8.14).
 *
 * */
typedef struct WisStdVideoAV1TileInfoFlags {
    uint32_t uniform_tile_spacing_flag : 1; ///< Indicates that the tiles are uniformly spaced across the picture.
    uint32_t reserved                  : 31;
} WisStdVideoAV1TileInfoFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Tile Information (AV1 Bitstream Specification Section 6.8.14).
 *
 * */
typedef struct WisStdVideoAV1TileInfo {
    WisStdVideoAV1TileInfoFlags flags; ///< Tile info flags.
    uint8_t TileCols; ///< Number of tiles across the picture.
    uint8_t TileRows; ///< Number of tiles down the picture.
    uint16_t context_update_tile_id; ///< Specifies which tile to use for the CDF update.
    /**
     * @brief Specifies the number of bytes needed to code each tile size.
     * */
    uint8_t tile_size_bytes_minus_1;
    uint8_t reserved1[7];
    /**
     * @brief Pointer to an array specifying the start column (in MI units) for each tile column.
     * */
    const uint16_t* pMiColStarts;
    /**
     * @brief Pointer to an array specifying the start row (in MI units) for each tile row.
     * */
    const uint16_t* pMiRowStarts;
    const uint16_t* pWidthInSbsMinus1; ///< Pointer to an array of tile widths in superblocks minus 1.
    const uint16_t* pHeightInSbsMinus1; ///< Pointer to an array of tile heights in superblocks minus 1.
} WisStdVideoAV1TileInfo;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Constrained Directional Enhancement Filter (CDEF) parameters (AV1 Bitstream
 * Specification Section 6.8.19).
 *
 * */
typedef struct WisStdVideoAV1CDEF {
    uint8_t cdef_damping_minus_3; ///< Controls the amount of damping in the deringing filter.
    uint8_t cdef_bits; ///< Specifies the number of bits needed to specify the CDEF filter strength.
    uint8_t cdef_y_pri_strength[8]; ///< Primary filter strength for Y.
    uint8_t cdef_y_sec_strength[8]; ///< Secondary filter strength for Y.
    uint8_t cdef_uv_pri_strength[8]; ///< Primary filter strength for UV.
    uint8_t cdef_uv_sec_strength[8]; ///< Secondary filter strength for UV.
} WisStdVideoAV1CDEF;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Loop Restoration parameters (AV1 Bitstream Specification Section 6.8.20).
 *
 * */
typedef struct WisStdVideoAV1LoopRestoration {
    /**
     * @brief Array specifying the loop restoration type for each plane (Y, U, V).
     * */
    WisStdVideoAV1FrameRestorationType FrameRestorationType[3];
    /**
     * @brief Array specifying the size of loop restoration units for each plane.
     * */
    uint16_t LoopRestorationSize[3];
} WisStdVideoAV1LoopRestoration;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Global Motion parameters (AV1 Bitstream Specification Section 6.8.17).
 *
 * */
typedef struct WisStdVideoAV1GlobalMotion {
    uint8_t GmType[8]; ///< Array specifying the global motion type for each reference frame.
    int32_t gm_params[8 * 6]; ///< Array of global motion parameters.
} WisStdVideoAV1GlobalMotion;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Film Grain Flags (AV1 Bitstream Specification Section 6.8.24).
 *
 * */
typedef struct WisStdVideoAV1FilmGrainFlags {
    uint32_t chroma_scaling_from_luma : 1; ///< Flag indicating that chroma scaling is derived from luma.
    uint32_t overlap_flag             : 1; ///< Flag indicating overlapping film grain blocks.
    uint32_t clip_to_restricted_range : 1; ///< Flag indicating clipping to restricted range.
    uint32_t update_grain             : 1; ///< Flag indicating the film grain parameters are updated in this frame.
    uint32_t reserved                 : 28;
} WisStdVideoAV1FilmGrainFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Film Grain parameters (AV1 Bitstream Specification Section 6.8.24).
 *
 * */
typedef struct WisStdVideoAV1FilmGrain {
    WisStdVideoAV1FilmGrainFlags flags; ///< Film grain flags.
    uint8_t grain_scaling_minus_8; ///< Shift value for the film grain scale calculation.
    uint8_t ar_coeff_lag; ///< Number of auto-regressive coefficients.
    uint8_t ar_coeff_shift_minus_6; ///< Shift value for auto-regressive coefficients.
    /**
     * @brief Specifies how much the Gaussian random numbers should be scaled down.
     * */
    uint8_t grain_scale_shift;
    uint16_t grain_seed; ///< Specifies the seed for the pseudo-random number generator.
    /**
     * @brief Specifies the reference frame index to obtain the film grain parameters from.
     * */
    uint8_t film_grain_params_ref_idx;
    uint8_t num_y_points; ///< Number of points for luma scaling.
    uint8_t point_y_value[14]; ///< Luma point values.
    uint8_t point_y_scaling[14]; ///< Luma point scaling.
    uint8_t num_cb_points; ///< Number of points for Cb scaling.
    uint8_t point_cb_value[10]; ///< Cb point values.
    uint8_t point_cb_scaling[10]; ///< Cb point scaling.
    uint8_t num_cr_points; ///< Number of points for Cr scaling.
    uint8_t point_cr_value[10]; ///< Cr point values.
    uint8_t point_cr_scaling[10]; ///< Cr point scaling.
    int8_t ar_coeffs_y_plus_128[24]; ///< Auto-regressive coefficients for Y.
    int8_t ar_coeffs_cb_plus_128[25]; ///< Auto-regressive coefficients for Cb.
    int8_t ar_coeffs_cr_plus_128[25]; ///< Auto-regressive coefficients for Cr.
    uint8_t cb_mult; ///< Cb multiplier for chroma scaling from luma.
    uint8_t cb_luma_mult; ///< Cb luma multiplier for chroma scaling from luma.
    uint16_t cb_offset; ///< Cb offset for chroma scaling from luma.
    uint8_t cr_mult; ///< Cr multiplier for chroma scaling from luma.
    uint8_t cr_luma_mult; ///< Cr luma multiplier for chroma scaling from luma.
    uint16_t cr_offset; ///< Cr offset for chroma scaling from luma.
} WisStdVideoAV1FilmGrain;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Sequence Header flags (AV1 Bitstream Specification Section 5.5).
 *
 * */
typedef struct WisStdVideoAV1SequenceHeaderFlags {
    uint32_t still_picture                : 1; ///< Specifies if the video sequence contains a single still picture.
    uint32_t reduced_still_picture_header : 1; ///< Specifies if reduced header parameters are used for a still picture.
    uint32_t use_128x128_superblock       : 1; ///< Specifies if superblocks are 128x128 or 64x64.
    uint32_t enable_filter_intra          : 1; ///< Specifies if the filter intra predictor can be used.
    uint32_t enable_intra_edge_filter     : 1; ///< Specifies if intra edge filtering can be used.
    uint32_t enable_interintra_compound   : 1; ///< Specifies if inter-intra compound prediction can be used.
    uint32_t enable_masked_compound       : 1; ///< Specifies if masked compound prediction can be used.
    uint32_t enable_warped_motion         : 1; ///< Specifies if warped motion can be used.
    uint32_t enable_dual_filter           : 1; ///< Specifies if dual interpolation filters can be used.
    uint32_t enable_order_hint            : 1; ///< Specifies if order hints are used.
    uint32_t enable_jnt_comp      : 1; ///< Specifies if the distance weights process is used for compound prediction.
    uint32_t enable_ref_frame_mvs : 1; ///< Specifies if reference frame motion vectors are present.
    uint32_t frame_id_numbers_present_flag      : 1; ///< Specifies if frame ID numbers are present.
    uint32_t enable_superres                    : 1; ///< Specifies if the superresolution feature can be used.
    uint32_t enable_cdef                        : 1; ///< Specifies if the CDEF filtering process can be used.
    uint32_t enable_restoration                 : 1; ///< Specifies if loop restoration can be used.
    uint32_t film_grain_params_present          : 1; ///< Specifies if film grain parameters are present.
    uint32_t timing_info_present_flag           : 1; ///< Specifies if timing info is present.
    uint32_t initial_display_delay_present_flag : 1; ///< Specifies if the initial display delay info is present.
    uint32_t reserved                           : 13;
} WisStdVideoAV1SequenceHeaderFlags;

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Sequence Header OBU parameters (AV1 Bitstream Specification Section 5.5).
 *
 * */
typedef struct WisStdVideoAV1SequenceHeader {
    WisStdVideoAV1SequenceHeaderFlags flags; ///< Sequence header flags.
    WisStdVideoAV1Profile seq_profile; ///< AV1 profile.
    /**
     * @brief Number of bits used to specify the frame width minus 1.
     * */
    uint8_t frame_width_bits_minus_1;
    /**
     * @brief Number of bits used to specify the frame height minus 1.
     * */
    uint8_t frame_height_bits_minus_1;
    uint16_t max_frame_width_minus_1; ///< Maximum frame width minus 1.
    uint16_t max_frame_height_minus_1; ///< Maximum frame height minus 1.
    /**
     * @brief Specifies the number of bits used to encode delta_frame_id.
     * */
    uint8_t delta_frame_id_length_minus_2;
    /**
     * @brief Used to calculate the number of bits used to encode frame_id.
     * */
    uint8_t additional_frame_id_length_minus_1;
    uint8_t order_hint_bits_minus_1; ///< Used to compute OrderHintBits.
    uint8_t seq_force_integer_mv; ///< Equal to 1: motion vectors will always be integers.
    uint8_t seq_force_screen_content_tools; ///< Screen content tools setting.
    uint8_t reserved1[5];
    const WisStdVideoAV1ColorConfig* pColorConfig; ///< Pointer to color configuration parameters.
    const WisStdVideoAV1TimingInfo* pTimingInfo; ///< Pointer to timing info parameters.
} WisStdVideoAV1SequenceHeader;

/**
 * @brief Provided by Wisdom 0.7.1. Flags for AV1 decode picture info (from Uncompressed Header).
 *
 * */
typedef struct WisStdVideoDecodeAV1PictureInfoFlags {
    uint32_t error_resilient_mode             : 1; ///< Indicates error resilient mode is enabled.
    uint32_t disable_cdf_update               : 1; ///< Indicates CDF update is disabled.
    uint32_t use_superres                     : 1; ///< Indicates superresolution is enabled for this frame.
    uint32_t render_and_frame_size_different  : 1; ///< Indicates actual frame size and render frame size are different.
    uint32_t allow_screen_content_tools       : 1; ///< Indicates screen content tools are allowed.
    uint32_t is_filter_switchable             : 1; ///< Indicates whether interpolation filter is switchable.
    uint32_t force_integer_mv                 : 1; ///< Indicates whether motion vectors must be forced to integer.
    uint32_t frame_size_override_flag         : 1; ///< Indicates if frame size override is set.
    uint32_t buffer_removal_time_present_flag : 1; ///< Indicates whether buffer removal time is present.
    uint32_t allow_intrabc                    : 1; ///< Indicates if intra block copy is allowed.
    /**
     * @brief Indicates if reference frames are completely decided by last_frame_idx.
     * */
    uint32_t frame_refs_short_signaling   : 1;
    uint32_t allow_high_precision_mv      : 1; ///< Indicates whether high precision motion vectors are allowed.
    uint32_t is_motion_mode_switchable    : 1; ///< Indicates whether motion mode is switchable.
    uint32_t use_ref_frame_mvs            : 1; ///< Indicates whether reference frame MVs are used.
    uint32_t disable_frame_end_update_cdf : 1; ///< Specifies whether the frame end CDF update is skipped.
    uint32_t allow_warped_motion          : 1; ///< Indicates whether warped motion is allowed for this frame.
    uint32_t reduced_tx_set               : 1; ///< Indicates whether the frame uses a reduced transform set.
    /**
     * @brief Specifies that the mode info for inter blocks contains the syntax element comp_mode.
     * */
    uint32_t reference_select             : 1;
    uint32_t skip_mode_present            : 1; ///< Specifies whether skip mode is allowed.
    uint32_t delta_q_present              : 1; ///< Specifies whether a delta q index is present for the frame.
    uint32_t delta_lf_present             : 1; ///< Specifies whether delta loop filter values are present.
    uint32_t delta_lf_multi               : 1; ///< Specifies whether independent delta loop filter values are used.
    uint32_t segmentation_enabled         : 1; ///< Indicates if segmentation is enabled.
    uint32_t segmentation_update_map      : 1; ///< Indicates if segmentation map is updated.
    uint32_t segmentation_temporal_update : 1; ///< Indicates if temporal segmentation is updated.
    uint32_t segmentation_update_data     : 1; ///< Indicates if segmentation feature data is updated.
    uint32_t UsesLr                       : 1; ///< Indicates if loop restoration is used.
    uint32_t usesChromaLr                 : 1; ///< Indicates if loop restoration is used for chroma.
    uint32_t apply_grain                  : 1; ///< Indicates if film grain should be applied.
    uint32_t reserved                     : 3;
} WisStdVideoDecodeAV1PictureInfoFlags;

/**
 * @brief Provided by Wisdom 0.7.1. Information provided by the application to the video decoder for each AV1 picture
 * (Khronos Video extensions).
 *
 * */
typedef struct WisStdVideoDecodeAV1PictureInfo {
    WisStdVideoDecodeAV1PictureInfoFlags flags; ///< Decode picture info flags.
    WisStdVideoAV1FrameType frame_type; ///< Frame type: Key, Inter, Intra-only, or Switch.
    uint32_t current_frame_id; ///< Specifies the frame ID for the current frame.
    uint8_t OrderHint; ///< Order hint of the current frame used for motion vector scaling.
    /**
     * @brief Index of the reference frame containing the CDF values to be loaded at the start of the frame.
     * */
    uint8_t primary_ref_frame;
    /**
     * @brief An 8-bit mask that specifies which reference frame slots will be updated with the current frame.
     * */
    uint8_t refresh_frame_flags;
    uint8_t reserved1;
    /**
     * @brief Specifies the filter selection used for performing inter prediction.
     * */
    WisStdVideoAV1InterpolationFilter interpolation_filter;
    WisStdVideoAV1TxMode TxMode; ///< Specifies how the transform size is determined.
    /**
     * @brief Specifies the left shift to be applied to decoded delta q values.
     * */
    uint8_t delta_q_res;
    /**
     * @brief Specifies the left shift to be applied to decoded delta loop filter values.
     * */
    uint8_t delta_lf_res;
    /**
     * @brief Specifies the indices of the reference frames to be used for skip mode.
     * */
    uint8_t SkipModeFrame[2];
    /**
     * @brief Denominator for frame size calculation if superres is enabled.
     * */
    uint8_t coded_denom;
    uint8_t reserved2[3];
    uint8_t OrderHints[8]; ///< Order hints of the decoded reference frames.
    uint32_t expectedFrameId[8]; ///< Expected frame IDs for reference frames.
    const WisStdVideoAV1TileInfo* pTileInfo; ///< Pointer to AV1 tile information.
    const WisStdVideoAV1Quantization* pQuantization; ///< Pointer to standard quantization matrices and values.
    const WisStdVideoAV1Segmentation* pSegmentation; ///< Pointer to segmentation parameter information.
    const WisStdVideoAV1LoopFilter* pLoopFilter; ///< Pointer to loop filter parameters.
    const WisStdVideoAV1CDEF* pCDEF; ///< Pointer to CDEF parameters.
    const WisStdVideoAV1LoopRestoration* pLoopRestoration; ///< Pointer to loop restoration parameters.
    const WisStdVideoAV1GlobalMotion* pGlobalMotion; ///< Pointer to global motion parameters.
    const WisStdVideoAV1FilmGrain* pFilmGrain; ///< Pointer to film grain synthesis parameters.
} WisStdVideoDecodeAV1PictureInfo;

/**
 * @brief Provided by Wisdom 0.7.1. Flags for AV1 Decode Reference Information.
 *
 * */
typedef struct WisStdVideoDecodeAV1ReferenceInfoFlags {
    uint32_t disable_frame_end_update_cdf : 1; ///< Reference originally had disabled frame end CDF update.
    uint32_t segmentation_enabled         : 1; ///< Reference originally had segmentation enabled.
    uint32_t reserved                     : 30;
} WisStdVideoDecodeAV1ReferenceInfoFlags;

/**
 * @brief Provided by Wisdom 0.7.1. Information provided by the application about an AV1 reference frame.
 *
 * */
typedef struct WisStdVideoDecodeAV1ReferenceInfo {
    WisStdVideoDecodeAV1ReferenceInfoFlags flags; ///< Reference information flags.
    uint8_t frame_type; ///< Frame type of the reference frame.
    /**
     * @brief Specifies the direction of the reference frame relative to other references used in motion vector
     * derivation.
     * */
    uint8_t RefFrameSignBias;
    uint8_t OrderHint; ///< Order hint of the reference frame.
    /**
     * @brief Saved order hints when this reference frame was decoded.
     * */
    uint8_t SavedOrderHints[8];
} WisStdVideoDecodeAV1ReferenceInfo;

/**
 * @brief Provided by Wisdom 0.7.1. Information about a supported video codec.
 *
 * */
typedef struct WisVideoCodecDesc {
    WisStdCodecProfile codec_profile; ///< The video codec to query capabilities for.
    /**
     * @brief The data format of the video frames for this codec. This field is used to specify the expected format of
     * the video frames that will be decoded using this codec, and can influence the supported bit depths and chroma
     * subsampling formats.
     * */
    WisDataFormat image_format;
    uint32_t width; ///< Max width of the video frame in pixels.
    uint32_t height; ///< Max height of the video frame in pixels.
} WisVideoCodecDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Information about a video decode operation.
 *
 * */
typedef struct WisVideoDecoderDesc {
    uint32_t width; ///< Width of the video frame in pixels.
    uint32_t height; ///< Height of the video frame in pixels.
    /**
     * @brief The data format of the output video frames. This field specifies the expected format of the decoded video
     * frames that will be produced by the video decoder, and can influence the supported bit depths and chroma
     * subsampling formats.
     * */
    WisDataFormat image_format;
    /**
     * @brief The video codec profile that the decoder will use for decoding. This field specifies the profile of the
     * video codec that the decoder will use for decoding video frames, and can influence the supported bit depths and
     * chroma subsampling formats.
     * */
    WisStdCodecProfile codec_profile;
    /**
     * @brief The number of decode buffers that the decoder will use for decoding video frames.
     * */
    uint32_t decode_picture_buffer_count;
} WisVideoDecoderDesc;

#ifdef WISDOM_DX12
/**
 * @brief Provided by Wisdom 0.7.1. Handle for video decoder parameters. Represents the parameters and capabilities of a
 * video decoder, such as supported codecs, bit depths, and chroma subsampling formats.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12VideoDecoderParameters, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video decoder. Represents a video decoder instance that can be used to
 * decode video frames.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12VideoDecoder, 3);

/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video command list. Represents a command list that can be used to
 * record video decode commands.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12VideoDecodeCommandList, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
WIS_DEFINE_DX12_DEVICE_EXT_HANDLE(WisDX12VideoDecodingExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Variant type for video decode input descriptions. Used to specify the type of input
 * data for a video decode operation.
 *
 * */
typedef struct WisDX12VideoDecodeInputDesc {
    /**
     * @brief Input description for a video decode operation that uses a bitstream buffer as input. The buffer view
     * should contain the compressed video data to be decoded.
     * */
    WisDX12BufferView bitstream_buffer;
    uint64_t offset; ///< Offset in the buffer where the bistream data is located.
    uint64_t size; ///< Size of the bitstream data in bytes.
} WisDX12VideoDecodeInputDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecoderParameters handle.
 * @param self is a pointer to the valid WisVideoDecoderParameters instance.
 *
 * */
WISDOM_VIDEO_API void wisDX12DestroyVideoDecoderParameters(WisDX12VideoDecoderParameters* self);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecoder handle.
 * @param self is a pointer to the valid WisVideoDecoder instance.
 *
 * */
WISDOM_VIDEO_API void wisDX12DestroyVideoDecoder(WisDX12VideoDecoder* self);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecodeCommandList handle.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 *
 * */
WISDOM_VIDEO_API void wisDX12DestroyVideoDecodeCommandList(WisDX12VideoDecodeCommandList* self);

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
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult
wisDX12VideoDecodingExtensionQueryCodecCaps(WisDX12VideoDecodingExtension* self, const WisVideoCodecDesc* codec_desc);

/**
 * @brief Provided by Wisdom 0.7.1. Creates a video decoder instance.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param decoder_desc Information about the video decoder to create.
 * @param video_decoder Output parameter that holds the created video decoder handle if the operation is successful.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionCreateDecoder(
    const WisDX12VideoDecodingExtension* self,
    const WisVideoDecoderDesc* decoder_desc,
    WisDX12VideoDecoder* video_decoder
);

/**
 * @brief Provided by Wisdom 0.7.1. Creates a video command list instance.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param command_allocator The command allocator that the command list will use for memory management of command
 * buffers. It @wis_must be created with the same WisDevice as the extension and have `WisCommandQueueTypeVideoDecode`
 * or `WisCommandQueueTypeVideoEncode` specified.
 * @param command_list Output parameter that holds the created video command list handle if the operation is successful.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisDX12VideoDecodingExtensionCreateCommandList(
    WisDX12VideoDecodingExtension* self,
    const WisDX12CommandAllocator* command_allocator,
    WisDX12VideoDecodeCommandList* command_list
);

/**
 * @brief Provided by Wisdom 0.7.1. Creates parameters from provided sequence header or parameter set.
 * @param self is a pointer to the valid WisVideoDecoder instance.
 * @param sequence_parameters Pointer to the sequence header or parameter set data. The format and content of this data
 * should be appropriate for the codec being used, and should contain the necessary information to initialize the video
 * decoder parameters.
 * @param decoder_parameters Output parameter that holds the created video decoder parameters handle if the operation is
 * successful.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisDX12VideoDecoderCreateParameters(
    const WisDX12VideoDecoder* self,
    const void* sequence_parameters,
    WisDX12VideoDecoderParameters* decoder_parameters
);

/**
 * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisDX12VideoDecodeCommandListBegin(const WisDX12VideoDecodeCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisDX12VideoDecodeCommandListEnd(const WisDX12VideoDecodeCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Records a video decode command to the command list.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 * @param decoder The video decoder that will be used for decoding the video frame.
 * @param decoder_parameters Input description for a video decode operation that uses video decoder parameters as input.
 * The video decoder parameters should be created from the sequence header or parameter set data for the video stream,
 * and should contain the necessary information to initialize the video decoder for decoding the video frames.
 * @param input_desc Description of the input data for the video decode operation. This field specifies the type and
 * location of the input data that will be used for decoding the video frame.
 *
 * */
WISDOM_VIDEO_API void wisDX12VideoDecodeCommandListDecodeFrame(
    const WisDX12VideoDecodeCommandList* self,
    const WisDX12VideoDecoder* decoder,
    const WisDX12VideoDecoderParameters* decoder_parameters,
    const WisDX12VideoDecodeInputDesc* input_desc
);

#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
/**
 * @brief Provided by Wisdom 0.7.1. Handle for video decoder parameters. Represents the parameters and capabilities of a
 * video decoder, such as supported codecs, bit depths, and chroma subsampling formats.
 *
 * */
WIS_DEFINE_HANDLE(WisVKVideoDecoderParameters, 4);

/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video decoder. Represents a video decoder instance that can be used to
 * decode video frames.
 *
 * */
WIS_DEFINE_HANDLE(WisVKVideoDecoder, 4);

/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video command list. Represents a command list that can be used to
 * record video decode commands.
 *
 * */
WIS_DEFINE_HANDLE(WisVKVideoDecodeCommandList, 7);

/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
WIS_DEFINE_VK_DEVICE_EXT_HANDLE(WisVKVideoDecodingExtension, 5);

/**
 * @brief Provided by Wisdom 0.7.1. Variant type for video decode input descriptions. Used to specify the type of input
 * data for a video decode operation.
 *
 * */
typedef struct WisVKVideoDecodeInputDesc {
    /**
     * @brief Input description for a video decode operation that uses a bitstream buffer as input. The buffer view
     * should contain the compressed video data to be decoded.
     * */
    WisVKBufferView bitstream_buffer;
    uint64_t offset; ///< Offset in the buffer where the bistream data is located.
    uint64_t size; ///< Size of the bitstream data in bytes.
} WisVKVideoDecodeInputDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecoderParameters handle.
 * @param self is a pointer to the valid WisVideoDecoderParameters instance.
 *
 * */
WISDOM_VIDEO_API void wisVKDestroyVideoDecoderParameters(WisVKVideoDecoderParameters* self);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecoder handle.
 * @param self is a pointer to the valid WisVideoDecoder instance.
 *
 * */
WISDOM_VIDEO_API void wisVKDestroyVideoDecoder(WisVKVideoDecoder* self);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecodeCommandList handle.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 *
 * */
WISDOM_VIDEO_API void wisVKDestroyVideoDecodeCommandList(WisVKVideoDecodeCommandList* self);

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
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult
wisVKVideoDecodingExtensionQueryCodecCaps(WisVKVideoDecodingExtension* self, const WisVideoCodecDesc* codec_desc);

/**
 * @brief Provided by Wisdom 0.7.1. Creates a video decoder instance.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param decoder_desc Information about the video decoder to create.
 * @param video_decoder Output parameter that holds the created video decoder handle if the operation is successful.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisVKVideoDecodingExtensionCreateDecoder(
    const WisVKVideoDecodingExtension* self,
    const WisVideoDecoderDesc* decoder_desc,
    WisVKVideoDecoder* video_decoder
);

/**
 * @brief Provided by Wisdom 0.7.1. Creates a video command list instance.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param command_allocator The command allocator that the command list will use for memory management of command
 * buffers. It @wis_must be created with the same WisDevice as the extension and have `WisCommandQueueTypeVideoDecode`
 * or `WisCommandQueueTypeVideoEncode` specified.
 * @param command_list Output parameter that holds the created video command list handle if the operation is successful.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisVKVideoDecodingExtensionCreateCommandList(
    WisVKVideoDecodingExtension* self,
    const WisVKCommandAllocator* command_allocator,
    WisVKVideoDecodeCommandList* command_list
);

/**
 * @brief Provided by Wisdom 0.7.1. Creates parameters from provided sequence header or parameter set.
 * @param self is a pointer to the valid WisVideoDecoder instance.
 * @param sequence_parameters Pointer to the sequence header or parameter set data. The format and content of this data
 * should be appropriate for the codec being used, and should contain the necessary information to initialize the video
 * decoder parameters.
 * @param decoder_parameters Output parameter that holds the created video decoder parameters handle if the operation is
 * successful.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisVKVideoDecoderCreateParameters(
    const WisVKVideoDecoder* self,
    const void* sequence_parameters,
    WisVKVideoDecoderParameters* decoder_parameters
);

/**
 * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisVKVideoDecodeCommandListBegin(const WisVKVideoDecodeCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_VIDEO_API WisResult wisVKVideoDecodeCommandListEnd(const WisVKVideoDecodeCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Records a video decode command to the command list.
 * @param self is a pointer to the valid WisVideoDecodeCommandList instance.
 * @param decoder The video decoder that will be used for decoding the video frame.
 * @param decoder_parameters Input description for a video decode operation that uses video decoder parameters as input.
 * The video decoder parameters should be created from the sequence header or parameter set data for the video stream,
 * and should contain the necessary information to initialize the video decoder for decoding the video frames.
 * @param input_desc Description of the input data for the video decode operation. This field specifies the type and
 * location of the input data that will be used for decoding the video frame.
 *
 * */
WISDOM_VIDEO_API void wisVKVideoDecodeCommandListDecodeFrame(
    const WisVKVideoDecodeCommandList* self,
    const WisVKVideoDecoder* decoder,
    const WisVKVideoDecoderParameters* decoder_parameters,
    const WisVKVideoDecodeInputDesc* input_desc
);

#endif // WISDOM_VULKAN

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_VIDEO_C_API_H
