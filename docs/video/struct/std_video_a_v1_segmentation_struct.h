/**
 * @struct WisStdVideoAV1Segmentation
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1Segmentation_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1Segmentation {
 *     uint8_t FeatureEnabled[8];
 *     int16_t FeatureData[8*8];
 * } WisStdVideoAV1Segmentation;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1Segmentation {
 *     std::array<std::uint8_t, 8> FeatureEnabled;
 *     std::array<std::int16_t, 8*8> FeatureData;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1Segmentation_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `FeatureEnabled` Array specifying whether the feature is enabled for a segment.
 * - `FeatureData` Array specifying the feature data for a segment feature.
 * \endcond
 *
 * @section WisStdVideoAV1Segmentation_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1Segmentation_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
