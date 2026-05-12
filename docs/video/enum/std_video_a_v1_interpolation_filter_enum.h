/**
 * @struct WisStdVideoAV1InterpolationFilter WisStdVideoAV1InterpolationFilter
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1InterpolationFilter_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1InterpolationFilter {
 *     WisStdVideoAV1InterpolationFilterEighttap = 0,
 *     WisStdVideoAV1InterpolationFilterEighttapSmooth = 1,
 *     WisStdVideoAV1InterpolationFilterEighttapSharp = 2,
 *     WisStdVideoAV1InterpolationFilterBilinear = 3,
 *     WisStdVideoAV1InterpolationFilterSwitchable = 4,
 *     WisStdVideoAV1InterpolationFilterInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1InterpolationFilter;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1InterpolationFilter {
 *     Eighttap = 0,
 *     EighttapSmooth = 1,
 *     EighttapSharp = 2,
 *     Bilinear = 3,
 *     Switchable = 4,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1InterpolationFilter_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Interpolation filter types (AV1 Bitstream Specification Section 6.8.9).
 *
 * Values:
 * - `WisStdVideoAV1InterpolationFilterEighttap = 0`: Eight-tap filter.
 * - `WisStdVideoAV1InterpolationFilterEighttapSmooth = 1`: Eight-tap smooth filter.
 * - `WisStdVideoAV1InterpolationFilterEighttapSharp = 2`: Eight-tap sharp filter.
 * - `WisStdVideoAV1InterpolationFilterBilinear = 3`: Bilinear filter.
 * - `WisStdVideoAV1InterpolationFilterSwitchable = 4`: Switchable interpolation filter at the block level.
 * - `WisStdVideoAV1InterpolationFilterInvalid = 0x7FFFFFFF`: Invalid filter.
 * \endcond
 *
 *
 * @section WisStdVideoAV1InterpolationFilter_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
