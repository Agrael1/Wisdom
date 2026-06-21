/**
 * @struct WisStdVideoDecodeH265PictureInfo
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoDecodeH265PictureInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoDecodeH265PictureInfo {
 *     WisStdVideoDecodeH265PictureInfoFlags flags;
 *     uint8_t                               sps_video_parameter_set_id;
 *     uint8_t                               pps_seq_parameter_set_id;
 *     uint8_t                               pps_pic_parameter_set_id;
 *     uint8_t                               NumDeltaPocsOfRefRpsIdx;
 *     int32_t                               PicOrderCntVal;
 *     uint16_t                              NumBitsForSTRefPicSetInSlice;
 *     uint16_t                              reserved;
 *     uint8_t                               RefPicSetStCurrBefore[8];
 *     uint8_t                               RefPicSetStCurrAfter[8];
 *     uint8_t                               RefPicSetLtCurr[8];
 * } WisStdVideoDecodeH265PictureInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoDecodeH265PictureInfo {
 *     wis::StdVideoDecodeH265PictureInfoFlags flags;
 *     std::uint8_t                            sps_video_parameter_set_id;
 *     std::uint8_t                            pps_seq_parameter_set_id;
 *     std::uint8_t                            pps_pic_parameter_set_id;
 *     std::uint8_t                            NumDeltaPocsOfRefRpsIdx;
 *     std::int32_t                            PicOrderCntVal;
 *     std::uint16_t                           NumBitsForSTRefPicSetInSlice;
 *     std::uint16_t                           reserved;
 *     std::array<std::uint8_t, 8>             RefPicSetStCurrBefore;
 *     std::array<std::uint8_t, 8>             RefPicSetStCurrAfter;
 *     std::array<std::uint8_t, 8>             RefPicSetLtCurr;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoDecodeH265PictureInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Decode picture flags.
 * - `sps_video_parameter_set_id` SPS VPS ID.
 * - `pps_seq_parameter_set_id` PPS SPS ID.
 * - `pps_pic_parameter_set_id` PPS ID.
 * - `NumDeltaPocsOfRefRpsIdx` Number of delta POCs of reference RPS index.
 * - `PicOrderCntVal` Picture order count value.
 * - `NumBitsForSTRefPicSetInSlice` Number of bits for short-term ref pic set in slice.
 * - `reserved` No description.
 * - `RefPicSetStCurrBefore` Short-term reference pictures before the current picture in output order.
 * - `RefPicSetStCurrAfter` Short-term reference pictures after the current picture in output order.
 * - `RefPicSetLtCurr` Long-term reference pictures currently used.
 * \endcond
 *
 * @section WisStdVideoDecodeH265PictureInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoDecodeH265PictureInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisVideoDecodePictureDesc
 * \endcond
 */
