/**
 * @struct WisStdVideoH265SpsFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265SpsFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265SpsFlags {
 *     uint32_t sps_temporal_id_nesting_flag : 1;
 *     uint32_t separate_colour_plane_flag : 1;
 *     uint32_t conformance_window_flag : 1;
 *     uint32_t sps_sub_layer_ordering_info_present_flag : 1;
 *     uint32_t scaling_list_enabled_flag : 1;
 *     uint32_t sps_scaling_list_data_present_flag : 1;
 *     uint32_t amp_enabled_flag : 1;
 *     uint32_t sample_adaptive_offset_enabled_flag : 1;
 *     uint32_t pcm_enabled_flag : 1;
 *     uint32_t pcm_loop_filter_disabled_flag : 1;
 *     uint32_t long_term_ref_pics_present_flag : 1;
 *     uint32_t sps_temporal_mvp_enabled_flag : 1;
 *     uint32_t strong_intra_smoothing_enabled_flag : 1;
 *     uint32_t vui_parameters_present_flag : 1;
 *     uint32_t sps_extension_present_flag : 1;
 *     uint32_t sps_range_extension_flag : 1;
 *     uint32_t transform_skip_rotation_enabled_flag : 1;
 *     uint32_t transform_skip_context_enabled_flag : 1;
 *     uint32_t implicit_rdpcm_enabled_flag : 1;
 *     uint32_t explicit_rdpcm_enabled_flag : 1;
 *     uint32_t extended_precision_processing_flag : 1;
 *     uint32_t intra_smoothing_disabled_flag : 1;
 *     uint32_t high_precision_offsets_enabled_flag : 1;
 *     uint32_t persistent_rice_adaptation_enabled_flag : 1;
 *     uint32_t cabac_bypass_alignment_enabled_flag : 1;
 *     uint32_t sps_scc_extension_flag : 1;
 *     uint32_t sps_curr_pic_ref_enabled_flag : 1;
 *     uint32_t palette_mode_enabled_flag : 1;
 *     uint32_t sps_palette_predictor_initializers_present_flag : 1;
 *     uint32_t intra_boundary_filtering_disabled_flag : 1;
 *     uint32_t reserved : 2;
 * } WisStdVideoH265SpsFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265SpsFlags {
 *     std::uint32_t sps_temporal_id_nesting_flag : 1;
 *     std::uint32_t separate_colour_plane_flag : 1;
 *     std::uint32_t conformance_window_flag : 1;
 *     std::uint32_t sps_sub_layer_ordering_info_present_flag : 1;
 *     std::uint32_t scaling_list_enabled_flag : 1;
 *     std::uint32_t sps_scaling_list_data_present_flag : 1;
 *     std::uint32_t amp_enabled_flag : 1;
 *     std::uint32_t sample_adaptive_offset_enabled_flag : 1;
 *     std::uint32_t pcm_enabled_flag : 1;
 *     std::uint32_t pcm_loop_filter_disabled_flag : 1;
 *     std::uint32_t long_term_ref_pics_present_flag : 1;
 *     std::uint32_t sps_temporal_mvp_enabled_flag : 1;
 *     std::uint32_t strong_intra_smoothing_enabled_flag : 1;
 *     std::uint32_t vui_parameters_present_flag : 1;
 *     std::uint32_t sps_extension_present_flag : 1;
 *     std::uint32_t sps_range_extension_flag : 1;
 *     std::uint32_t transform_skip_rotation_enabled_flag : 1;
 *     std::uint32_t transform_skip_context_enabled_flag : 1;
 *     std::uint32_t implicit_rdpcm_enabled_flag : 1;
 *     std::uint32_t explicit_rdpcm_enabled_flag : 1;
 *     std::uint32_t extended_precision_processing_flag : 1;
 *     std::uint32_t intra_smoothing_disabled_flag : 1;
 *     std::uint32_t high_precision_offsets_enabled_flag : 1;
 *     std::uint32_t persistent_rice_adaptation_enabled_flag : 1;
 *     std::uint32_t cabac_bypass_alignment_enabled_flag : 1;
 *     std::uint32_t sps_scc_extension_flag : 1;
 *     std::uint32_t sps_curr_pic_ref_enabled_flag : 1;
 *     std::uint32_t palette_mode_enabled_flag : 1;
 *     std::uint32_t sps_palette_predictor_initializers_present_flag : 1;
 *     std::uint32_t intra_boundary_filtering_disabled_flag : 1;
 *     std::uint32_t reserved : 2;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265SpsFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `sps_temporal_id_nesting_flag` No description.
 * - `separate_colour_plane_flag` No description.
 * - `conformance_window_flag` No description.
 * - `sps_sub_layer_ordering_info_present_flag` No description.
 * - `scaling_list_enabled_flag` No description.
 * - `sps_scaling_list_data_present_flag` No description.
 * - `amp_enabled_flag` No description.
 * - `sample_adaptive_offset_enabled_flag` No description.
 * - `pcm_enabled_flag` No description.
 * - `pcm_loop_filter_disabled_flag` No description.
 * - `long_term_ref_pics_present_flag` No description.
 * - `sps_temporal_mvp_enabled_flag` No description.
 * - `strong_intra_smoothing_enabled_flag` No description.
 * - `vui_parameters_present_flag` No description.
 * - `sps_extension_present_flag` No description.
 * - `sps_range_extension_flag` No description.
 * - `transform_skip_rotation_enabled_flag` No description.
 * - `transform_skip_context_enabled_flag` No description.
 * - `implicit_rdpcm_enabled_flag` No description.
 * - `explicit_rdpcm_enabled_flag` No description.
 * - `extended_precision_processing_flag` No description.
 * - `intra_smoothing_disabled_flag` No description.
 * - `high_precision_offsets_enabled_flag` No description.
 * - `persistent_rice_adaptation_enabled_flag` No description.
 * - `cabac_bypass_alignment_enabled_flag` No description.
 * - `sps_scc_extension_flag` No description.
 * - `sps_curr_pic_ref_enabled_flag` No description.
 * - `palette_mode_enabled_flag` No description.
 * - `sps_palette_predictor_initializers_present_flag` No description.
 * - `intra_boundary_filtering_disabled_flag` No description.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoH265SpsFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265SpsFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265SequenceParameterSet
 * \endcond
 */
