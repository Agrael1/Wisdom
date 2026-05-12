/**
 * @struct WisStdVideoAV1ChromaSamplePosition WisStdVideoAV1ChromaSamplePosition
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1ChromaSamplePosition_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1ChromaSamplePosition {
 *     WisStdVideoAV1ChromaSamplePositionUnknown = 0,
 *     WisStdVideoAV1ChromaSamplePositionVertical = 1,
 *     WisStdVideoAV1ChromaSamplePositionColocated = 2,
 *     WisStdVideoAV1ChromaSamplePositionReserved = 3,
 *     WisStdVideoAV1ChromaSamplePositionInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1ChromaSamplePosition;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1ChromaSamplePosition {
 *     Unknown = 0,
 *     Vertical = 1,
 *     Colocated = 2,
 *     Reserved = 3,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1ChromaSamplePosition_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * AV1 chroma sample position (AV1 Bitstream Specification Section 6.4.2).
 *
 * Values:
 * - `WisStdVideoAV1ChromaSamplePositionUnknown = 0`: Unknown chroma sample position.
 * - `WisStdVideoAV1ChromaSamplePositionVertical = 1`: Horizontally co-located with luma, vertically shifted by 0.5.
 * - `WisStdVideoAV1ChromaSamplePositionColocated = 2`: Co-located with luma.
 * - `WisStdVideoAV1ChromaSamplePositionReserved = 3`:
 * - `WisStdVideoAV1ChromaSamplePositionInvalid = 0x7FFFFFFF`:
 * \endcond
 *
 *
 * @section WisStdVideoAV1ChromaSamplePosition_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1ColorConfig
 * \endcond
 */
