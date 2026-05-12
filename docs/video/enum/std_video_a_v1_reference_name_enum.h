/**
 * @struct WisStdVideoAV1ReferenceName WisStdVideoAV1ReferenceName
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1ReferenceName_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1ReferenceName {
 *     WisStdVideoAV1ReferenceNameIntraFrame = 0,
 *     WisStdVideoAV1ReferenceNameLastFrame = 1,
 *     WisStdVideoAV1ReferenceNameLast2Frame = 2,
 *     WisStdVideoAV1ReferenceNameLast3Frame = 3,
 *     WisStdVideoAV1ReferenceNameGoldenFrame = 4,
 *     WisStdVideoAV1ReferenceNameBwdrefFrame = 5,
 *     WisStdVideoAV1ReferenceNameAltref2Frame = 6,
 *     WisStdVideoAV1ReferenceNameAltrefFrame = 7,
 *     WisStdVideoAV1ReferenceNameInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1ReferenceName;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1ReferenceName {
 *     IntraFrame = 0,
 *     LastFrame = 1,
 *     Last2Frame = 2,
 *     Last3Frame = 3,
 *     GoldenFrame = 4,
 *     BwdrefFrame = 5,
 *     Altref2Frame = 6,
 *     AltrefFrame = 7,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1ReferenceName_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Names of the reference frames used in AV1 (AV1 Bitstream Specification Section 6.1).
 *
 * Values:
 * - `WisStdVideoAV1ReferenceNameIntraFrame = 0`: Intra frame reference.
 * - `WisStdVideoAV1ReferenceNameLastFrame = 1`: LAST_FRAME (1).
 * - `WisStdVideoAV1ReferenceNameLast2Frame = 2`: LAST2_FRAME (2).
 * - `WisStdVideoAV1ReferenceNameLast3Frame = 3`: LAST3_FRAME (3).
 * - `WisStdVideoAV1ReferenceNameGoldenFrame = 4`: GOLDEN_FRAME (4).
 * - `WisStdVideoAV1ReferenceNameBwdrefFrame = 5`: BWDREF_FRAME (5).
 * - `WisStdVideoAV1ReferenceNameAltref2Frame = 6`: ALTREF2_FRAME (6).
 * - `WisStdVideoAV1ReferenceNameAltrefFrame = 7`: ALTREF_FRAME (7).
 * - `WisStdVideoAV1ReferenceNameInvalid = 0x7FFFFFFF`: Invalid reference name.
 * \endcond
 *
 *
 * @section WisStdVideoAV1ReferenceName_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * \endcond
 */
