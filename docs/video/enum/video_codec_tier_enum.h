/**
 * @struct WisVideoCodecTier WisVideoCodecTier
 * @ingroup Enumerations Video
 *
 * @section WisVideoCodecTier_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisVideoCodecTier {
 *     WisVideoCodecTierBaseline = 0,
 *     WisVideoCodecTierMain = 1,
 *     WisVideoCodecTierHigh = 2,
 *     WisVideoCodecTierProfessional = 3,
 * } WisVideoCodecTier;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class VideoCodecTier {
 *     Baseline = 0,
 *     Main = 1,
 *     High = 2,
 *     Professional = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoCodecTier_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Video codec tier levels. Used to specify the tier level of a video codec, which indicates the performance and feature
 * capabilities of the codec implementation.
 *
 * Values:
 * - `WisVideoCodecTierBaseline = 0`: Tier 0: Baseline tier with basic features and performance capabilities. Used only
 * for H.264. Otherwise it is mapped to Main profile.
 * - `WisVideoCodecTierMain = 1`: Tier 1: Main tier with improved features and performance capabilities compared to Tier
 * 0.
 * - `WisVideoCodecTierHigh = 2`: Tier 2: High tier with advanced features and performance capabilities compared to
 * Tier 1. Used for H.264/AV1.
 * - `WisVideoCodecTierProfessional = 3`: Tier 3: Professional tier with the highest level of features and performance
 * capabilities. Used for H.265.
 * \endcond
 *
 *
 * @section WisVideoCodecTier_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisVideoCodecDesc
 * \endcond
 */
