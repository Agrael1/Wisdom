/**
 * @struct WisStdVideoAV1LoopFilter
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1LoopFilter_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1LoopFilter {
 *     WisStdVideoAV1LoopFilterFlags flags;
 *     uint8_t                       loop_filter_level[4];
 *     uint8_t                       loop_filter_sharpness;
 *     uint8_t                       update_ref_delta;
 *     int8_t                        loop_filter_ref_deltas[8];
 *     uint8_t                       update_mode_delta;
 *     int8_t                        loop_filter_mode_deltas[2];
 * } WisStdVideoAV1LoopFilter;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1LoopFilter {
 *     wis::StdVideoAV1LoopFilterFlags flags;
 *     std::array<std::uint8_t, 4>     loop_filter_level;
 *     std::uint8_t                    loop_filter_sharpness;
 *     std::uint8_t                    update_ref_delta;
 *     std::array<std::int8_t, 8>      loop_filter_ref_deltas;
 *     std::uint8_t                    update_mode_delta;
 *     std::array<std::int8_t, 2>      loop_filter_mode_deltas;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1LoopFilter_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Loop filter flags.
 * - `loop_filter_level` Array containing loop filter strength values.
 * - `loop_filter_sharpness` Loop filter sharpness.
 * - `update_ref_delta` Indicates that the loop filter ref deltas are to be updated.
 * - `loop_filter_ref_deltas` Loop filter reference deltas.
 * - `update_mode_delta` Indicates that the loop filter mode deltas are to be updated.
 * - `loop_filter_mode_deltas` Loop filter mode deltas.
 * \endcond
 *
 * @section WisStdVideoAV1LoopFilter_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1LoopFilter_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
