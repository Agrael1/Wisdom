/**
 * @struct WisStdVideoH265SequenceParameterSet
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265SequenceParameterSet_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265SequenceParameterSet {
 *     WisStdVideoH265SpsFlags                       flags;
 *     WisStdVideoH265ChromaFormatIdc                chroma_format_idc;
 *     uint32_t                                      pic_width_in_luma_samples;
 *     uint32_t                                      pic_height_in_luma_samples;
 *     uint8_t                                       sps_video_parameter_set_id;
 *     uint8_t                                       sps_max_sub_layers_minus1;
 *     uint8_t                                       sps_seq_parameter_set_id;
 *     uint8_t                                       bit_depth_luma_minus8;
 *     uint8_t                                       bit_depth_chroma_minus8;
 *     uint8_t                                       log2_max_pic_order_cnt_lsb_minus4;
 *     uint8_t                                       log2_min_luma_coding_block_size_minus3;
 *     uint8_t                                       log2_diff_max_min_luma_coding_block_size;
 *     uint8_t                                       log2_min_luma_transform_block_size_minus2;
 *     uint8_t                                       log2_diff_max_min_luma_transform_block_size;
 *     uint8_t                                       max_transform_hierarchy_depth_inter;
 *     uint8_t                                       max_transform_hierarchy_depth_intra;
 *     uint8_t                                       num_short_term_ref_pic_sets;
 *     uint8_t                                       num_long_term_ref_pics_sps;
 *     uint8_t                                       pcm_sample_bit_depth_luma_minus1;
 *     uint8_t                                       pcm_sample_bit_depth_chroma_minus1;
 *     uint8_t                                       log2_min_pcm_luma_coding_block_size_minus3;
 *     uint8_t                                       log2_diff_max_min_pcm_luma_coding_block_size;
 *     uint8_t                                       reserved1;
 *     uint8_t                                       reserved2;
 *     uint8_t                                       palette_max_size;
 *     uint8_t                                       delta_palette_max_predictor_size;
 *     uint8_t                                       motion_vector_resolution_control_idc;
 *     uint8_t                                       sps_num_palette_predictor_initializers_minus1;
 *     uint32_t                                      conf_win_left_offset;
 *     uint32_t                                      conf_win_right_offset;
 *     uint32_t                                      conf_win_top_offset;
 *     uint32_t                                      conf_win_bottom_offset;
 *     const WisStdVideoH265ProfileTierLevel*        pProfileTierLevel;
 *     const WisStdVideoH265DecPicBufMgr*            pDecPicBufMgr;
 *     const WisStdVideoH265ScalingLists*            pScalingLists;
 *     const WisStdVideoH265ShortTermRefPicSet*      pShortTermRefPicSet;
 *     const WisStdVideoH265LongTermRefPicsSps*      pLongTermRefPicsSps;
 *     const WisStdVideoH265SequenceParameterSetVui* pSequenceParameterSetVui;
 *     const WisStdVideoH265PredictorPaletteEntries* pPredictorPaletteEntries;
 * } WisStdVideoH265SequenceParameterSet;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265SequenceParameterSet {
 *     wis::StdVideoH265SpsFlags                       flags;
 *     wis::StdVideoH265ChromaFormatIdc                chroma_format_idc;
 *     std::uint32_t                                   pic_width_in_luma_samples;
 *     std::uint32_t                                   pic_height_in_luma_samples;
 *     std::uint8_t                                    sps_video_parameter_set_id;
 *     std::uint8_t                                    sps_max_sub_layers_minus1;
 *     std::uint8_t                                    sps_seq_parameter_set_id;
 *     std::uint8_t                                    bit_depth_luma_minus8;
 *     std::uint8_t                                    bit_depth_chroma_minus8;
 *     std::uint8_t                                    log2_max_pic_order_cnt_lsb_minus4;
 *     std::uint8_t                                    log2_min_luma_coding_block_size_minus3;
 *     std::uint8_t                                    log2_diff_max_min_luma_coding_block_size;
 *     std::uint8_t                                    log2_min_luma_transform_block_size_minus2;
 *     std::uint8_t                                    log2_diff_max_min_luma_transform_block_size;
 *     std::uint8_t                                    max_transform_hierarchy_depth_inter;
 *     std::uint8_t                                    max_transform_hierarchy_depth_intra;
 *     std::uint8_t                                    num_short_term_ref_pic_sets;
 *     std::uint8_t                                    num_long_term_ref_pics_sps;
 *     std::uint8_t                                    pcm_sample_bit_depth_luma_minus1;
 *     std::uint8_t                                    pcm_sample_bit_depth_chroma_minus1;
 *     std::uint8_t                                    log2_min_pcm_luma_coding_block_size_minus3;
 *     std::uint8_t                                    log2_diff_max_min_pcm_luma_coding_block_size;
 *     std::uint8_t                                    reserved1;
 *     std::uint8_t                                    reserved2;
 *     std::uint8_t                                    palette_max_size;
 *     std::uint8_t                                    delta_palette_max_predictor_size;
 *     std::uint8_t                                    motion_vector_resolution_control_idc;
 *     std::uint8_t                                    sps_num_palette_predictor_initializers_minus1;
 *     std::uint32_t                                   conf_win_left_offset;
 *     std::uint32_t                                   conf_win_right_offset;
 *     std::uint32_t                                   conf_win_top_offset;
 *     std::uint32_t                                   conf_win_bottom_offset;
 *     const wis::StdVideoH265ProfileTierLevel*        pProfileTierLevel;
 *     const wis::StdVideoH265DecPicBufMgr*            pDecPicBufMgr;
 *     const wis::StdVideoH265ScalingLists*            pScalingLists;
 *     const wis::StdVideoH265ShortTermRefPicSet*      pShortTermRefPicSet;
 *     const wis::StdVideoH265LongTermRefPicsSps*      pLongTermRefPicsSps;
 *     const wis::StdVideoH265SequenceParameterSetVui* pSequenceParameterSetVui;
 *     const wis::StdVideoH265PredictorPaletteEntries* pPredictorPaletteEntries;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265SequenceParameterSet_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` SPS flags.
 * - `chroma_format_idc` Chroma format IDC.
 * - `pic_width_in_luma_samples` Picture width in luma samples.
 * - `pic_height_in_luma_samples` Picture height in luma samples.
 * - `sps_video_parameter_set_id` SPS video parameter set ID.
 * - `sps_max_sub_layers_minus1` SPS max sub-layers minus 1.
 * - `sps_seq_parameter_set_id` SPS sequence parameter set ID.
 * - `bit_depth_luma_minus8` Bit depth luma minus 8.
 * - `bit_depth_chroma_minus8` Bit depth chroma minus 8.
 * - `log2_max_pic_order_cnt_lsb_minus4` Log2 max POC LSB minus 4.
 * - `log2_min_luma_coding_block_size_minus3` Log2 min luma coding block size minus 3.
 * - `log2_diff_max_min_luma_coding_block_size` Log2 diff max min luma coding block size.
 * - `log2_min_luma_transform_block_size_minus2` Log2 min luma transform block size minus 2.
 * - `log2_diff_max_min_luma_transform_block_size` Log2 diff max min luma transform block size.
 * - `max_transform_hierarchy_depth_inter` Max transform hierarchy depth inter.
 * - `max_transform_hierarchy_depth_intra` Max transform hierarchy depth intra.
 * - `num_short_term_ref_pic_sets` Number of short-term reference picture sets.
 * - `num_long_term_ref_pics_sps` Number of long-term reference pictures in SPS.
 * - `pcm_sample_bit_depth_luma_minus1` PCM sample bit depth luma minus 1.
 * - `pcm_sample_bit_depth_chroma_minus1` PCM sample bit depth chroma minus 1.
 * - `log2_min_pcm_luma_coding_block_size_minus3` Log2 min PCM luma coding block size minus 3.
 * - `log2_diff_max_min_pcm_luma_coding_block_size` Log2 diff max min PCM luma coding block size.
 * - `reserved1` No description.
 * - `reserved2` No description.
 * - `palette_max_size` Palette max size for SCC.
 * - `delta_palette_max_predictor_size` Delta palette max predictor size.
 * - `motion_vector_resolution_control_idc` Motion vector resolution control IDC.
 * - `sps_num_palette_predictor_initializers_minus1` Number of palette predictor initializers minus 1.
 * - `conf_win_left_offset` Conformance window left offset.
 * - `conf_win_right_offset` Conformance window right offset.
 * - `conf_win_top_offset` Conformance window top offset.
 * - `conf_win_bottom_offset` Conformance window bottom offset.
 * - `pProfileTierLevel` Pointer to profile tier level.
 * - `pDecPicBufMgr` Pointer to decoded picture buffer management info.
 * - `pScalingLists` Pointer to scaling lists.
 * - `pShortTermRefPicSet` Pointer to short-term reference picture sets.
 * - `pLongTermRefPicsSps` Pointer to long-term reference pictures in SPS.
 * - `pSequenceParameterSetVui` Pointer to sequence parameter set VUI.
 * - `pPredictorPaletteEntries` Pointer to predictor palette entries.
 * \endcond
 *
 * @section WisStdVideoH265SequenceParameterSet_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265SequenceParameterSet_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
