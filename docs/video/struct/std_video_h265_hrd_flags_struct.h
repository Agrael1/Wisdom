/**
 * @struct WisStdVideoH265HrdFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265HrdFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265HrdFlags {
 *     uint32_t nal_hrd_parameters_present_flag : 1;
 *     uint32_t vcl_hrd_parameters_present_flag : 1;
 *     uint32_t sub_pic_hrd_params_present_flag : 1;
 *     uint32_t sub_pic_cpb_params_in_pic_timing_sei_flag : 1;
 *     uint32_t fixed_pic_rate_general_flag : 8;
 *     uint32_t fixed_pic_rate_within_cvs_flag : 8;
 *     uint32_t low_delay_hrd_flag : 8;
 *     uint32_t reserved : 4;
 * } WisStdVideoH265HrdFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265HrdFlags {
 *     std::uint32_t nal_hrd_parameters_present_flag : 1;
 *     std::uint32_t vcl_hrd_parameters_present_flag : 1;
 *     std::uint32_t sub_pic_hrd_params_present_flag : 1;
 *     std::uint32_t sub_pic_cpb_params_in_pic_timing_sei_flag : 1;
 *     std::uint32_t fixed_pic_rate_general_flag : 8;
 *     std::uint32_t fixed_pic_rate_within_cvs_flag : 8;
 *     std::uint32_t low_delay_hrd_flag : 8;
 *     std::uint32_t reserved : 4;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265HrdFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `nal_hrd_parameters_present_flag` Specifies that NAL HRD parameters are present.
 * - `vcl_hrd_parameters_present_flag` Specifies that VCL HRD parameters are present.
 * - `sub_pic_hrd_params_present_flag` Specifies that sub-picture HRD parameters are present.
 * - `sub_pic_cpb_params_in_pic_timing_sei_flag` Specifies that sub-picture CPB parameters are present in the pic timing
 * SEI.
 * - `fixed_pic_rate_general_flag` An 8-bit flag per sub-layer indicating fixed picture rate.
 * - `fixed_pic_rate_within_cvs_flag` An 8-bit flag per sub-layer indicating fixed pic rate within CVS.
 * - `low_delay_hrd_flag` An 8-bit flag per sub-layer indicating low delay HRD.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoH265HrdFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265HrdFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265HrdParameters
 * \endcond
 */
