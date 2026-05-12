/**
 * @struct WisStdVideoDecodeAV1PictureInfo
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoDecodeAV1PictureInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoDecodeAV1PictureInfo {
 *     WisStdVideoDecodeAV1PictureInfoFlags flags;
 *     WisStdVideoAV1FrameType              frame_type;
 *     uint32_t                             current_frame_id;
 *     uint8_t                              OrderHint;
 *     uint8_t                              primary_ref_frame;
 *     uint8_t                              refresh_frame_flags;
 *     uint8_t                              reserved1;
 *     WisStdVideoAV1InterpolationFilter    interpolation_filter;
 *     WisStdVideoAV1TxMode                 TxMode;
 *     uint8_t                              delta_q_res;
 *     uint8_t                              delta_lf_res;
 *     uint8_t                              SkipModeFrame[2];
 *     uint8_t                              coded_denom;
 *     uint8_t                              reserved2[3];
 *     uint8_t                              OrderHints[8];
 *     uint32_t                             expectedFrameId[8];
 *     const WisStdVideoAV1TileInfo*        pTileInfo;
 *     const WisStdVideoAV1Quantization*    pQuantization;
 *     const WisStdVideoAV1Segmentation*    pSegmentation;
 *     const WisStdVideoAV1LoopFilter*      pLoopFilter;
 *     const WisStdVideoAV1CDEF*            pCDEF;
 *     const WisStdVideoAV1LoopRestoration* pLoopRestoration;
 *     const WisStdVideoAV1GlobalMotion*    pGlobalMotion;
 *     const WisStdVideoAV1FilmGrain*       pFilmGrain;
 * } WisStdVideoDecodeAV1PictureInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoDecodeAV1PictureInfo {
 *     wis::StdVideoDecodeAV1PictureInfoFlags flags;
 *     wis::StdVideoAV1FrameType              frame_type;
 *     std::uint32_t                          current_frame_id;
 *     std::uint8_t                           OrderHint;
 *     std::uint8_t                           primary_ref_frame;
 *     std::uint8_t                           refresh_frame_flags;
 *     std::uint8_t                           reserved1;
 *     wis::StdVideoAV1InterpolationFilter    interpolation_filter;
 *     wis::StdVideoAV1TxMode                 TxMode;
 *     std::uint8_t                           delta_q_res;
 *     std::uint8_t                           delta_lf_res;
 *     std::array<std::uint8_t, 2>            SkipModeFrame;
 *     std::uint8_t                           coded_denom;
 *     std::array<std::uint8_t, 3>            reserved2;
 *     std::array<std::uint8_t, 8>            OrderHints;
 *     std::array<std::uint32_t, 8>           expectedFrameId;
 *     const wis::StdVideoAV1TileInfo*        pTileInfo;
 *     const wis::StdVideoAV1Quantization*    pQuantization;
 *     const wis::StdVideoAV1Segmentation*    pSegmentation;
 *     const wis::StdVideoAV1LoopFilter*      pLoopFilter;
 *     const wis::StdVideoAV1CDEF*            pCDEF;
 *     const wis::StdVideoAV1LoopRestoration* pLoopRestoration;
 *     const wis::StdVideoAV1GlobalMotion*    pGlobalMotion;
 *     const wis::StdVideoAV1FilmGrain*       pFilmGrain;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoDecodeAV1PictureInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Decode picture info flags.
 * - `frame_type` Frame type: Key, Inter, Intra-only, or Switch.
 * - `current_frame_id` Specifies the frame ID for the current frame.
 * - `OrderHint` Order hint of the current frame used for motion vector scaling.
 * - `primary_ref_frame` Index of the reference frame containing the CDF values to be loaded at the start of the frame.
 * - `refresh_frame_flags` An 8-bit mask that specifies which reference frame slots will be updated with the current
 * frame.
 * - `reserved1` No description.
 * - `interpolation_filter` Specifies the filter selection used for performing inter prediction.
 * - `TxMode` Specifies how the transform size is determined.
 * - `delta_q_res` Specifies the left shift to be applied to decoded delta q values.
 * - `delta_lf_res` Specifies the left shift to be applied to decoded delta loop filter values.
 * - `SkipModeFrame` Specifies the indices of the reference frames to be used for skip mode.
 * - `coded_denom` Denominator for frame size calculation if superres is enabled.
 * - `reserved2` No description.
 * - `OrderHints` Order hints of the decoded reference frames.
 * - `expectedFrameId` Expected frame IDs for reference frames.
 * - `pTileInfo` Pointer to AV1 tile information.
 * - `pQuantization` Pointer to standard quantization matrices and values.
 * - `pSegmentation` Pointer to segmentation parameter information.
 * - `pLoopFilter` Pointer to loop filter parameters.
 * - `pCDEF` Pointer to CDEF parameters.
 * - `pLoopRestoration` Pointer to loop restoration parameters.
 * - `pGlobalMotion` Pointer to global motion parameters.
 * - `pFilmGrain` Pointer to film grain synthesis parameters.
 * \endcond
 *
 * @section WisStdVideoDecodeAV1PictureInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoDecodeAV1PictureInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
