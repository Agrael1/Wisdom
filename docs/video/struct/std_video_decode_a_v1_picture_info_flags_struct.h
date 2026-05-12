/**
 * @struct WisStdVideoDecodeAV1PictureInfoFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoDecodeAV1PictureInfoFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoDecodeAV1PictureInfoFlags {
 *     uint32_t error_resilient_mode : 1;
 *     uint32_t disable_cdf_update : 1;
 *     uint32_t use_superres : 1;
 *     uint32_t render_and_frame_size_different : 1;
 *     uint32_t allow_screen_content_tools : 1;
 *     uint32_t is_filter_switchable : 1;
 *     uint32_t force_integer_mv : 1;
 *     uint32_t frame_size_override_flag : 1;
 *     uint32_t buffer_removal_time_present_flag : 1;
 *     uint32_t allow_intrabc : 1;
 *     uint32_t frame_refs_short_signaling : 1;
 *     uint32_t allow_high_precision_mv : 1;
 *     uint32_t is_motion_mode_switchable : 1;
 *     uint32_t use_ref_frame_mvs : 1;
 *     uint32_t disable_frame_end_update_cdf : 1;
 *     uint32_t allow_warped_motion : 1;
 *     uint32_t reduced_tx_set : 1;
 *     uint32_t reference_select : 1;
 *     uint32_t skip_mode_present : 1;
 *     uint32_t delta_q_present : 1;
 *     uint32_t delta_lf_present : 1;
 *     uint32_t delta_lf_multi : 1;
 *     uint32_t segmentation_enabled : 1;
 *     uint32_t segmentation_update_map : 1;
 *     uint32_t segmentation_temporal_update : 1;
 *     uint32_t segmentation_update_data : 1;
 *     uint32_t UsesLr : 1;
 *     uint32_t usesChromaLr : 1;
 *     uint32_t apply_grain : 1;
 *     uint32_t reserved : 3;
 * } WisStdVideoDecodeAV1PictureInfoFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoDecodeAV1PictureInfoFlags {
 *     std::uint32_t error_resilient_mode : 1;
 *     std::uint32_t disable_cdf_update : 1;
 *     std::uint32_t use_superres : 1;
 *     std::uint32_t render_and_frame_size_different : 1;
 *     std::uint32_t allow_screen_content_tools : 1;
 *     std::uint32_t is_filter_switchable : 1;
 *     std::uint32_t force_integer_mv : 1;
 *     std::uint32_t frame_size_override_flag : 1;
 *     std::uint32_t buffer_removal_time_present_flag : 1;
 *     std::uint32_t allow_intrabc : 1;
 *     std::uint32_t frame_refs_short_signaling : 1;
 *     std::uint32_t allow_high_precision_mv : 1;
 *     std::uint32_t is_motion_mode_switchable : 1;
 *     std::uint32_t use_ref_frame_mvs : 1;
 *     std::uint32_t disable_frame_end_update_cdf : 1;
 *     std::uint32_t allow_warped_motion : 1;
 *     std::uint32_t reduced_tx_set : 1;
 *     std::uint32_t reference_select : 1;
 *     std::uint32_t skip_mode_present : 1;
 *     std::uint32_t delta_q_present : 1;
 *     std::uint32_t delta_lf_present : 1;
 *     std::uint32_t delta_lf_multi : 1;
 *     std::uint32_t segmentation_enabled : 1;
 *     std::uint32_t segmentation_update_map : 1;
 *     std::uint32_t segmentation_temporal_update : 1;
 *     std::uint32_t segmentation_update_data : 1;
 *     std::uint32_t UsesLr : 1;
 *     std::uint32_t usesChromaLr : 1;
 *     std::uint32_t apply_grain : 1;
 *     std::uint32_t reserved : 3;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoDecodeAV1PictureInfoFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `error_resilient_mode` Indicates error resilient mode is enabled.
 * - `disable_cdf_update` Indicates CDF update is disabled.
 * - `use_superres` Indicates superresolution is enabled for this frame.
 * - `render_and_frame_size_different` Indicates actual frame size and render frame size are different.
 * - `allow_screen_content_tools` Indicates screen content tools are allowed.
 * - `is_filter_switchable` Indicates whether interpolation filter is switchable.
 * - `force_integer_mv` Indicates whether motion vectors @wis_must be forced to integer.
 * - `frame_size_override_flag` Indicates if frame size override is set.
 * - `buffer_removal_time_present_flag` Indicates whether buffer removal time is present.
 * - `allow_intrabc` Indicates if intra block copy is allowed.
 * - `frame_refs_short_signaling` Indicates if reference frames are completely decided by last_frame_idx.
 * - `allow_high_precision_mv` Indicates whether high precision motion vectors are allowed.
 * - `is_motion_mode_switchable` Indicates whether motion mode is switchable.
 * - `use_ref_frame_mvs` Indicates whether reference frame MVs are used.
 * - `disable_frame_end_update_cdf` Specifies whether the frame end CDF update is skipped.
 * - `allow_warped_motion` Indicates whether warped motion is allowed for this frame.
 * - `reduced_tx_set` Indicates whether the frame uses a reduced transform set.
 * - `reference_select` Specifies that the mode info for inter blocks contains the syntax element comp_mode.
 * - `skip_mode_present` Specifies whether skip mode is allowed.
 * - `delta_q_present` Specifies whether a delta q index is present for the frame.
 * - `delta_lf_present` Specifies whether delta loop filter values are present.
 * - `delta_lf_multi` Specifies whether independent delta loop filter values are used.
 * - `segmentation_enabled` Indicates if segmentation is enabled.
 * - `segmentation_update_map` Indicates if segmentation map is updated.
 * - `segmentation_temporal_update` Indicates if temporal segmentation is updated.
 * - `segmentation_update_data` Indicates if segmentation feature data is updated.
 * - `UsesLr` Indicates if loop restoration is used.
 * - `usesChromaLr` Indicates if loop restoration is used for chroma.
 * - `apply_grain` Indicates if film grain @wis_should be applied.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoDecodeAV1PictureInfoFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoDecodeAV1PictureInfoFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
