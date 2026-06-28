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
    uint8_t superres_denom = 0; // comes from frame header
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

struct DX12H265DecoderParameters {
    uint32_t max_vps_count;
    uint32_t max_sps_count;
    uint32_t max_pps_count;

    // SPS-derived fields (from first SPS entry, or defaults)
    uint16_t pic_width_in_min_cbs_y;
    uint16_t pic_height_in_min_cbs_y;
    uint8_t chroma_format_idc;
    uint8_t separate_colour_plane_flag;
    uint8_t bit_depth_luma_minus8;
    uint8_t bit_depth_chroma_minus8;
    uint8_t log2_min_luma_coding_block_size_minus3;
    uint8_t log2_diff_max_min_luma_coding_block_size;
    uint8_t log2_min_luma_transform_block_size_minus2;
    uint8_t log2_diff_max_min_luma_transform_block_size;
    uint8_t max_transform_hierarchy_depth_inter;
    uint8_t max_transform_hierarchy_depth_intra;
    uint8_t num_short_term_ref_pic_sets;
    uint8_t num_long_term_ref_pics_sps;
    uint8_t log2_max_pic_order_cnt_lsb_minus4;
    uint8_t sps_max_sub_layers_minus1;

    // SPS + some PPS coding tool flags (dwCodingParamToolFlags in DXVA_PicParams_HEVC)
    union {
        struct {
            uint32_t scaling_list_enabled_flag                 : 1;
            uint32_t amp_enabled_flag                         : 1;
            uint32_t sample_adaptive_offset_enabled_flag      : 1;
            uint32_t pcm_enabled_flag                         : 1;
            uint32_t pcm_sample_bit_depth_luma_minus1         : 4;
            uint32_t pcm_sample_bit_depth_chroma_minus1       : 4;
            uint32_t log2_min_pcm_luma_coding_block_size_minus3 : 2;
            uint32_t log2_diff_max_min_pcm_luma_coding_block_size : 2;
            uint32_t pcm_loop_filter_disabled_flag             : 1;
            uint32_t long_term_ref_pics_present_flag          : 1;
            uint32_t sps_temporal_mvp_enabled_flag            : 1;
            uint32_t strong_intra_smoothing_enabled_flag      : 1;
            uint32_t dependent_slice_segments_enabled_flag    : 1;
            uint32_t output_flag_present_flag                 : 1;
            uint32_t num_extra_slice_header_bits              : 3;
            uint32_t sign_data_hiding_enabled_flag            : 1;
            uint32_t cabac_init_present_flag                  : 1;
            uint32_t reserved1                                : 5;
        };
        uint32_t coding_param_tool_flags;
    };

    // PPS-derived fields (dwCodingSettingPicturePropertyFlags in DXVA_PicParams_HEVC)
    union {
        struct {
            uint32_t constrained_intra_pred_flag                : 1;
            uint32_t transform_skip_enabled_flag                : 1;
            uint32_t cu_qp_delta_enabled_flag                   : 1;
            uint32_t pps_slice_chroma_qp_offsets_present_flag   : 1;
            uint32_t weighted_pred_flag                         : 1;
            uint32_t weighted_bipred_flag                       : 1;
            uint32_t transquant_bypass_enabled_flag             : 1;
            uint32_t tiles_enabled_flag                         : 1;
            uint32_t entropy_coding_sync_enabled_flag           : 1;
            uint32_t uniform_spacing_flag                       : 1;
            uint32_t loop_filter_across_tiles_enabled_flag      : 1;
            uint32_t pps_loop_filter_across_slices_enabled_flag : 1;
            uint32_t deblocking_filter_override_enabled_flag    : 1;
            uint32_t pps_deblocking_filter_disabled_flag        : 1;
            uint32_t lists_modification_present_flag            : 1;
            uint32_t slice_segment_header_extension_present_flag : 1;
            uint32_t reserved2                                  : 16;
        };
        uint32_t picture_property_flags;
    };

    int8_t pps_cb_qp_offset;
    int8_t pps_cr_qp_offset;
    uint8_t num_tile_columns_minus1;
    uint8_t num_tile_rows_minus1;
    uint8_t diff_cu_qp_delta_depth;
    int8_t pps_beta_offset_div2;
    int8_t pps_tc_offset_div2;
    uint8_t log2_parallel_merge_level_minus2;
    uint8_t num_ref_idx_l0_default_active_minus1;
    uint8_t num_ref_idx_l1_default_active_minus1;
    int8_t init_qp_minus26;

    uint32_t sps_seq_parameter_set_id;
    uint32_t pps_pic_parameter_set_id;

    DX12H265DecoderParameters(const WisVideoDecodeH265Desc& desc) noexcept
        : max_vps_count(desc.max_vps_count)
        , max_sps_count(desc.max_sps_count)
        , max_pps_count(desc.max_pps_count)
    {
        // Extract from first SPS if available
        if (desc.sps && desc.sps_count > 0) {
            auto& sps = desc.sps[0];
            chroma_format_idc = static_cast<uint8_t>(sps.chroma_format_idc);
            bit_depth_luma_minus8 = sps.bit_depth_luma_minus8;
            bit_depth_chroma_minus8 = sps.bit_depth_chroma_minus8;
            log2_min_luma_coding_block_size_minus3 = sps.log2_min_luma_coding_block_size_minus3;
            log2_diff_max_min_luma_coding_block_size = sps.log2_diff_max_min_luma_coding_block_size;
            log2_min_luma_transform_block_size_minus2 = sps.log2_min_luma_transform_block_size_minus2;
            log2_diff_max_min_luma_transform_block_size = sps.log2_diff_max_min_luma_transform_block_size;
            max_transform_hierarchy_depth_inter = sps.max_transform_hierarchy_depth_inter;
            max_transform_hierarchy_depth_intra = sps.max_transform_hierarchy_depth_intra;
            num_short_term_ref_pic_sets = sps.num_short_term_ref_pic_sets;
            num_long_term_ref_pics_sps = sps.num_long_term_ref_pics_sps;
            log2_max_pic_order_cnt_lsb_minus4 = sps.log2_max_pic_order_cnt_lsb_minus4;
            sps_max_sub_layers_minus1 = sps.sps_max_sub_layers_minus1;
            sps_seq_parameter_set_id = sps.sps_seq_parameter_set_id;

            // Compute PicWidthInMinCbsY = ceil(pic_width_in_luma_samples / MinCbSizeY)
            // MinCbSizeY = 1 << (log2_min_luma_coding_block_size_minus3 + 3)
            uint8_t min_cb_log2 = sps.log2_min_luma_coding_block_size_minus3 + 3;
            uint32_t min_cb_size = 1u << min_cb_log2;
            pic_width_in_min_cbs_y = static_cast<uint16_t>((sps.pic_width_in_luma_samples + min_cb_size - 1) / min_cb_size);
            pic_height_in_min_cbs_y = static_cast<uint16_t>((sps.pic_height_in_luma_samples + min_cb_size - 1) / min_cb_size);

            // SPS flags -> coding param tool flags
            separate_colour_plane_flag = sps.flags.separate_colour_plane_flag;
            scaling_list_enabled_flag = sps.flags.scaling_list_enabled_flag;
            amp_enabled_flag = sps.flags.amp_enabled_flag;
            sample_adaptive_offset_enabled_flag = sps.flags.sample_adaptive_offset_enabled_flag;
            pcm_enabled_flag = sps.flags.pcm_enabled_flag;
            pcm_sample_bit_depth_luma_minus1 = sps.pcm_sample_bit_depth_luma_minus1;
            pcm_sample_bit_depth_chroma_minus1 = sps.pcm_sample_bit_depth_chroma_minus1;
            log2_min_pcm_luma_coding_block_size_minus3 = sps.log2_min_pcm_luma_coding_block_size_minus3;
            log2_diff_max_min_pcm_luma_coding_block_size = sps.log2_diff_max_min_pcm_luma_coding_block_size;
            pcm_loop_filter_disabled_flag = sps.flags.pcm_loop_filter_disabled_flag;
            long_term_ref_pics_present_flag = sps.flags.long_term_ref_pics_present_flag;
            sps_temporal_mvp_enabled_flag = sps.flags.sps_temporal_mvp_enabled_flag;
            strong_intra_smoothing_enabled_flag = sps.flags.strong_intra_smoothing_enabled_flag;
        }

        // Extract from first PPS if available
        if (desc.pps && desc.pps_count > 0) {
            auto& pps = desc.pps[0];

            // dwCodingParamToolFlags (PPS-derived)
            dependent_slice_segments_enabled_flag = pps.flags.dependent_slice_segments_enabled_flag;
            output_flag_present_flag = pps.flags.output_flag_present_flag;
            sign_data_hiding_enabled_flag = pps.flags.sign_data_hiding_enabled_flag;
            cabac_init_present_flag = pps.flags.cabac_init_present_flag;
            num_extra_slice_header_bits = pps.num_extra_slice_header_bits;

            // dwCodingSettingPicturePropertyFlags (PPS-derived)
            constrained_intra_pred_flag = pps.flags.constrained_intra_pred_flag;
            transform_skip_enabled_flag = pps.flags.transform_skip_enabled_flag;
            cu_qp_delta_enabled_flag = pps.flags.cu_qp_delta_enabled_flag;
            pps_slice_chroma_qp_offsets_present_flag = pps.flags.pps_slice_chroma_qp_offsets_present_flag;
            weighted_pred_flag = pps.flags.weighted_pred_flag;
            weighted_bipred_flag = pps.flags.weighted_bipred_flag;
            transquant_bypass_enabled_flag = pps.flags.transquant_bypass_enabled_flag;
            tiles_enabled_flag = pps.flags.tiles_enabled_flag;
            entropy_coding_sync_enabled_flag = pps.flags.entropy_coding_sync_enabled_flag;
            uniform_spacing_flag = pps.flags.uniform_spacing_flag;
            loop_filter_across_tiles_enabled_flag = pps.flags.loop_filter_across_tiles_enabled_flag;
            pps_loop_filter_across_slices_enabled_flag = pps.flags.pps_loop_filter_across_slices_enabled_flag;
            deblocking_filter_override_enabled_flag = pps.flags.deblocking_filter_override_enabled_flag;
            pps_deblocking_filter_disabled_flag = pps.flags.pps_deblocking_filter_disabled_flag;
            lists_modification_present_flag = pps.flags.lists_modification_present_flag;
            slice_segment_header_extension_present_flag = pps.flags.slice_segment_header_extension_present_flag;

            // PPS scalar fields
            pps_cb_qp_offset = pps.pps_cb_qp_offset;
            pps_cr_qp_offset = pps.pps_cr_qp_offset;
            num_tile_columns_minus1 = pps.num_tile_columns_minus1;
            num_tile_rows_minus1 = pps.num_tile_rows_minus1;
            diff_cu_qp_delta_depth = pps.diff_cu_qp_delta_depth;
            pps_beta_offset_div2 = pps.pps_beta_offset_div2;
            pps_tc_offset_div2 = pps.pps_tc_offset_div2;
            log2_parallel_merge_level_minus2 = pps.log2_parallel_merge_level_minus2;
            num_ref_idx_l0_default_active_minus1 = pps.num_ref_idx_l0_default_active_minus1;
            num_ref_idx_l1_default_active_minus1 = pps.num_ref_idx_l1_default_active_minus1;
            init_qp_minus26 = pps.init_qp_minus26;
            pps_pic_parameter_set_id = pps.pps_pic_parameter_set_id;
        }
    }
};

} // namespace wis::detail

#endif // __cplusplus
