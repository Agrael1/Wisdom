/**
 * @struct WisStdVideoH265VideoParameterSet
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265VideoParameterSet_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265VideoParameterSet {
 *     WisStdVideoH265VpsFlags                flags;
 *     uint8_t                                vps_video_parameter_set_id;
 *     uint8_t                                vps_max_sub_layers_minus1;
 *     uint8_t                                reserved1;
 *     uint8_t                                reserved2;
 *     uint32_t                               vps_num_units_in_tick;
 *     uint32_t                               vps_time_scale;
 *     uint32_t                               vps_num_ticks_poc_diff_one_minus1;
 *     uint32_t                               reserved3;
 *     const WisStdVideoH265DecPicBufMgr*     pDecPicBufMgr;
 *     const WisStdVideoH265HrdParameters*    pHrdParameters;
 *     const WisStdVideoH265ProfileTierLevel* pProfileTierLevel;
 * } WisStdVideoH265VideoParameterSet;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265VideoParameterSet {
 *     wis::StdVideoH265VpsFlags                flags;
 *     std::uint8_t                             vps_video_parameter_set_id;
 *     std::uint8_t                             vps_max_sub_layers_minus1;
 *     std::uint8_t                             reserved1;
 *     std::uint8_t                             reserved2;
 *     std::uint32_t                            vps_num_units_in_tick;
 *     std::uint32_t                            vps_time_scale;
 *     std::uint32_t                            vps_num_ticks_poc_diff_one_minus1;
 *     std::uint32_t                            reserved3;
 *     const wis::StdVideoH265DecPicBufMgr*     pDecPicBufMgr;
 *     const wis::StdVideoH265HrdParameters*    pHrdParameters;
 *     const wis::StdVideoH265ProfileTierLevel* pProfileTierLevel;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265VideoParameterSet_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` VPS flags.
 * - `vps_video_parameter_set_id` Specifies the VPS ID.
 * - `vps_max_sub_layers_minus1` Specifies the max number of temporal sub-layers minus 1.
 * - `reserved1` No description.
 * - `reserved2` No description.
 * - `vps_num_units_in_tick` Number of time units per tick.
 * - `vps_time_scale` Time scale.
 * - `vps_num_ticks_poc_diff_one_minus1` VPS num ticks POC diff one minus 1.
 * - `reserved3` No description.
 * - `pDecPicBufMgr` Pointer to decoded picture buffer management info.
 * - `pHrdParameters` Pointer to HRD parameters.
 * - `pProfileTierLevel` Pointer to profile tier level.
 * \endcond
 *
 * @section WisStdVideoH265VideoParameterSet_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265VideoParameterSet_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
