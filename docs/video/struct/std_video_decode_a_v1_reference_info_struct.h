/**
 * @struct WisStdVideoDecodeAV1ReferenceInfo
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoDecodeAV1ReferenceInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoDecodeAV1ReferenceInfo {
 *     WisStdVideoDecodeAV1ReferenceInfoFlags flags;
 *     uint8_t                                frame_type;
 *     uint8_t                                RefFrameSignBias;
 *     uint8_t                                OrderHint;
 *     uint8_t                                SavedOrderHints[8];
 * } WisStdVideoDecodeAV1ReferenceInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoDecodeAV1ReferenceInfo {
 *     wis::StdVideoDecodeAV1ReferenceInfoFlags flags;
 *     std::uint8_t                             frame_type;
 *     std::uint8_t                             RefFrameSignBias;
 *     std::uint8_t                             OrderHint;
 *     std::array<std::uint8_t, 8>              SavedOrderHints;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoDecodeAV1ReferenceInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Reference information flags.
 * - `frame_type` Frame type of the reference frame.
 * - `RefFrameSignBias` Specifies the direction of the reference frame relative to other references used in motion
 * vector derivation.
 * - `OrderHint` Order hint of the reference frame.
 * - `SavedOrderHints` Saved order hints when this reference frame was decoded.
 * \endcond
 *
 * @section WisStdVideoDecodeAV1ReferenceInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoDecodeAV1ReferenceInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisVideoDecodePictureDesc
 * \endcond
 */
