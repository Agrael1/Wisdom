/**
 * @struct WisStdVideoH265ShortTermRefPicSetFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265ShortTermRefPicSetFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265ShortTermRefPicSetFlags {
 *     uint32_t inter_ref_pic_set_prediction_flag : 1;
 *     uint32_t delta_rps_sign : 1;
 *     uint32_t reserved : 30;
 * } WisStdVideoH265ShortTermRefPicSetFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265ShortTermRefPicSetFlags {
 *     std::uint32_t inter_ref_pic_set_prediction_flag : 1;
 *     std::uint32_t delta_rps_sign : 1;
 *     std::uint32_t reserved : 30;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265ShortTermRefPicSetFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `inter_ref_pic_set_prediction_flag` Specifies that inter ref pic set prediction is used.
 * - `delta_rps_sign` Sign of the delta RPS.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoH265ShortTermRefPicSetFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265ShortTermRefPicSetFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265ShortTermRefPicSet
 * \endcond
 */
