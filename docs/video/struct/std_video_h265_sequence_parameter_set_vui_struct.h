/**
 * @struct WisStdVideoH265SequenceParameterSetVui
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265SequenceParameterSetVui_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265SequenceParameterSetVui {
 *     WisStdVideoH265SpsVuiFlags          flags;
 *     WisStdVideoH265AspectRatioIdc       aspect_ratio_idc;
 *     uint16_t                            sar_width;
 *     uint16_t                            sar_height;
 *     uint8_t                             video_format;
 *     uint8_t                             colour_primaries;
 *     uint8_t                             transfer_characteristics;
 *     uint8_t                             matrix_coeffs;
 *     uint8_t                             chroma_sample_loc_type_top_field;
 *     uint8_t                             chroma_sample_loc_type_bottom_field;
 *     uint8_t                             reserved1;
 *     uint8_t                             reserved2;
 *     uint16_t                            def_disp_win_left_offset;
 *     uint16_t                            def_disp_win_right_offset;
 *     uint16_t                            def_disp_win_top_offset;
 *     uint16_t                            def_disp_win_bottom_offset;
 *     uint32_t                            vui_num_units_in_tick;
 *     uint32_t                            vui_time_scale;
 *     uint32_t                            vui_num_ticks_poc_diff_one_minus1;
 *     uint16_t                            min_spatial_segmentation_idc;
 *     uint16_t                            reserved3;
 *     uint8_t                             max_bytes_per_pic_denom;
 *     uint8_t                             max_bits_per_min_cu_denom;
 *     uint8_t                             log2_max_mv_length_horizontal;
 *     uint8_t                             log2_max_mv_length_vertical;
 *     const WisStdVideoH265HrdParameters* pHrdParameters;
 * } WisStdVideoH265SequenceParameterSetVui;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265SequenceParameterSetVui {
 *     wis::StdVideoH265SpsVuiFlags          flags;
 *     wis::StdVideoH265AspectRatioIdc       aspect_ratio_idc;
 *     std::uint16_t                         sar_width;
 *     std::uint16_t                         sar_height;
 *     std::uint8_t                          video_format;
 *     std::uint8_t                          colour_primaries;
 *     std::uint8_t                          transfer_characteristics;
 *     std::uint8_t                          matrix_coeffs;
 *     std::uint8_t                          chroma_sample_loc_type_top_field;
 *     std::uint8_t                          chroma_sample_loc_type_bottom_field;
 *     std::uint8_t                          reserved1;
 *     std::uint8_t                          reserved2;
 *     std::uint16_t                         def_disp_win_left_offset;
 *     std::uint16_t                         def_disp_win_right_offset;
 *     std::uint16_t                         def_disp_win_top_offset;
 *     std::uint16_t                         def_disp_win_bottom_offset;
 *     std::uint32_t                         vui_num_units_in_tick;
 *     std::uint32_t                         vui_time_scale;
 *     std::uint32_t                         vui_num_ticks_poc_diff_one_minus1;
 *     std::uint16_t                         min_spatial_segmentation_idc;
 *     std::uint16_t                         reserved3;
 *     std::uint8_t                          max_bytes_per_pic_denom;
 *     std::uint8_t                          max_bits_per_min_cu_denom;
 *     std::uint8_t                          log2_max_mv_length_horizontal;
 *     std::uint8_t                          log2_max_mv_length_vertical;
 *     const wis::StdVideoH265HrdParameters* pHrdParameters;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265SequenceParameterSetVui_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` SPS VUI flags.
 * - `aspect_ratio_idc` Aspect ratio IDC.
 * - `sar_width` SAR width.
 * - `sar_height` SAR height.
 * - `video_format` Video format.
 * - `colour_primaries` Colour primaries.
 * - `transfer_characteristics` Transfer characteristics.
 * - `matrix_coeffs` Matrix coefficients.
 * - `chroma_sample_loc_type_top_field` Chroma sample location type top field.
 * - `chroma_sample_loc_type_bottom_field` Chroma sample location type bottom field.
 * - `reserved1` No description.
 * - `reserved2` No description.
 * - `def_disp_win_left_offset` Default display window left offset.
 * - `def_disp_win_right_offset` Default display window right offset.
 * - `def_disp_win_top_offset` Default display window top offset.
 * - `def_disp_win_bottom_offset` Default display window bottom offset.
 * - `vui_num_units_in_tick` VUI num units in tick.
 * - `vui_time_scale` VUI time scale.
 * - `vui_num_ticks_poc_diff_one_minus1` VUI num ticks POC diff one minus 1.
 * - `min_spatial_segmentation_idc` Minimum spatial segmentation IDC.
 * - `reserved3` No description.
 * - `max_bytes_per_pic_denom` Max bytes per picture denominator.
 * - `max_bits_per_min_cu_denom` Max bits per min CU denominator.
 * - `log2_max_mv_length_horizontal` Log2 max MV length horizontal.
 * - `log2_max_mv_length_vertical` Log2 max MV length vertical.
 * - `pHrdParameters` Pointer to HRD parameters.
 * \endcond
 *
 * @section WisStdVideoH265SequenceParameterSetVui_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265SequenceParameterSetVui_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265SequenceParameterSet
 * \endcond
 */
