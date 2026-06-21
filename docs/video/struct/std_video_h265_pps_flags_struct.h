/**
 * @struct WisStdVideoH265PpsFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265PpsFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265PpsFlags {
 *     uint32_t dependent_slice_segments_enabled_flag : 1;
 *     uint32_t output_flag_present_flag : 1;
 *     uint32_t sign_data_hiding_enabled_flag : 1;
 *     uint32_t cabac_init_present_flag : 1;
 *     uint32_t constrained_intra_pred_flag : 1;
 *     uint32_t transform_skip_enabled_flag : 1;
 *     uint32_t cu_qp_delta_enabled_flag : 1;
 *     uint32_t pps_slice_chroma_qp_offsets_present_flag : 1;
 *     uint32_t weighted_pred_flag : 1;
 *     uint32_t weighted_bipred_flag : 1;
 *     uint32_t transquant_bypass_enabled_flag : 1;
 *     uint32_t tiles_enabled_flag : 1;
 *     uint32_t entropy_coding_sync_enabled_flag : 1;
 *     uint32_t uniform_spacing_flag : 1;
 *     uint32_t loop_filter_across_tiles_enabled_flag : 1;
 *     uint32_t pps_loop_filter_across_slices_enabled_flag : 1;
 *     uint32_t deblocking_filter_control_present_flag : 1;
 *     uint32_t deblocking_filter_override_enabled_flag : 1;
 *     uint32_t pps_deblocking_filter_disabled_flag : 1;
 *     uint32_t pps_scaling_list_data_present_flag : 1;
 *     uint32_t lists_modification_present_flag : 1;
 *     uint32_t slice_segment_header_extension_present_flag : 1;
 *     uint32_t pps_extension_present_flag : 1;
 *     uint32_t cross_component_prediction_enabled_flag : 1;
 *     uint32_t chroma_qp_offset_list_enabled_flag : 1;
 *     uint32_t pps_curr_pic_ref_enabled_flag : 1;
 *     uint32_t residual_adaptive_colour_transform_enabled_flag : 1;
 *     uint32_t pps_slice_act_qp_offsets_present_flag : 1;
 *     uint32_t pps_palette_predictor_initializers_present_flag : 1;
 *     uint32_t monochrome_palette_flag : 1;
 *     uint32_t pps_range_extension_flag : 1;
 *     uint32_t reserved : 1;
 * } WisStdVideoH265PpsFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265PpsFlags {
 *     std::uint32_t dependent_slice_segments_enabled_flag : 1;
 *     std::uint32_t output_flag_present_flag : 1;
 *     std::uint32_t sign_data_hiding_enabled_flag : 1;
 *     std::uint32_t cabac_init_present_flag : 1;
 *     std::uint32_t constrained_intra_pred_flag : 1;
 *     std::uint32_t transform_skip_enabled_flag : 1;
 *     std::uint32_t cu_qp_delta_enabled_flag : 1;
 *     std::uint32_t pps_slice_chroma_qp_offsets_present_flag : 1;
 *     std::uint32_t weighted_pred_flag : 1;
 *     std::uint32_t weighted_bipred_flag : 1;
 *     std::uint32_t transquant_bypass_enabled_flag : 1;
 *     std::uint32_t tiles_enabled_flag : 1;
 *     std::uint32_t entropy_coding_sync_enabled_flag : 1;
 *     std::uint32_t uniform_spacing_flag : 1;
 *     std::uint32_t loop_filter_across_tiles_enabled_flag : 1;
 *     std::uint32_t pps_loop_filter_across_slices_enabled_flag : 1;
 *     std::uint32_t deblocking_filter_control_present_flag : 1;
 *     std::uint32_t deblocking_filter_override_enabled_flag : 1;
 *     std::uint32_t pps_deblocking_filter_disabled_flag : 1;
 *     std::uint32_t pps_scaling_list_data_present_flag : 1;
 *     std::uint32_t lists_modification_present_flag : 1;
 *     std::uint32_t slice_segment_header_extension_present_flag : 1;
 *     std::uint32_t pps_extension_present_flag : 1;
 *     std::uint32_t cross_component_prediction_enabled_flag : 1;
 *     std::uint32_t chroma_qp_offset_list_enabled_flag : 1;
 *     std::uint32_t pps_curr_pic_ref_enabled_flag : 1;
 *     std::uint32_t residual_adaptive_colour_transform_enabled_flag : 1;
 *     std::uint32_t pps_slice_act_qp_offsets_present_flag : 1;
 *     std::uint32_t pps_palette_predictor_initializers_present_flag : 1;
 *     std::uint32_t monochrome_palette_flag : 1;
 *     std::uint32_t pps_range_extension_flag : 1;
 *     std::uint32_t reserved : 1;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265PpsFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `dependent_slice_segments_enabled_flag` No description.
 * - `output_flag_present_flag` No description.
 * - `sign_data_hiding_enabled_flag` No description.
 * - `cabac_init_present_flag` No description.
 * - `constrained_intra_pred_flag` No description.
 * - `transform_skip_enabled_flag` No description.
 * - `cu_qp_delta_enabled_flag` No description.
 * - `pps_slice_chroma_qp_offsets_present_flag` No description.
 * - `weighted_pred_flag` No description.
 * - `weighted_bipred_flag` No description.
 * - `transquant_bypass_enabled_flag` No description.
 * - `tiles_enabled_flag` No description.
 * - `entropy_coding_sync_enabled_flag` No description.
 * - `uniform_spacing_flag` No description.
 * - `loop_filter_across_tiles_enabled_flag` No description.
 * - `pps_loop_filter_across_slices_enabled_flag` No description.
 * - `deblocking_filter_control_present_flag` No description.
 * - `deblocking_filter_override_enabled_flag` No description.
 * - `pps_deblocking_filter_disabled_flag` No description.
 * - `pps_scaling_list_data_present_flag` No description.
 * - `lists_modification_present_flag` No description.
 * - `slice_segment_header_extension_present_flag` No description.
 * - `pps_extension_present_flag` No description.
 * - `cross_component_prediction_enabled_flag` No description.
 * - `chroma_qp_offset_list_enabled_flag` No description.
 * - `pps_curr_pic_ref_enabled_flag` No description.
 * - `residual_adaptive_colour_transform_enabled_flag` No description.
 * - `pps_slice_act_qp_offsets_present_flag` No description.
 * - `pps_palette_predictor_initializers_present_flag` No description.
 * - `monochrome_palette_flag` No description.
 * - `pps_range_extension_flag` No description.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoH265PpsFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265PpsFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265PictureParameterSet
 * \endcond
 */
