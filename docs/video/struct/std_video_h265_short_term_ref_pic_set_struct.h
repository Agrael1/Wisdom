/**
 * @struct WisStdVideoH265ShortTermRefPicSet
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265ShortTermRefPicSet_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265ShortTermRefPicSet {
 *     WisStdVideoH265ShortTermRefPicSetFlags flags;
 *     uint32_t                               delta_idx_minus1;
 *     uint16_t                               use_delta_flag;
 *     uint16_t                               abs_delta_rps_minus1;
 *     uint16_t                               used_by_curr_pic_flag;
 *     uint16_t                               used_by_curr_pic_s0_flag;
 *     uint16_t                               used_by_curr_pic_s1_flag;
 *     uint16_t                               reserved1;
 *     uint8_t                                reserved2;
 *     uint8_t                                reserved3;
 *     uint8_t                                num_negative_pics;
 *     uint8_t                                num_positive_pics;
 *     uint16_t                               delta_poc_s0_minus1[16];
 *     uint16_t                               delta_poc_s1_minus1[16];
 * } WisStdVideoH265ShortTermRefPicSet;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265ShortTermRefPicSet {
 *     wis::StdVideoH265ShortTermRefPicSetFlags flags;
 *     std::uint32_t                            delta_idx_minus1;
 *     std::uint16_t                            use_delta_flag;
 *     std::uint16_t                            abs_delta_rps_minus1;
 *     std::uint16_t                            used_by_curr_pic_flag;
 *     std::uint16_t                            used_by_curr_pic_s0_flag;
 *     std::uint16_t                            used_by_curr_pic_s1_flag;
 *     std::uint16_t                            reserved1;
 *     std::uint8_t                             reserved2;
 *     std::uint8_t                             reserved3;
 *     std::uint8_t                             num_negative_pics;
 *     std::uint8_t                             num_positive_pics;
 *     std::array<std::uint16_t, 16>            delta_poc_s0_minus1;
 *     std::array<std::uint16_t, 16>            delta_poc_s1_minus1;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265ShortTermRefPicSet_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Short-term ref pic set flags.
 * - `delta_idx_minus1` Delta index minus 1.
 * - `use_delta_flag` Use delta flag.
 * - `abs_delta_rps_minus1` Absolute delta RPS minus 1.
 * - `used_by_curr_pic_flag` Used by current picture flag.
 * - `used_by_curr_pic_s0_flag` Used by current picture S0 flag.
 * - `used_by_curr_pic_s1_flag` Used by current picture S1 flag.
 * - `reserved1` No description.
 * - `reserved2` No description.
 * - `reserved3` No description.
 * - `num_negative_pics` Number of negative pictures.
 * - `num_positive_pics` Number of positive pictures.
 * - `delta_poc_s0_minus1` Delta POC S0 minus 1.
 * - `delta_poc_s1_minus1` Delta POC S1 minus 1.
 * \endcond
 *
 * @section WisStdVideoH265ShortTermRefPicSet_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265ShortTermRefPicSet_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265SequenceParameterSet
 * \endcond
 */
