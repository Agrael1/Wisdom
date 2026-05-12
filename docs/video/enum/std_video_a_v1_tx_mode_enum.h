/**
 * @struct WisStdVideoAV1TxMode WisStdVideoAV1TxMode
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1TxMode_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1TxMode {
 *     WisStdVideoAV1TxModeOnly4x4 = 0,
 *     WisStdVideoAV1TxModeLargest = 1,
 *     WisStdVideoAV1TxModeSelect = 2,
 *     WisStdVideoAV1TxModeInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1TxMode;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1TxMode {
 *     Only4x4 = 0,
 *     Largest = 1,
 *     Select = 2,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1TxMode_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Transform mode (AV1 Bitstream Specification Section 6.8.21).
 *
 * Values:
 * - `WisStdVideoAV1TxModeOnly4x4 = 0`: Only 4x4 transforms.
 * - `WisStdVideoAV1TxModeLargest = 1`: Largest allowed transform for the partition.
 * - `WisStdVideoAV1TxModeSelect = 2`: Select the transform mode.
 * - `WisStdVideoAV1TxModeInvalid = 0x7FFFFFFF`: Invalid TxMode.
 * \endcond
 *
 *
 * @section WisStdVideoAV1TxMode_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
