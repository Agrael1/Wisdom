/**
 * @struct WisStdVideoAV1ColorPrimaries WisStdVideoAV1ColorPrimaries
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1ColorPrimaries_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1ColorPrimaries {
 *     WisStdVideoAV1ColorPrimariesBt709 = 1,
 *     WisStdVideoAV1ColorPrimariesUnspecified = 2,
 *     WisStdVideoAV1ColorPrimariesBt470M = 4,
 *     WisStdVideoAV1ColorPrimariesBt470BG = 5,
 *     WisStdVideoAV1ColorPrimariesBt601 = 6,
 *     WisStdVideoAV1ColorPrimariesSmpte240 = 7,
 *     WisStdVideoAV1ColorPrimariesGenericFilm = 8,
 *     WisStdVideoAV1ColorPrimariesBt2020 = 9,
 *     WisStdVideoAV1ColorPrimariesXyz = 10,
 *     WisStdVideoAV1ColorPrimariesSmpte431 = 11,
 *     WisStdVideoAV1ColorPrimariesSmpte432 = 12,
 *     WisStdVideoAV1ColorPrimariesEbu3213 = 22,
 *     WisStdVideoAV1ColorPrimariesInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1ColorPrimaries;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1ColorPrimaries {
 *     Bt709 = 1,
 *     Unspecified = 2,
 *     Bt470M = 4,
 *     Bt470BG = 5,
 *     Bt601 = 6,
 *     Smpte240 = 7,
 *     GenericFilm = 8,
 *     Bt2020 = 9,
 *     Xyz = 10,
 *     Smpte431 = 11,
 *     Smpte432 = 12,
 *     Ebu3213 = 22,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1ColorPrimaries_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * AV1 color primaries mapping to ISO/IEC 23000-2 / CICP.
 *
 * Values:
 * - `WisStdVideoAV1ColorPrimariesBt709 = 1`: Rec. ITU-R BT.709-6.
 * - `WisStdVideoAV1ColorPrimariesUnspecified = 2`: Image characteristics are unknown or unspecified.
 * - `WisStdVideoAV1ColorPrimariesBt470M = 4`: Rec. ITU-R BT.470-6 System M (historical).
 * - `WisStdVideoAV1ColorPrimariesBt470BG = 5`: Rec. ITU-R BT.470-6 System B, G (historical).
 * - `WisStdVideoAV1ColorPrimariesBt601 = 6`: Rec. ITU-R BT.601-7 525.
 * - `WisStdVideoAV1ColorPrimariesSmpte240 = 7`: SMPTE 240M.
 * - `WisStdVideoAV1ColorPrimariesGenericFilm = 8`: Generic film (color filters using Illuminant C).
 * - `WisStdVideoAV1ColorPrimariesBt2020 = 9`: Rec. ITU-R BT.2020-2.
 * - `WisStdVideoAV1ColorPrimariesXyz = 10`: SMPTE ST 428-1.
 * - `WisStdVideoAV1ColorPrimariesSmpte431 = 11`: SMPTE RP 431-2.
 * - `WisStdVideoAV1ColorPrimariesSmpte432 = 12`: SMPTE EG 432-1.
 * - `WisStdVideoAV1ColorPrimariesEbu3213 = 22`: EBU Tech. 3213-E.
 * - `WisStdVideoAV1ColorPrimariesInvalid = 0x7FFFFFFF`: Invalid.
 * \endcond
 *
 *
 * @section WisStdVideoAV1ColorPrimaries_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1ColorConfig
 * \endcond
 */
