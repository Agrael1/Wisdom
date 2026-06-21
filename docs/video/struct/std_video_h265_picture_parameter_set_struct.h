/**
 * @struct WisStdVideoH265PictureParameterSet
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265PictureParameterSet_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265PictureParameterSet {
 *     WisStdVideoH265PpsFlags                       flags;
 *     uint8_t                                       pps_pic_parameter_set_id;
 *     uint8_t                                       pps_seq_parameter_set_id;
 *     uint8_t                                       sps_video_parameter_set_id;
 *     uint8_t                                       num_extra_slice_header_bits;
 *     uint8_t                                       num_ref_idx_l0_default_active_minus1;
 *     uint8_t                                       num_ref_idx_l1_default_active_minus1;
 *     int8_t                                        init_qp_minus26;
 *     uint8_t                                       diff_cu_qp_delta_depth;
 *     int8_t                                        pps_cb_qp_offset;
 *     int8_t                                        pps_cr_qp_offset;
 *     int8_t                                        pps_beta_offset_div2;
 *     int8_t                                        pps_tc_offset_div2;
 *     uint8_t                                       log2_parallel_merge_level_minus2;
 *     uint8_t                                       log2_max_transform_skip_block_size_minus2;
 *     uint8_t                                       diff_cu_chroma_qp_offset_depth;
 *     uint8_t                                       chroma_qp_offset_list_len_minus1;
 *     int8_t                                        cb_qp_offset_list[6];
 *     int8_t                                        cr_qp_offset_list[6];
 *     uint8_t                                       log2_sao_offset_scale_luma;
 *     uint8_t                                       log2_sao_offset_scale_chroma;
 *     int8_t                                        pps_act_y_qp_offset_plus5;
 *     int8_t                                        pps_act_cb_qp_offset_plus5;
 *     int8_t                                        pps_act_cr_qp_offset_plus3;
 *     uint8_t                                       pps_num_palette_predictor_initializers;
 *     uint8_t                                       luma_bit_depth_entry_minus8;
 *     uint8_t                                       chroma_bit_depth_entry_minus8;
 *     uint8_t                                       num_tile_columns_minus1;
 *     uint8_t                                       num_tile_rows_minus1;
 *     uint8_t                                       reserved1;
 *     uint8_t                                       reserved2;
 *     uint16_t                                      column_width_minus1[19];
 *     uint16_t                                      row_height_minus1[21];
 *     uint32_t                                      reserved3;
 *     const WisStdVideoH265ScalingLists*            pScalingLists;
 *     const WisStdVideoH265PredictorPaletteEntries* pPredictorPaletteEntries;
 * } WisStdVideoH265PictureParameterSet;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265PictureParameterSet {
 *     wis::StdVideoH265PpsFlags                       flags;
 *     std::uint8_t                                    pps_pic_parameter_set_id;
 *     std::uint8_t                                    pps_seq_parameter_set_id;
 *     std::uint8_t                                    sps_video_parameter_set_id;
 *     std::uint8_t                                    num_extra_slice_header_bits;
 *     std::uint8_t                                    num_ref_idx_l0_default_active_minus1;
 *     std::uint8_t                                    num_ref_idx_l1_default_active_minus1;
 *     std::int8_t                                     init_qp_minus26;
 *     std::uint8_t                                    diff_cu_qp_delta_depth;
 *     std::int8_t                                     pps_cb_qp_offset;
 *     std::int8_t                                     pps_cr_qp_offset;
 *     std::int8_t                                     pps_beta_offset_div2;
 *     std::int8_t                                     pps_tc_offset_div2;
 *     std::uint8_t                                    log2_parallel_merge_level_minus2;
 *     std::uint8_t                                    log2_max_transform_skip_block_size_minus2;
 *     std::uint8_t                                    diff_cu_chroma_qp_offset_depth;
 *     std::uint8_t                                    chroma_qp_offset_list_len_minus1;
 *     std::array<std::int8_t, 6>                      cb_qp_offset_list;
 *     std::array<std::int8_t, 6>                      cr_qp_offset_list;
 *     std::uint8_t                                    log2_sao_offset_scale_luma;
 *     std::uint8_t                                    log2_sao_offset_scale_chroma;
 *     std::int8_t                                     pps_act_y_qp_offset_plus5;
 *     std::int8_t                                     pps_act_cb_qp_offset_plus5;
 *     std::int8_t                                     pps_act_cr_qp_offset_plus3;
 *     std::uint8_t                                    pps_num_palette_predictor_initializers;
 *     std::uint8_t                                    luma_bit_depth_entry_minus8;
 *     std::uint8_t                                    chroma_bit_depth_entry_minus8;
 *     std::uint8_t                                    num_tile_columns_minus1;
 *     std::uint8_t                                    num_tile_rows_minus1;
 *     std::uint8_t                                    reserved1;
 *     std::uint8_t                                    reserved2;
 *     std::array<std::uint16_t, 19>                   column_width_minus1;
 *     std::array<std::uint16_t, 21>                   row_height_minus1;
 *     std::uint32_t                                   reserved3;
 *     const wis::StdVideoH265ScalingLists*            pScalingLists;
 *     const wis::StdVideoH265PredictorPaletteEntries* pPredictorPaletteEntries;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265PictureParameterSet_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` PPS flags.
 * - `pps_pic_parameter_set_id` PPS ID.
 * - `pps_seq_parameter_set_id` PPS SPS ID.
 * - `sps_video_parameter_set_id` SPS VPS ID.
 * - `num_extra_slice_header_bits` Number of extra slice header bits.
 * - `num_ref_idx_l0_default_active_minus1` Number of ref idx L0 default active minus 1.
 * - `num_ref_idx_l1_default_active_minus1` Number of ref idx L1 default active minus 1.
 * - `init_qp_minus26` Init QP minus 26.
 * - `diff_cu_qp_delta_depth` Diff CU QP delta depth.
 * - `pps_cb_qp_offset` PPS Cb QP offset.
 * - `pps_cr_qp_offset` PPS Cr QP offset.
 * - `pps_beta_offset_div2` PPS beta offset div 2.
 * - `pps_tc_offset_div2` PPS tc offset div 2.
 * - `log2_parallel_merge_level_minus2` Log2 parallel merge level minus 2.
 * - `log2_max_transform_skip_block_size_minus2` Log2 max transform skip block size minus 2.
 * - `diff_cu_chroma_qp_offset_depth` Diff CU chroma QP offset depth.
 * - `chroma_qp_offset_list_len_minus1` Chroma QP offset list length minus 1.
 * - `cb_qp_offset_list` Cb QP offset list.
 * - `cr_qp_offset_list` Cr QP offset list.
 * - `log2_sao_offset_scale_luma` Log2 SAO offset scale luma.
 * - `log2_sao_offset_scale_chroma` Log2 SAO offset scale chroma.
 * - `pps_act_y_qp_offset_plus5` PPS ACT Y QP offset plus 5.
 * - `pps_act_cb_qp_offset_plus5` PPS ACT Cb QP offset plus 5.
 * - `pps_act_cr_qp_offset_plus3` PPS ACT Cr QP offset plus 3.
 * - `pps_num_palette_predictor_initializers` Number of palette predictor initializers.
 * - `luma_bit_depth_entry_minus8` Luma bit depth entry minus 8.
 * - `chroma_bit_depth_entry_minus8` Chroma bit depth entry minus 8.
 * - `num_tile_columns_minus1` Number of tile columns minus 1.
 * - `num_tile_rows_minus1` Number of tile rows minus 1.
 * - `reserved1` No description.
 * - `reserved2` No description.
 * - `column_width_minus1` Column width minus 1 for each tile column.
 * - `row_height_minus1` Row height minus 1 for each tile row.
 * - `reserved3` No description.
 * - `pScalingLists` Pointer to scaling lists.
 * - `pPredictorPaletteEntries` Pointer to predictor palette entries.
 * \endcond
 *
 * @section WisStdVideoH265PictureParameterSet_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265PictureParameterSet_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
