/**
 * @struct WisStdVideoAV1TimingInfo
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1TimingInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1TimingInfo {
 *     WisStdVideoAV1TimingInfoFlags flags;
 *     uint32_t                      num_units_in_display_tick;
 *     uint32_t                      time_scale;
 *     uint32_t                      num_ticks_per_picture_minus_1;
 * } WisStdVideoAV1TimingInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1TimingInfo {
 *     wis::StdVideoAV1TimingInfoFlags flags;
 *     std::uint32_t                   num_units_in_display_tick;
 *     std::uint32_t                   time_scale;
 *     std::uint32_t                   num_ticks_per_picture_minus_1;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1TimingInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Timing flags.
 * - `num_units_in_display_tick` Number of units in a display tick.
 * - `time_scale` Time scale.
 * - `num_ticks_per_picture_minus_1` Ticks per picture minus 1.
 * \endcond
 *
 * @section WisStdVideoAV1TimingInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1TimingInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1SequenceHeader
 * \endcond
 */
