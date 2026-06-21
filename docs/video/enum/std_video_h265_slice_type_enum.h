/**
 * @struct WisStdVideoH265SliceType WisStdVideoH265SliceType
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoH265SliceType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoH265SliceType {
 *     WisStdVideoH265SliceTypeB = 0,
 *     WisStdVideoH265SliceTypeP = 1,
 *     WisStdVideoH265SliceTypeI = 2,
 *     WisStdVideoH265SliceTypeInvalid = 0x7FFFFFFF,
 * } WisStdVideoH265SliceType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoH265SliceType {
 *     B = 0,
 *     P = 1,
 *     I = 2,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265SliceType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * H.265 slice types as defined in HEVC spec Table 7-7.
 *
 * Values:
 * - `WisStdVideoH265SliceTypeB = 0`: B slice (bi-predictive).
 * - `WisStdVideoH265SliceTypeP = 1`: P slice (predictive).
 * - `WisStdVideoH265SliceTypeI = 2`: I slice (intra).
 * - `WisStdVideoH265SliceTypeInvalid = 0x7FFFFFFF`: Invalid slice type.
 * \endcond
 *
 *
 * @section WisStdVideoH265SliceType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * \endcond
 */
