/**
 * @struct WisStdVideoAV1LoopRestoration
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1LoopRestoration_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1LoopRestoration {
 *     WisStdVideoAV1FrameRestorationType FrameRestorationType[3];
 *     uint16_t                           LoopRestorationSize[3];
 * } WisStdVideoAV1LoopRestoration;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1LoopRestoration {
 *     std::array<wis::StdVideoAV1FrameRestorationType, 3> FrameRestorationType;
 *     std::array<std::uint16_t, 3>         LoopRestorationSize;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1LoopRestoration_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `FrameRestorationType` Array specifying the loop restoration type for each plane (Y, U, V).
 * - `LoopRestorationSize` Array specifying the size of loop restoration units for each plane.
 * \endcond
 *
 * @section WisStdVideoAV1LoopRestoration_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1LoopRestoration_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
