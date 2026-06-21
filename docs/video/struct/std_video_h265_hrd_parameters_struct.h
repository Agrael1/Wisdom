/**
 * @struct WisStdVideoH265HrdParameters
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265HrdParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265HrdParameters {
 *     WisStdVideoH265HrdFlags                     flags;
 *     uint8_t                                     tick_divisor_minus2;
 *     uint8_t                                     du_cpb_removal_delay_increment_length_minus1;
 *     uint8_t                                     dpb_output_delay_du_length_minus1;
 *     uint8_t                                     bit_rate_scale;
 *     uint8_t                                     cpb_size_scale;
 *     uint8_t                                     cpb_size_du_scale;
 *     uint8_t                                     initial_cpb_removal_delay_length_minus1;
 *     uint8_t                                     au_cpb_removal_delay_length_minus1;
 *     uint8_t                                     dpb_output_delay_length_minus1;
 *     uint8_t                                     cpb_cnt_minus1[7];
 *     uint16_t                                    elemental_duration_in_tc_minus1[7];
 *     uint16_t                                    reserved[3];
 *     const WisStdVideoH265SubLayerHrdParameters* pSubLayerHrdParametersNal;
 *     const WisStdVideoH265SubLayerHrdParameters* pSubLayerHrdParametersVcl;
 * } WisStdVideoH265HrdParameters;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265HrdParameters {
 *     wis::StdVideoH265HrdFlags                     flags;
 *     std::uint8_t                                  tick_divisor_minus2;
 *     std::uint8_t                                  du_cpb_removal_delay_increment_length_minus1;
 *     std::uint8_t                                  dpb_output_delay_du_length_minus1;
 *     std::uint8_t                                  bit_rate_scale;
 *     std::uint8_t                                  cpb_size_scale;
 *     std::uint8_t                                  cpb_size_du_scale;
 *     std::uint8_t                                  initial_cpb_removal_delay_length_minus1;
 *     std::uint8_t                                  au_cpb_removal_delay_length_minus1;
 *     std::uint8_t                                  dpb_output_delay_length_minus1;
 *     std::array<std::uint8_t, 7>                   cpb_cnt_minus1;
 *     std::array<std::uint16_t, 7>                  elemental_duration_in_tc_minus1;
 *     std::array<std::uint16_t, 3>                  reserved;
 *     const wis::StdVideoH265SubLayerHrdParameters* pSubLayerHrdParametersNal;
 *     const wis::StdVideoH265SubLayerHrdParameters* pSubLayerHrdParametersVcl;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265HrdParameters_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` HRD flags.
 * - `tick_divisor_minus2` Tick divisor minus 2.
 * - `du_cpb_removal_delay_increment_length_minus1` DU CPB removal delay increment length minus 1.
 * - `dpb_output_delay_du_length_minus1` DPB output delay DU length minus 1.
 * - `bit_rate_scale` Bit rate scale.
 * - `cpb_size_scale` CPB size scale.
 * - `cpb_size_du_scale` CPB size du scale.
 * - `initial_cpb_removal_delay_length_minus1` Initial CPB removal delay length minus 1.
 * - `au_cpb_removal_delay_length_minus1` AU CPB removal delay length minus 1.
 * - `dpb_output_delay_length_minus1` DPB output delay length minus 1.
 * - `cpb_cnt_minus1` CPB count minus 1 for each sub-layer.
 * - `elemental_duration_in_tc_minus1` Elemental duration in tc minus 1 for each sub-layer.
 * - `reserved` No description.
 * - `pSubLayerHrdParametersNal` Pointer to NAL sub-layer HRD parameters.
 * - `pSubLayerHrdParametersVcl` Pointer to VCL sub-layer HRD parameters.
 * \endcond
 *
 * @section WisStdVideoH265HrdParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265HrdParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265VideoParameterSet, WisStdVideoH265SequenceParameterSetVui
 * \endcond
 */
