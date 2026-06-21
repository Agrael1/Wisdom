/**
 * @struct WisStdVideoH265DecPicBufMgr
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265DecPicBufMgr_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265DecPicBufMgr {
 *     uint32_t max_latency_increase_plus1[7];
 *     uint8_t  max_dec_pic_buffering_minus1[7];
 *     uint8_t  max_num_reorder_pics[7];
 * } WisStdVideoH265DecPicBufMgr;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265DecPicBufMgr {
 *     std::array<std::uint32_t, 7> max_latency_increase_plus1;
 *     std::array<std::uint8_t, 7> max_dec_pic_buffering_minus1;
 *     std::array<std::uint8_t, 7> max_num_reorder_pics;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265DecPicBufMgr_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `max_latency_increase_plus1` Specifies max latency increase for each sub-layer.
 * - `max_dec_pic_buffering_minus1` Specifies max decoded picture buffering minus 1 for each sub-layer.
 * - `max_num_reorder_pics` Specifies max number of reorder pictures for each sub-layer.
 * \endcond
 *
 * @section WisStdVideoH265DecPicBufMgr_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265DecPicBufMgr_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265VideoParameterSet, WisStdVideoH265SequenceParameterSet
 * \endcond
 */
