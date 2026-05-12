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
 * @brief Provided by Wisdom 0.7.1. AV1 profiles as defined in the AV1 Bitstream Specification section 6.4.1.
 *
 * */
enum class StdVideoAV1Profile {
    Main = 0, ///< Main profile (8-bit or 10-bit color, 4:0:0 or 4:2:0).
    High = 1, ///< High profile (adds 8-bit or 10-bit 4:4:4).
    Professional = 2, ///< Professional profile (adds 12-bit color, and 4:2:2).
    Invalid = 0x7FFFFFFF, ///< Invalid profile.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 levels as defined in the AV1 Bitstream Specification Annex A.3.
 *
 * */
enum class StdVideoAV1Level {
    Level2_0 = 0, ///< Level 2.0
    Level2_1 = 1, ///< Level 2.1
    Level2_2 = 2, ///< Level 2.2
    Level2_3 = 3, ///< Level 2.3
    Level3_0 = 4, ///< Level 3.0
    Level3_1 = 5, ///< Level 3.1
    Level3_2 = 6, ///< Level 3.2
    Level3_3 = 7, ///< Level 3.3
    Level4_0 = 8, ///< Level 4.0
    Level4_1 = 9, ///< Level 4.1
    Level4_2 = 10, ///< Level 4.2
    Level4_3 = 11, ///< Level 4.3
    Level5_0 = 12, ///< Level 5.0
    Level5_1 = 13, ///< Level 5.1
    Level5_2 = 14, ///< Level 5.2
    Level5_3 = 15, ///< Level 5.3
    Level6_0 = 16, ///< Level 6.0
    Level6_1 = 17, ///< Level 6.1
    Level6_2 = 18, ///< Level 6.2
    Level6_3 = 19, ///< Level 6.3
    Level7_0 = 20, ///< Level 7.0
    Level7_1 = 21, ///< Level 7.1
    Level7_2 = 22, ///< Level 7.2
    Level7_3 = 23, ///< Level 7.3
    Invalid = 0x7FFFFFFF, ///< Invalid level.
};

/**
 * @brief Provided by Wisdom 0.7.1. Specifies the AV1 frame type (AV1 Bitstream Specification Section 6.8.2).
 *
 * */
enum class StdVideoAV1FrameType {
    Key = 0, ///< A key frame contains only intra-coded blocks and is fully decipherable.
    Inter = 1, ///< An inter frame may contain intra-coded blocks and inter-coded blocks.
    IntraOnly = 2, ///< An intra-only frame contains only intra-coded blocks but acts otherwise as an inter frame.
    Switch = 3, ///< A switch frame is an inter frame that can be used as a switching point for adaptive streaming.
    Invalid = 0x7FFFFFFF, ///< Invalid frame type.
};

/**
 * @brief Provided by Wisdom 0.7.1. Names of the reference frames used in AV1 (AV1 Bitstream Specification Section 6.1).
 *
 * */
enum class StdVideoAV1ReferenceName {
    IntraFrame = 0, ///< Intra frame reference.
    LastFrame = 1, ///< LAST_FRAME (1).
    Last2Frame = 2, ///< LAST2_FRAME (2).
    Last3Frame = 3, ///< LAST3_FRAME (3).
    GoldenFrame = 4, ///< GOLDEN_FRAME (4).
    BwdrefFrame = 5, ///< BWDREF_FRAME (5).
    Altref2Frame = 6, ///< ALTREF2_FRAME (6).
    AltrefFrame = 7, ///< ALTREF_FRAME (7).
    Invalid = 0x7FFFFFFF, ///< Invalid reference name.
};

/**
 * @brief Provided by Wisdom 0.7.1. Interpolation filter types (AV1 Bitstream Specification Section 6.8.9).
 *
 * */
enum class StdVideoAV1InterpolationFilter {
    Eighttap = 0, ///< Eight-tap filter.
    EighttapSmooth = 1, ///< Eight-tap smooth filter.
    EighttapSharp = 2, ///< Eight-tap sharp filter.
    Bilinear = 3, ///< Bilinear filter.
    Switchable = 4, ///< Switchable interpolation filter at the block level.
    Invalid = 0x7FFFFFFF, ///< Invalid filter.
};

/**
 * @brief Provided by Wisdom 0.7.1. Transform mode (AV1 Bitstream Specification Section 6.8.21).
 *
 * */
enum class StdVideoAV1TxMode {
    Only4x4 = 0, ///< Only 4x4 transforms.
    Largest = 1, ///< Largest allowed transform for the partition.
    Select = 2, ///< Select the transform mode.
    Invalid = 0x7FFFFFFF, ///< Invalid TxMode.
};

/**
 * @brief Provided by Wisdom 0.7.1. Loop restoration types (AV1 Bitstream Specification Section 6.10.15).
 *
 * */
enum class StdVideoAV1FrameRestorationType {
    None = 0, ///< No loop restoration.
    Wiener = 1, ///< Wiener filter loop restoration.
    Sgrproj = 2, ///< Self-guided filter loop restoration.
    Switchable = 3, ///< Switchable between Wiener and Sgrproj.
    Invalid = 0x7FFFFFFF, ///< Invalid restoration type.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 color primaries mapping to ISO/IEC 23000-2 / CICP.
 *
 * */
enum class StdVideoAV1ColorPrimaries {
    Bt709 = 1, ///< Rec. ITU-R BT.709-6.
    Unspecified = 2, ///< Image characteristics are unknown or unspecified.
    Bt470M = 4, ///< Rec. ITU-R BT.470-6 System M (historical).
    Bt470BG = 5, ///< Rec. ITU-R BT.470-6 System B, G (historical).
    Bt601 = 6, ///< Rec. ITU-R BT.601-7 525.
    Smpte240 = 7, ///< SMPTE 240M.
    GenericFilm = 8, ///< Generic film (color filters using Illuminant C).
    Bt2020 = 9, ///< Rec. ITU-R BT.2020-2.
    Xyz = 10, ///< SMPTE ST 428-1.
    Smpte431 = 11, ///< SMPTE RP 431-2.
    Smpte432 = 12, ///< SMPTE EG 432-1.
    Ebu3213 = 22, ///< EBU Tech. 3213-E.
    Invalid = 0x7FFFFFFF, ///< Invalid.
};

/**
 * @brief Provided by Wisdom 0.7.1. Transfer characteristics for AV1.
 *
 * */
enum class StdVideoAV1TransferCharacteristics {
    Reserved0 = 0,
    Bt709 = 1, ///< Rec. ITU-R BT.709-6.
    Unspecified = 2, ///< Unspecified.
    Reserved3 = 3,
    Bt470M = 4, ///< Rec. ITU-R BT.470-6 System M (historical).
    Bt470BG = 5, ///< Rec. ITU-R BT.470-6 System B, G (historical).
    Bt601 = 6, ///< Rec. ITU-R BT.601-7.
    Smpte240 = 7, ///< SMPTE 240M.
    Linear = 8, ///< Linear transfer characteristics.
    Log100 = 9, ///< Logarithmic transfer characteristic (100:1 range).
    Log100Sqrt10 = 10, ///< Logarithmic transfer characteristic (100 * Sqrt(10) : 1 range).
    Iec61966 = 11, ///< IEC 61966-2-4.
    Bt1361 = 12, ///< Rec. ITU-R BT.1361-0 extended colour gamut system (historical).
    Srgb = 13, ///< IEC 61966-2-1 sRGB.
    Bt2020_10Bit = 14, ///< Rec. ITU-R BT.2020-2 (10-bit system).
    Bt2020_12Bit = 15, ///< Rec. ITU-R BT.2020-2 (12-bit system).
    Smpte2084 = 16, ///< SMPTE ST 2084 (PQ).
    Smpte428 = 17, ///< SMPTE ST 428-1.
    Hlg = 18, ///< ARIB STD-B67 (HLG).
    Invalid = 0x7FFFFFFF,
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 matrix coefficients mapping to CICP.
 *
 * */
enum class StdVideoAV1MatrixCoefficients {
    Identity = 0, ///< Identity matrix.
    Bt709 = 1, ///< Rec. ITU-R BT.709-6.
    Unspecified = 2, ///< Matrix characteristics are unspecified.
    Reserved3 = 3,
    Fcc = 4, ///< FCC Title 47 Code of Federal Regulations.
    Bt470BG = 5, ///< Rec. ITU-R BT.470-6 System B, G (historical).
    Bt601 = 6, ///< Rec. ITU-R BT.601-7.
    Smpte240 = 7, ///< SMPTE 240M.
    SmpteYcgco = 8, ///< YCgCo.
    Bt2020Ncl = 9, ///< Bt2020 non-constant luminance.
    Bt2020Cl = 10, ///< Bt2020 constant luminance.
    Smpte2085 = 11, ///< SMPTE ST 2085.
    ChromatNcl = 12, ///< Chromaticity-derived non-constant luminance.
    ChromatCl = 13, ///< Chromaticity-derived constant luminance.
    Ictcp = 14, ///< Rec. ITU-R BT.2100-0 ICtCp.
    Invalid = 0x7FFFFFFF,
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 chroma sample position (AV1 Bitstream Specification Section 6.4.2).
 *
 * */
enum class StdVideoAV1ChromaSamplePosition {
    Unknown = 0, ///< Unknown chroma sample position.
    Vertical = 1, ///< Horizontally co-located with luma, vertically shifted by 0.5.
    Colocated = 2, ///< Co-located with luma.
    Reserved = 3,
    Invalid = 0x7FFFFFFF,
};

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
 * @brief Provided by Wisdom 0.7.1. Color configuration flags (AV1 Bitstream Specification 6.4.2).
 *
 * */
struct StdVideoAV1ColorConfigFlags {
    std::uint32_t mono_chrome         : 1; ///< Indicates if the video does not contain U and V color planes.
    std::uint32_t color_range         : 1; ///< Flag indicating if full color range is used.
    std::uint32_t separate_uv_delta_q : 1; ///< Flag indicating U and V planes have separate delta quantization.
    std::uint32_t color_description_present_flag : 1; ///< Indicates if color description is present.
    std::uint32_t reserved                       : 28;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Color Configuration (AV1 Bitstream Specification Section 6.4.2).
 *
 * */
struct StdVideoAV1ColorConfig {
    wis::StdVideoAV1ColorConfigFlags flags; ///< Color configuration flags.
    std::uint8_t BitDepth; ///< Bit depth of the color samples (8, 10, or 12).
    std::uint8_t subsampling_x; ///< Chroma subsampling x.
    std::uint8_t subsampling_y; ///< Chroma subsampling y.
    std::uint8_t reserved1;
    wis::StdVideoAV1ColorPrimaries color_primaries; ///< Color primaries.
    wis::StdVideoAV1TransferCharacteristics transfer_characteristics; ///< Transfer characteristics.
    wis::StdVideoAV1MatrixCoefficients matrix_coefficients; ///< Matrix coefficients.
    wis::StdVideoAV1ChromaSamplePosition chroma_sample_position; ///< Chroma sample position.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Timing Info flags.
 *
 * */
struct StdVideoAV1TimingInfoFlags {
    std::uint32_t equal_picture_interval : 1; ///< Indicates if pictures should be displayed with equal intervals.
    std::uint32_t reserved               : 31;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Timing Info (AV1 Bitstream Specification Section 6.4.3).
 *
 * */
struct StdVideoAV1TimingInfo {
    wis::StdVideoAV1TimingInfoFlags flags; ///< Timing flags.
    std::uint32_t num_units_in_display_tick; ///< Number of units in a display tick.
    std::uint32_t time_scale; ///< Time scale.
    std::uint32_t num_ticks_per_picture_minus_1; ///< Ticks per picture minus 1.
};

/**
 * @brief Provided by Wisdom 0.7.1. Loop filter flags (AV1 Bitstream Specification Section 6.8.10).
 *
 * */
struct StdVideoAV1LoopFilterFlags {
    /**
     * @brief Indicates whether the filter level depends on the mode and reference frame used to predict a block.
     * */
    std::uint32_t loop_filter_delta_enabled : 1;
    /**
     * @brief Indicates whether additional syntax elements are present that specify which mode and reference frame
     * deltas are to be updated.
     * */
    std::uint32_t loop_filter_delta_update : 1;
    std::uint32_t reserved                 : 30;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Loop Filter Parameters (AV1 Bitstream Specification Section 6.8.10).
 *
 * */
struct StdVideoAV1LoopFilter {
    wis::StdVideoAV1LoopFilterFlags flags; ///< Loop filter flags.
    std::array<std::uint8_t, 4> loop_filter_level; ///< Array containing loop filter strength values.
    std::uint8_t loop_filter_sharpness; ///< Loop filter sharpness.
    std::uint8_t update_ref_delta; ///< Indicates that the loop filter ref deltas are to be updated.
    std::array<std::int8_t, 8> loop_filter_ref_deltas; ///< Loop filter reference deltas.
    /**
     * @brief Indicates that the loop filter mode deltas are to be updated.
     * */
    std::uint8_t update_mode_delta;
    std::array<std::int8_t, 2> loop_filter_mode_deltas; ///< Loop filter mode deltas.
};

/**
 * @brief Provided by Wisdom 0.7.1. Quantization flags (AV1 Bitstream Specification Section 6.8.11).
 *
 * */
struct StdVideoAV1QuantizationFlags {
    std::uint32_t using_qmatrix : 1; ///< Specifies whether the quantizer matrix should be used.
    /**
     * @brief Specifies whether the U and V delta quantizer values are transmitted separately.
     * */
    std::uint32_t diff_uv_delta : 1;
    std::uint32_t reserved      : 30;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Quantization Parameters (AV1 Bitstream Specification Section 6.8.11).
 *
 * */
struct StdVideoAV1Quantization {
    wis::StdVideoAV1QuantizationFlags flags; ///< Quantization flags.
    std::uint8_t base_q_idx; ///< Indicates the base frame qindex.
    std::int8_t DeltaQYDc; ///< Y DC quantizer relative to base_q_idx.
    std::int8_t DeltaQUDc; ///< U DC quantizer relative to base_q_idx.
    std::int8_t DeltaQUAc; ///< U AC quantizer relative to base_q_idx.
    std::int8_t DeltaQVDc; ///< V DC quantizer relative to base_q_idx.
    std::int8_t DeltaQVAc; ///< V AC quantizer relative to base_q_idx.
    /**
     * @brief Specifies the level in the quantizer matrix that should be used for luma plane decoding.
     * */
    std::uint8_t qm_y;
    /**
     * @brief Specifies the level in the quantizer matrix that should be used for chroma U plane decoding.
     * */
    std::uint8_t qm_u;
    /**
     * @brief Specifies the level in the quantizer matrix that should be used for chroma V plane decoding.
     * */
    std::uint8_t qm_v;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Segmentation Parameters (AV1 Bitstream Specification Section 6.8.13).
 *
 * */
struct StdVideoAV1Segmentation {
    std::array<std::uint8_t, 8> FeatureEnabled; ///< Array specifying whether the feature is enabled for a segment.
    std::array<std::int16_t, 8 * 8> FeatureData; ///< Array specifying the feature data for a segment feature.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Tile Info Flags (AV1 Bitstream Specification Section 6.8.14).
 *
 * */
struct StdVideoAV1TileInfoFlags {
    std::uint32_t uniform_tile_spacing_flag : 1; ///< Indicates that the tiles are uniformly spaced across the picture.
    std::uint32_t reserved                  : 31;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Tile Information (AV1 Bitstream Specification Section 6.8.14).
 *
 * */
struct StdVideoAV1TileInfo {
    wis::StdVideoAV1TileInfoFlags flags; ///< Tile info flags.
    std::uint8_t TileCols; ///< Number of tiles across the picture.
    std::uint8_t TileRows; ///< Number of tiles down the picture.
    std::uint16_t context_update_tile_id; ///< Specifies which tile to use for the CDF update.
    /**
     * @brief Specifies the number of bytes needed to code each tile size.
     * */
    std::uint8_t tile_size_bytes_minus_1;
    std::array<std::uint8_t, 7> reserved1;
    /**
     * @brief Pointer to an array specifying the start column (in MI units) for each tile column.
     * */
    const std::uint16_t* pMiColStarts;
    /**
     * @brief Pointer to an array specifying the start row (in MI units) for each tile row.
     * */
    const std::uint16_t* pMiRowStarts;
    const std::uint16_t* pWidthInSbsMinus1; ///< Pointer to an array of tile widths in superblocks minus 1.
    const std::uint16_t* pHeightInSbsMinus1; ///< Pointer to an array of tile heights in superblocks minus 1.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Constrained Directional Enhancement Filter (CDEF) parameters (AV1 Bitstream
 * Specification Section 6.8.19).
 *
 * */
struct StdVideoAV1CDEF {
    std::uint8_t cdef_damping_minus_3; ///< Controls the amount of damping in the deringing filter.
    std::uint8_t cdef_bits; ///< Specifies the number of bits needed to specify the CDEF filter strength.
    std::array<std::uint8_t, 8> cdef_y_pri_strength; ///< Primary filter strength for Y.
    std::array<std::uint8_t, 8> cdef_y_sec_strength; ///< Secondary filter strength for Y.
    std::array<std::uint8_t, 8> cdef_uv_pri_strength; ///< Primary filter strength for UV.
    std::array<std::uint8_t, 8> cdef_uv_sec_strength; ///< Secondary filter strength for UV.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Loop Restoration parameters (AV1 Bitstream Specification Section 6.8.20).
 *
 * */
struct StdVideoAV1LoopRestoration {
    /**
     * @brief Array specifying the loop restoration type for each plane (Y, U, V).
     * */
    std::array<wis::StdVideoAV1FrameRestorationType, 3> FrameRestorationType;
    /**
     * @brief Array specifying the size of loop restoration units for each plane.
     * */
    std::array<std::uint16_t, 3> LoopRestorationSize;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Global Motion parameters (AV1 Bitstream Specification Section 6.8.17).
 *
 * */
struct StdVideoAV1GlobalMotion {
    std::array<std::uint8_t, 8> GmType; ///< Array specifying the global motion type for each reference frame.
    std::array<std::int32_t, 8 * 6> gm_params; ///< Array of global motion parameters.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Film Grain Flags (AV1 Bitstream Specification Section 6.8.24).
 *
 * */
struct StdVideoAV1FilmGrainFlags {
    std::uint32_t chroma_scaling_from_luma : 1; ///< Flag indicating that chroma scaling is derived from luma.
    std::uint32_t overlap_flag             : 1; ///< Flag indicating overlapping film grain blocks.
    std::uint32_t clip_to_restricted_range : 1; ///< Flag indicating clipping to restricted range.
    std::uint32_t update_grain : 1; ///< Flag indicating the film grain parameters are updated in this frame.
    std::uint32_t reserved     : 28;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Film Grain parameters (AV1 Bitstream Specification Section 6.8.24).
 *
 * */
struct StdVideoAV1FilmGrain {
    wis::StdVideoAV1FilmGrainFlags flags; ///< Film grain flags.
    std::uint8_t grain_scaling_minus_8; ///< Shift value for the film grain scale calculation.
    std::uint8_t ar_coeff_lag; ///< Number of auto-regressive coefficients.
    std::uint8_t ar_coeff_shift_minus_6; ///< Shift value for auto-regressive coefficients.
    /**
     * @brief Specifies how much the Gaussian random numbers should be scaled down.
     * */
    std::uint8_t grain_scale_shift;
    std::uint16_t grain_seed; ///< Specifies the seed for the pseudo-random number generator.
    /**
     * @brief Specifies the reference frame index to obtain the film grain parameters from.
     * */
    std::uint8_t film_grain_params_ref_idx;
    std::uint8_t num_y_points; ///< Number of points for luma scaling.
    std::array<std::uint8_t, 14> point_y_value; ///< Luma point values.
    std::array<std::uint8_t, 14> point_y_scaling; ///< Luma point scaling.
    std::uint8_t num_cb_points; ///< Number of points for Cb scaling.
    std::array<std::uint8_t, 10> point_cb_value; ///< Cb point values.
    std::array<std::uint8_t, 10> point_cb_scaling; ///< Cb point scaling.
    std::uint8_t num_cr_points; ///< Number of points for Cr scaling.
    std::array<std::uint8_t, 10> point_cr_value; ///< Cr point values.
    std::array<std::uint8_t, 10> point_cr_scaling; ///< Cr point scaling.
    std::array<std::int8_t, 24> ar_coeffs_y_plus_128; ///< Auto-regressive coefficients for Y.
    std::array<std::int8_t, 25> ar_coeffs_cb_plus_128; ///< Auto-regressive coefficients for Cb.
    std::array<std::int8_t, 25> ar_coeffs_cr_plus_128; ///< Auto-regressive coefficients for Cr.
    std::uint8_t cb_mult; ///< Cb multiplier for chroma scaling from luma.
    std::uint8_t cb_luma_mult; ///< Cb luma multiplier for chroma scaling from luma.
    std::uint16_t cb_offset; ///< Cb offset for chroma scaling from luma.
    std::uint8_t cr_mult; ///< Cr multiplier for chroma scaling from luma.
    std::uint8_t cr_luma_mult; ///< Cr luma multiplier for chroma scaling from luma.
    std::uint16_t cr_offset; ///< Cr offset for chroma scaling from luma.
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Sequence Header flags (AV1 Bitstream Specification Section 5.5).
 *
 * */
struct StdVideoAV1SequenceHeaderFlags {
    std::uint32_t still_picture : 1; ///< Specifies if the video sequence contains a single still picture.
    /**
     * @brief Specifies if reduced header parameters are used for a still picture.
     * */
    std::uint32_t reduced_still_picture_header : 1;
    std::uint32_t use_128x128_superblock       : 1; ///< Specifies if superblocks are 128x128 or 64x64.
    std::uint32_t enable_filter_intra          : 1; ///< Specifies if the filter intra predictor can be used.
    std::uint32_t enable_intra_edge_filter     : 1; ///< Specifies if intra edge filtering can be used.
    std::uint32_t enable_interintra_compound   : 1; ///< Specifies if inter-intra compound prediction can be used.
    std::uint32_t enable_masked_compound       : 1; ///< Specifies if masked compound prediction can be used.
    std::uint32_t enable_warped_motion         : 1; ///< Specifies if warped motion can be used.
    std::uint32_t enable_dual_filter           : 1; ///< Specifies if dual interpolation filters can be used.
    std::uint32_t enable_order_hint            : 1; ///< Specifies if order hints are used.
    std::uint32_t enable_jnt_comp : 1; ///< Specifies if the distance weights process is used for compound prediction.
    std::uint32_t enable_ref_frame_mvs               : 1; ///< Specifies if reference frame motion vectors are present.
    std::uint32_t frame_id_numbers_present_flag      : 1; ///< Specifies if frame ID numbers are present.
    std::uint32_t enable_superres                    : 1; ///< Specifies if the superresolution feature can be used.
    std::uint32_t enable_cdef                        : 1; ///< Specifies if the CDEF filtering process can be used.
    std::uint32_t enable_restoration                 : 1; ///< Specifies if loop restoration can be used.
    std::uint32_t film_grain_params_present          : 1; ///< Specifies if film grain parameters are present.
    std::uint32_t timing_info_present_flag           : 1; ///< Specifies if timing info is present.
    std::uint32_t initial_display_delay_present_flag : 1; ///< Specifies if the initial display delay info is present.
    std::uint32_t reserved                           : 13;
};

/**
 * @brief Provided by Wisdom 0.7.1. AV1 Sequence Header OBU parameters (AV1 Bitstream Specification Section 5.5).
 *
 * */
struct StdVideoAV1SequenceHeader {
    wis::StdVideoAV1SequenceHeaderFlags flags; ///< Sequence header flags.
    wis::StdVideoAV1Profile seq_profile; ///< AV1 profile.
    /**
     * @brief Number of bits used to specify the frame width minus 1.
     * */
    std::uint8_t frame_width_bits_minus_1;
    /**
     * @brief Number of bits used to specify the frame height minus 1.
     * */
    std::uint8_t frame_height_bits_minus_1;
    std::uint16_t max_frame_width_minus_1; ///< Maximum frame width minus 1.
    std::uint16_t max_frame_height_minus_1; ///< Maximum frame height minus 1.
    /**
     * @brief Specifies the number of bits used to encode delta_frame_id.
     * */
    std::uint8_t delta_frame_id_length_minus_2;
    /**
     * @brief Used to calculate the number of bits used to encode frame_id.
     * */
    std::uint8_t additional_frame_id_length_minus_1;
    std::uint8_t order_hint_bits_minus_1; ///< Used to compute OrderHintBits.
    std::uint8_t seq_force_integer_mv; ///< Equal to 1: motion vectors will always be integers.
    std::uint8_t seq_force_screen_content_tools; ///< Screen content tools setting.
    std::array<std::uint8_t, 5> reserved1;
    const wis::StdVideoAV1ColorConfig* pColorConfig; ///< Pointer to color configuration parameters.
    const wis::StdVideoAV1TimingInfo* pTimingInfo; ///< Pointer to timing info parameters.
};

/**
 * @brief Provided by Wisdom 0.7.1. Flags for AV1 decode picture info (from Uncompressed Header).
 *
 * */
struct StdVideoDecodeAV1PictureInfoFlags {
    std::uint32_t error_resilient_mode : 1; ///< Indicates error resilient mode is enabled.
    std::uint32_t disable_cdf_update   : 1; ///< Indicates CDF update is disabled.
    std::uint32_t use_superres         : 1; ///< Indicates superresolution is enabled for this frame.
    /**
     * @brief Indicates actual frame size and render frame size are different.
     * */
    std::uint32_t render_and_frame_size_different  : 1;
    std::uint32_t allow_screen_content_tools       : 1; ///< Indicates screen content tools are allowed.
    std::uint32_t is_filter_switchable             : 1; ///< Indicates whether interpolation filter is switchable.
    std::uint32_t force_integer_mv                 : 1; ///< Indicates whether motion vectors must be forced to integer.
    std::uint32_t frame_size_override_flag         : 1; ///< Indicates if frame size override is set.
    std::uint32_t buffer_removal_time_present_flag : 1; ///< Indicates whether buffer removal time is present.
    std::uint32_t allow_intrabc                    : 1; ///< Indicates if intra block copy is allowed.
    /**
     * @brief Indicates if reference frames are completely decided by last_frame_idx.
     * */
    std::uint32_t frame_refs_short_signaling   : 1;
    std::uint32_t allow_high_precision_mv      : 1; ///< Indicates whether high precision motion vectors are allowed.
    std::uint32_t is_motion_mode_switchable    : 1; ///< Indicates whether motion mode is switchable.
    std::uint32_t use_ref_frame_mvs            : 1; ///< Indicates whether reference frame MVs are used.
    std::uint32_t disable_frame_end_update_cdf : 1; ///< Specifies whether the frame end CDF update is skipped.
    std::uint32_t allow_warped_motion          : 1; ///< Indicates whether warped motion is allowed for this frame.
    std::uint32_t reduced_tx_set               : 1; ///< Indicates whether the frame uses a reduced transform set.
    /**
     * @brief Specifies that the mode info for inter blocks contains the syntax element comp_mode.
     * */
    std::uint32_t reference_select        : 1;
    std::uint32_t skip_mode_present       : 1; ///< Specifies whether skip mode is allowed.
    std::uint32_t delta_q_present         : 1; ///< Specifies whether a delta q index is present for the frame.
    std::uint32_t delta_lf_present        : 1; ///< Specifies whether delta loop filter values are present.
    std::uint32_t delta_lf_multi          : 1; ///< Specifies whether independent delta loop filter values are used.
    std::uint32_t segmentation_enabled    : 1; ///< Indicates if segmentation is enabled.
    std::uint32_t segmentation_update_map : 1; ///< Indicates if segmentation map is updated.
    std::uint32_t segmentation_temporal_update : 1; ///< Indicates if temporal segmentation is updated.
    std::uint32_t segmentation_update_data     : 1; ///< Indicates if segmentation feature data is updated.
    std::uint32_t UsesLr                       : 1; ///< Indicates if loop restoration is used.
    std::uint32_t usesChromaLr                 : 1; ///< Indicates if loop restoration is used for chroma.
    std::uint32_t apply_grain                  : 1; ///< Indicates if film grain should be applied.
    std::uint32_t reserved                     : 3;
};

/**
 * @brief Provided by Wisdom 0.7.1. Information provided by the application to the video decoder for each AV1 picture
 * (Khronos Video extensions).
 *
 * */
struct StdVideoDecodeAV1PictureInfo {
    wis::StdVideoDecodeAV1PictureInfoFlags flags; ///< Decode picture info flags.
    wis::StdVideoAV1FrameType frame_type; ///< Frame type: Key, Inter, Intra-only, or Switch.
    std::uint32_t current_frame_id; ///< Specifies the frame ID for the current frame.
    /**
     * @brief Order hint of the current frame used for motion vector scaling.
     * */
    std::uint8_t OrderHint;
    /**
     * @brief Index of the reference frame containing the CDF values to be loaded at the start of the frame.
     * */
    std::uint8_t primary_ref_frame;
    /**
     * @brief An 8-bit mask that specifies which reference frame slots will be updated with the current frame.
     * */
    std::uint8_t refresh_frame_flags;
    std::uint8_t reserved1;
    /**
     * @brief Specifies the filter selection used for performing inter prediction.
     * */
    wis::StdVideoAV1InterpolationFilter interpolation_filter;
    wis::StdVideoAV1TxMode TxMode; ///< Specifies how the transform size is determined.
    /**
     * @brief Specifies the left shift to be applied to decoded delta q values.
     * */
    std::uint8_t delta_q_res;
    /**
     * @brief Specifies the left shift to be applied to decoded delta loop filter values.
     * */
    std::uint8_t delta_lf_res;
    /**
     * @brief Specifies the indices of the reference frames to be used for skip mode.
     * */
    std::array<std::uint8_t, 2> SkipModeFrame;
    /**
     * @brief Denominator for frame size calculation if superres is enabled.
     * */
    std::uint8_t coded_denom;
    std::array<std::uint8_t, 3> reserved2;
    std::array<std::uint8_t, 8> OrderHints; ///< Order hints of the decoded reference frames.
    std::array<std::uint32_t, 8> expectedFrameId; ///< Expected frame IDs for reference frames.
    const wis::StdVideoAV1TileInfo* pTileInfo; ///< Pointer to AV1 tile information.
    const wis::StdVideoAV1Quantization* pQuantization; ///< Pointer to standard quantization matrices and values.
    const wis::StdVideoAV1Segmentation* pSegmentation; ///< Pointer to segmentation parameter information.
    const wis::StdVideoAV1LoopFilter* pLoopFilter; ///< Pointer to loop filter parameters.
    const wis::StdVideoAV1CDEF* pCDEF; ///< Pointer to CDEF parameters.
    const wis::StdVideoAV1LoopRestoration* pLoopRestoration; ///< Pointer to loop restoration parameters.
    const wis::StdVideoAV1GlobalMotion* pGlobalMotion; ///< Pointer to global motion parameters.
    const wis::StdVideoAV1FilmGrain* pFilmGrain; ///< Pointer to film grain synthesis parameters.
};

/**
 * @brief Provided by Wisdom 0.7.1. Flags for AV1 Decode Reference Information.
 *
 * */
struct StdVideoDecodeAV1ReferenceInfoFlags {
    std::uint32_t disable_frame_end_update_cdf : 1; ///< Reference originally had disabled frame end CDF update.
    std::uint32_t segmentation_enabled         : 1; ///< Reference originally had segmentation enabled.
    std::uint32_t reserved                     : 30;
};

/**
 * @brief Provided by Wisdom 0.7.1. Information provided by the application about an AV1 reference frame.
 *
 * */
struct StdVideoDecodeAV1ReferenceInfo {
    wis::StdVideoDecodeAV1ReferenceInfoFlags flags; ///< Reference information flags.
    std::uint8_t frame_type; ///< Frame type of the reference frame.
    /**
     * @brief Specifies the direction of the reference frame relative to other references used in motion vector
     * derivation.
     * */
    std::uint8_t RefFrameSignBias;
    std::uint8_t OrderHint; ///< Order hint of the reference frame.
    /**
     * @brief Saved order hints when this reference frame was decoded.
     * */
    std::array<std::uint8_t, 8> SavedOrderHints;
};

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
    wis::DataFormat image_format;
    std::uint32_t width; ///< Max width of the video frame in pixels.
    std::uint32_t height; ///< Max height of the video frame in pixels.
};

/**
 * @brief Provided by Wisdom 0.7.1. Information about a video decode operation.
 *
 * */
struct VideoDecoderDesc {
    std::uint32_t width; ///< Width of the video frame in pixels.
    std::uint32_t height; ///< Height of the video frame in pixels.
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
/**
 * @brief Provided by Wisdom 0.7.1. Variant type for video decode input descriptions. Used to specify the type of input
 * data for a video decode operation.
 *
 * */
struct DX12VideoDecodeInputDesc {
    /**
     * @brief Input description for a video decode operation that uses a bitstream buffer as input. The buffer view
     * should contain the compressed video data to be decoded.
     * */
    wis::DX12BufferView bitstream_buffer;
    std::uint64_t offset; ///< Offset in the buffer where the bistream data is located.
    std::uint64_t size; ///< Size of the bitstream data in bytes.
};

struct DX12VideoDecoderParametersDeleter {
    void operator()(WisDX12VideoDecoderParameters* handle) noexcept { ::wisDX12DestroyVideoDecoderParameters(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for video decoder parameters. Represents the parameters and capabilities of a
 * video decoder, such as supported codecs, bit depths, and chroma subsampling formats.
 *
 * */
class DX12VideoDecoderParameters : public wis::impl::Implements<
                                       wis::impl::DX12VideoDecoderParametersImpl,
                                       WisDX12VideoDecoderParameters,
                                       wis::DX12VideoDecoderParametersDeleter>
{
public:
    using ImplType::ImplType;

public:
};

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

struct DX12VideoDecodeCommandListDeleter {
    void operator()(WisDX12VideoDecodeCommandList* handle) noexcept { ::wisDX12DestroyVideoDecodeCommandList(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video command list. Represents a command list that can be used to
 * record video decode commands.
 *
 * */
class DX12VideoDecodeCommandList : public wis::impl::Implements<
                                       wis::impl::DX12VideoDecodeCommandListImpl,
                                       WisDX12VideoDecodeCommandList,
                                       wis::DX12VideoDecodeCommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Begin() const noexcept
    {
        const WisResult wis_result = ::wisDX12VideoDecodeCommandListBegin(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result End() const noexcept
    {
        const WisResult wis_result = ::wisDX12VideoDecodeCommandListEnd(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Records a video decode command to the command list.
     * @param decoder The video decoder that will be used for decoding the video frame.
     * @param input_desc Description of the input data for the video decode operation. This field specifies the type and
     * location of the input data that will be used for decoding the video frame.
     *
     * */
    inline void DecodeFrame(
        const wis::DX12VideoDecoder& decoder,
        const wis::DX12VideoDecodeInputDesc& input_desc
    ) const noexcept
    {
        ::wisDX12VideoDecodeCommandListDecodeFrame(
            &_impl_storage,
            reinterpret_cast<const WisDX12VideoDecoder*>(&decoder),
            reinterpret_cast<const WisDX12VideoDecodeInputDesc*>(&input_desc)
        );
    }
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
    /**
     * @brief Provided by Wisdom 0.7.1. Creates a video command list instance.
     * @param command_allocator The command allocator that the command list will use for memory management of command
     * buffers. It @wis_must be created with the same wis::Device as the extension and have
     * `wis::CommandQueueType::VideoDecode` or `wis::CommandQueueType::VideoEncode` specified.
     * @param out_result denoting the outcome of operation.
     * @return command_list Output parameter that holds the created video command list handle if the operation is
     * successful.
     *
     * */
    WIS_NODISCARD inline wis::DX12VideoDecodeCommandList CreateCommandList(
        const wis::DX12CommandAllocator& command_allocator,
        wis::Result& out_result
    ) noexcept
    {
        wis::DX12VideoDecodeCommandList command_list{};
        const WisResult wis_result = ::wisDX12VideoDecodingExtensionCreateCommandList(
            &_impl_storage,
            reinterpret_cast<const WisDX12CommandAllocator*>(&command_allocator),
            command_list.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return command_list;
    }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <video/vulkan/vk_types.hpp>

namespace wis {
/**
 * @brief Provided by Wisdom 0.7.1. Variant type for video decode input descriptions. Used to specify the type of input
 * data for a video decode operation.
 *
 * */
struct VKVideoDecodeInputDesc {
    /**
     * @brief Input description for a video decode operation that uses a bitstream buffer as input. The buffer view
     * should contain the compressed video data to be decoded.
     * */
    wis::VKBufferView bitstream_buffer;
    std::uint64_t offset; ///< Offset in the buffer where the bistream data is located.
    std::uint64_t size; ///< Size of the bitstream data in bytes.
};

struct VKVideoDecoderParametersDeleter {
    void operator()(WisVKVideoDecoderParameters* handle) noexcept { ::wisVKDestroyVideoDecoderParameters(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for video decoder parameters. Represents the parameters and capabilities of a
 * video decoder, such as supported codecs, bit depths, and chroma subsampling formats.
 *
 * */
class VKVideoDecoderParameters : public wis::impl::Implements<
                                     wis::impl::VKVideoDecoderParametersImpl,
                                     WisVKVideoDecoderParameters,
                                     wis::VKVideoDecoderParametersDeleter>
{
public:
    using ImplType::ImplType;

public:
};

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

struct VKVideoDecodeCommandListDeleter {
    void operator()(WisVKVideoDecodeCommandList* handle) noexcept { ::wisVKDestroyVideoDecodeCommandList(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for a video command list. Represents a command list that can be used to
 * record video decode commands.
 *
 * */
class VKVideoDecodeCommandList : public wis::impl::Implements<
                                     wis::impl::VKVideoDecodeCommandListImpl,
                                     WisVKVideoDecodeCommandList,
                                     wis::VKVideoDecodeCommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Begin() const noexcept
    {
        const WisResult wis_result = ::wisVKVideoDecodeCommandListBegin(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result End() const noexcept
    {
        const WisResult wis_result = ::wisVKVideoDecodeCommandListEnd(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Records a video decode command to the command list.
     * @param decoder The video decoder that will be used for decoding the video frame.
     * @param input_desc Description of the input data for the video decode operation. This field specifies the type and
     * location of the input data that will be used for decoding the video frame.
     *
     * */
    inline void DecodeFrame(
        const wis::VKVideoDecoder& decoder,
        const wis::VKVideoDecodeInputDesc& input_desc
    ) const noexcept
    {
        ::wisVKVideoDecodeCommandListDecodeFrame(
            &_impl_storage,
            reinterpret_cast<const WisVKVideoDecoder*>(&decoder),
            reinterpret_cast<const WisVKVideoDecodeInputDesc*>(&input_desc)
        );
    }
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
    /**
     * @brief Provided by Wisdom 0.7.1. Creates a video command list instance.
     * @param command_allocator The command allocator that the command list will use for memory management of command
     * buffers. It @wis_must be created with the same wis::Device as the extension and have
     * `wis::CommandQueueType::VideoDecode` or `wis::CommandQueueType::VideoEncode` specified.
     * @param out_result denoting the outcome of operation.
     * @return command_list Output parameter that holds the created video command list handle if the operation is
     * successful.
     *
     * */
    WIS_NODISCARD inline wis::VKVideoDecodeCommandList CreateCommandList(
        const wis::VKCommandAllocator& command_allocator,
        wis::Result& out_result
    ) noexcept
    {
        wis::VKVideoDecodeCommandList command_list{};
        const WisResult wis_result = ::wisVKVideoDecodingExtensionCreateCommandList(
            &_impl_storage,
            reinterpret_cast<const WisVKCommandAllocator*>(&command_allocator),
            command_list.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return command_list;
    }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_VIDEO_CPP_API_HPP
