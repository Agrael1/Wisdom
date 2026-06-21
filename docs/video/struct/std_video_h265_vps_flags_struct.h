/**
 * @struct WisStdVideoH265VpsFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265VpsFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265VpsFlags {
 *     uint32_t vps_temporal_id_nesting_flag : 1;
 *     uint32_t vps_sub_layer_ordering_info_present_flag : 1;
 *     uint32_t vps_timing_info_present_flag : 1;
 *     uint32_t vps_poc_proportional_to_timing_flag : 1;
 *     uint32_t reserved : 28;
 * } WisStdVideoH265VpsFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265VpsFlags {
 *     std::uint32_t vps_temporal_id_nesting_flag : 1;
 *     std::uint32_t vps_sub_layer_ordering_info_present_flag : 1;
 *     std::uint32_t vps_timing_info_present_flag : 1;
 *     std::uint32_t vps_poc_proportional_to_timing_flag : 1;
 *     std::uint32_t reserved : 28;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265VpsFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `vps_temporal_id_nesting_flag` Specifies that all referenced PPs have temporal_id equal to 0.
 * - `vps_sub_layer_ordering_info_present_flag` Specifies that ordering info is present for sub-layers.
 * - `vps_timing_info_present_flag` Specifies that timing info is present.
 * - `vps_poc_proportional_to_timing_flag` Specifies that POC is proportional to timing.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoH265VpsFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265VpsFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265VideoParameterSet
 * \endcond
 */
