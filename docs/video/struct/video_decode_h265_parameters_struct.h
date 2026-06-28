/**
 * @struct WisVideoDecodeH265Parameters
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecodeH265Parameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisVideoDecodeH265Parameters {
 *     uint32_t                                   max_vps_count;
 *     uint32_t                                   max_sps_count;
 *     uint32_t                                   max_pps_count;
 *     const WisStdVideoH265VideoParameterSet*    vps;
 *     uint32_t                                   vps_count;
 *     const WisStdVideoH265SequenceParameterSet* sps;
 *     uint32_t                                   sps_count;
 *     const WisStdVideoH265PictureParameterSet*  pps;
 *     uint32_t                                   pps_count;
 * } WisVideoDecodeH265Parameters;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoDecodeH265Parameters {
 *     std::uint32_t                                max_vps_count;
 *     std::uint32_t                                max_sps_count;
 *     std::uint32_t                                max_pps_count;
 *     const wis::StdVideoH265VideoParameterSet*    vps;
 *     std::uint32_t                                vps_count;
 *     const wis::StdVideoH265SequenceParameterSet* sps;
 *     std::uint32_t                                sps_count;
 *     const wis::StdVideoH265PictureParameterSet*  pps;
 *     std::uint32_t                                pps_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoDecodeH265Parameters_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `max_vps_count` Maximum number of VPS entries the slot can hold.
 * - `max_sps_count` Maximum number of SPS entries the slot can hold.
 * - `max_pps_count` Maximum number of PPS entries the slot can hold.
 * - `vps` Pointer to an array of VPS data.
 * - `vps_count` Number of VPS entries.
 * - `sps` Pointer to an array of SPS data.
 * - `sps_count` Number of SPS entries.
 * - `pps` Pointer to an array of PPS data.
 * - `pps_count` Number of PPS entries.
 * \endcond
 *
 * @section WisVideoDecodeH265Parameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecodeH265Parameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisVideoDecodeParameters
 * \endcond
 */
