/**
 * @struct WisStdVideoH265LongTermRefPicsSps
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265LongTermRefPicsSps_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265LongTermRefPicsSps {
 *     uint32_t used_by_curr_pic_lt_sps_flag;
 *     uint32_t lt_ref_pic_poc_lsb_sps[32];
 * } WisStdVideoH265LongTermRefPicsSps;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265LongTermRefPicsSps {
 *     std::uint32_t used_by_curr_pic_lt_sps_flag;
 *     std::array<std::uint32_t, 32> lt_ref_pic_poc_lsb_sps;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265LongTermRefPicsSps_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `used_by_curr_pic_lt_sps_flag` Used by current picture LT SPS flag.
 * - `lt_ref_pic_poc_lsb_sps` LT reference picture POC LSB SPS values.
 * \endcond
 *
 * @section WisStdVideoH265LongTermRefPicsSps_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265LongTermRefPicsSps_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265SequenceParameterSet
 * \endcond
 */
