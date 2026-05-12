/**
 * @struct WisStdVideoAV1LoopFilterFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1LoopFilterFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1LoopFilterFlags {
 *     uint32_t loop_filter_delta_enabled : 1;
 *     uint32_t loop_filter_delta_update : 1;
 *     uint32_t reserved : 30;
 * } WisStdVideoAV1LoopFilterFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1LoopFilterFlags {
 *     std::uint32_t loop_filter_delta_enabled : 1;
 *     std::uint32_t loop_filter_delta_update : 1;
 *     std::uint32_t reserved : 30;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1LoopFilterFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `loop_filter_delta_enabled` Indicates whether the filter level depends on the mode and reference frame used to
 * predict a block.
 * - `loop_filter_delta_update` Indicates whether additional syntax elements are present that specify which mode and
 * reference frame deltas are to be updated.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoAV1LoopFilterFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1LoopFilterFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1LoopFilter
 * \endcond
 */
