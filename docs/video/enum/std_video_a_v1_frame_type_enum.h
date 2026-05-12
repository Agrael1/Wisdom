/**
 * @struct WisStdVideoAV1FrameType WisStdVideoAV1FrameType
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1FrameType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1FrameType {
 *     WisStdVideoAV1FrameTypeKey = 0,
 *     WisStdVideoAV1FrameTypeInter = 1,
 *     WisStdVideoAV1FrameTypeIntraOnly = 2,
 *     WisStdVideoAV1FrameTypeSwitch = 3,
 *     WisStdVideoAV1FrameTypeInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1FrameType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1FrameType {
 *     Key = 0,
 *     Inter = 1,
 *     IntraOnly = 2,
 *     Switch = 3,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1FrameType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Specifies the AV1 frame type (AV1 Bitstream Specification Section 6.8.2).
 *
 * Values:
 * - `WisStdVideoAV1FrameTypeKey = 0`: A key frame contains only intra-coded blocks and is fully decipherable.
 * - `WisStdVideoAV1FrameTypeInter = 1`: An inter frame @wis_may contain intra-coded blocks and inter-coded blocks.
 * - `WisStdVideoAV1FrameTypeIntraOnly = 2`: An intra-only frame contains only intra-coded blocks but acts otherwise as
 * an inter frame.
 * - `WisStdVideoAV1FrameTypeSwitch = 3`: A switch frame is an inter frame that can be used as a switching point for
 * adaptive streaming.
 * - `WisStdVideoAV1FrameTypeInvalid = 0x7FFFFFFF`: Invalid frame type.
 * \endcond
 *
 *
 * @section WisStdVideoAV1FrameType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
