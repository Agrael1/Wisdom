/**
 * @struct WisStdVideoAV1SequenceHeaderFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1SequenceHeaderFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1SequenceHeaderFlags {
 *     uint32_t still_picture : 1;
 *     uint32_t reduced_still_picture_header : 1;
 *     uint32_t use_128x128_superblock : 1;
 *     uint32_t enable_filter_intra : 1;
 *     uint32_t enable_intra_edge_filter : 1;
 *     uint32_t enable_interintra_compound : 1;
 *     uint32_t enable_masked_compound : 1;
 *     uint32_t enable_warped_motion : 1;
 *     uint32_t enable_dual_filter : 1;
 *     uint32_t enable_order_hint : 1;
 *     uint32_t enable_jnt_comp : 1;
 *     uint32_t enable_ref_frame_mvs : 1;
 *     uint32_t frame_id_numbers_present_flag : 1;
 *     uint32_t enable_superres : 1;
 *     uint32_t enable_cdef : 1;
 *     uint32_t enable_restoration : 1;
 *     uint32_t film_grain_params_present : 1;
 *     uint32_t timing_info_present_flag : 1;
 *     uint32_t initial_display_delay_present_flag : 1;
 *     uint32_t reserved : 13;
 * } WisStdVideoAV1SequenceHeaderFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1SequenceHeaderFlags {
 *     std::uint32_t still_picture : 1;
 *     std::uint32_t reduced_still_picture_header : 1;
 *     std::uint32_t use_128x128_superblock : 1;
 *     std::uint32_t enable_filter_intra : 1;
 *     std::uint32_t enable_intra_edge_filter : 1;
 *     std::uint32_t enable_interintra_compound : 1;
 *     std::uint32_t enable_masked_compound : 1;
 *     std::uint32_t enable_warped_motion : 1;
 *     std::uint32_t enable_dual_filter : 1;
 *     std::uint32_t enable_order_hint : 1;
 *     std::uint32_t enable_jnt_comp : 1;
 *     std::uint32_t enable_ref_frame_mvs : 1;
 *     std::uint32_t frame_id_numbers_present_flag : 1;
 *     std::uint32_t enable_superres : 1;
 *     std::uint32_t enable_cdef : 1;
 *     std::uint32_t enable_restoration : 1;
 *     std::uint32_t film_grain_params_present : 1;
 *     std::uint32_t timing_info_present_flag : 1;
 *     std::uint32_t initial_display_delay_present_flag : 1;
 *     std::uint32_t reserved : 13;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1SequenceHeaderFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `still_picture` Specifies if the video sequence contains a single still picture.
 * - `reduced_still_picture_header` Specifies if reduced header parameters are used for a still picture.
 * - `use_128x128_superblock` Specifies if superblocks are 128x128 or 64x64.
 * - `enable_filter_intra` Specifies if the filter intra predictor can be used.
 * - `enable_intra_edge_filter` Specifies if intra edge filtering can be used.
 * - `enable_interintra_compound` Specifies if inter-intra compound prediction can be used.
 * - `enable_masked_compound` Specifies if masked compound prediction can be used.
 * - `enable_warped_motion` Specifies if warped motion can be used.
 * - `enable_dual_filter` Specifies if dual interpolation filters can be used.
 * - `enable_order_hint` Specifies if order hints are used.
 * - `enable_jnt_comp` Specifies if the distance weights process is used for compound prediction.
 * - `enable_ref_frame_mvs` Specifies if reference frame motion vectors are present.
 * - `frame_id_numbers_present_flag` Specifies if frame ID numbers are present.
 * - `enable_superres` Specifies if the superresolution feature can be used.
 * - `enable_cdef` Specifies if the CDEF filtering process can be used.
 * - `enable_restoration` Specifies if loop restoration can be used.
 * - `film_grain_params_present` Specifies if film grain parameters are present.
 * - `timing_info_present_flag` Specifies if timing info is present.
 * - `initial_display_delay_present_flag` Specifies if the initial display delay info is present.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoAV1SequenceHeaderFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1SequenceHeaderFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1SequenceHeader
 * \endcond
 */
