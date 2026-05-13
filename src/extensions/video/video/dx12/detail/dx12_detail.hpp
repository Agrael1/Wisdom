#ifndef WIS_DX12_VIDEO_DETAIL_HPP
#define WIS_DX12_VIDEO_DETAIL_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <dxva.h>
#include <video/generated/c_api.h>

namespace wis::detail {
struct DX12AV1DecoderParameters {
    uint32_t width;
    uint32_t height;

    uint32_t max_width;
    uint32_t max_height;

    uint8_t CurrPicTextureIndex = 0; // Set from output descriptor in DecodeFrame
    uint8_t superres_denom; // comes from frame header
    uint8_t bitdepth;
    uint8_t seq_profile;

    /* Tiles are frame header */
    // Coding Tools
    union {
        struct {
            uint32_t use_128x128_superblock       : 1;
            uint32_t intra_edge_filter            : 1;
            uint32_t interintra_compound          : 1;
            uint32_t masked_compound              : 1;
            uint32_t warped_motion                : 1;
            uint32_t dual_filter                  : 1;
            uint32_t jnt_comp                     : 1;
            uint32_t screen_content_tools         : 1; /* Frame Header */
            uint32_t integer_mv                   : 1; /* Decoder? */
            uint32_t cdef                         : 1;
            uint32_t restoration                  : 1;
            uint32_t film_grain                   : 1;
            uint32_t intrabc                      : 1; /* Frame Header begin */
            uint32_t high_precision_mv            : 1;
            uint32_t switchable_motion_mode       : 1;
            uint32_t filter_intra                 : 1; /* Seqence Header */
            uint32_t disable_frame_end_update_cdf : 1;
            uint32_t disable_cdf_update           : 1;
            uint32_t reference_mode               : 1;
            uint32_t skip_mode                    : 1;
            uint32_t reduced_tx_set               : 1;
            uint32_t superres                     : 1;
            uint32_t tx_mode                      : 2;
            uint32_t use_ref_frame_mvs            : 1; /* Frame Header end */
            uint32_t enable_ref_frame_mvs         : 1;
            uint32_t reference_frame_update       : 1;
            uint32_t Reserved                     : 5;
        };
        uint32_t CodingParamToolFlags;
    } coding;

    // Format & Picture Info flags
    union {
        struct {
            uint8_t frame_type     : 2; /* Frame Header begin */
            uint8_t show_frame     : 1;
            uint8_t showable_frame : 1; /* Frame Header end */
            uint8_t subsampling_x  : 1;
            uint8_t subsampling_y  : 1;
            uint8_t mono_chrome    : 1;
            uint8_t Reserved       : 1;
        };
        uint8_t FormatAndPictureInfoFlags;
    } format;

    uint8_t order_hint_bits;

public:
    DX12AV1DecoderParameters(const WisStdVideoAV1SequenceHeader& header) noexcept
        : width(header.frame_width_bits_minus_1 + 1)
        , height(header.frame_height_bits_minus_1 + 1)
        , max_width(header.max_frame_width_minus_1 + 1)
        , max_height(header.max_frame_height_minus_1 + 1)

        , bitdepth(header.pColorConfig ? header.pColorConfig->BitDepth : 8)
        , seq_profile(header.seq_profile)
        , coding{
              .use_128x128_superblock = header.flags.use_128x128_superblock,
              .intra_edge_filter = header.flags.enable_intra_edge_filter,
              .interintra_compound = header.flags.enable_interintra_compound,
              .masked_compound = header.flags.enable_masked_compound,
              .warped_motion = header.flags.enable_warped_motion,
              .dual_filter = header.flags.enable_dual_filter,
              .jnt_comp = header.flags.enable_jnt_comp,

              .cdef = header.flags.enable_cdef,
              .restoration = header.flags.enable_restoration,
              .film_grain = header.flags.film_grain_params_present,

              .filter_intra = header.flags.enable_filter_intra,
              
              .enable_ref_frame_mvs = header.flags.enable_ref_frame_mvs,
              .reference_frame_update = 1, // This is a guess based on available flags, adjust as necessary
        }
        ,format{
              .subsampling_x = header.pColorConfig ? header.pColorConfig->subsampling_x : uint8_t(0),
              .subsampling_y = header.pColorConfig ? header.pColorConfig->subsampling_y : uint8_t(0),
              .mono_chrome = header.pColorConfig ? uint8_t(header.pColorConfig->flags.mono_chrome) : uint8_t(0),
        }
        , order_hint_bits(header.order_hint_bits_minus_1 + 1)

    {}
};

} // namespace wis::detail

#endif // __cplusplus
