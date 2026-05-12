/**
 * @struct WisStdVideoAV1FrameRestorationType WisStdVideoAV1FrameRestorationType
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1FrameRestorationType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1FrameRestorationType {
 *     WisStdVideoAV1FrameRestorationTypeNone = 0,
 *     WisStdVideoAV1FrameRestorationTypeWiener = 1,
 *     WisStdVideoAV1FrameRestorationTypeSgrproj = 2,
 *     WisStdVideoAV1FrameRestorationTypeSwitchable = 3,
 *     WisStdVideoAV1FrameRestorationTypeInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1FrameRestorationType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1FrameRestorationType {
 *     None = 0,
 *     Wiener = 1,
 *     Sgrproj = 2,
 *     Switchable = 3,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1FrameRestorationType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Loop restoration types (AV1 Bitstream Specification Section 6.10.15).
 *
 * Values:
 * - `WisStdVideoAV1FrameRestorationTypeNone = 0`: No loop restoration.
 * - `WisStdVideoAV1FrameRestorationTypeWiener = 1`: Wiener filter loop restoration.
 * - `WisStdVideoAV1FrameRestorationTypeSgrproj = 2`: Self-guided filter loop restoration.
 * - `WisStdVideoAV1FrameRestorationTypeSwitchable = 3`: Switchable between Wiener and Sgrproj.
 * - `WisStdVideoAV1FrameRestorationTypeInvalid = 0x7FFFFFFF`: Invalid restoration type.
 * \endcond
 *
 *
 * @section WisStdVideoAV1FrameRestorationType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1LoopRestoration
 * \endcond
 */
