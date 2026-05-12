/**
 * @struct WisStdVideoAV1FilmGrainFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1FilmGrainFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1FilmGrainFlags {
 *     uint32_t chroma_scaling_from_luma : 1;
 *     uint32_t overlap_flag : 1;
 *     uint32_t clip_to_restricted_range : 1;
 *     uint32_t update_grain : 1;
 *     uint32_t reserved : 28;
 * } WisStdVideoAV1FilmGrainFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1FilmGrainFlags {
 *     std::uint32_t chroma_scaling_from_luma : 1;
 *     std::uint32_t overlap_flag : 1;
 *     std::uint32_t clip_to_restricted_range : 1;
 *     std::uint32_t update_grain : 1;
 *     std::uint32_t reserved : 28;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1FilmGrainFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `chroma_scaling_from_luma` Flag indicating that chroma scaling is derived from luma.
 * - `overlap_flag` Flag indicating overlapping film grain blocks.
 * - `clip_to_restricted_range` Flag indicating clipping to restricted range.
 * - `update_grain` Flag indicating the film grain parameters are updated in this frame.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoAV1FilmGrainFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1FilmGrainFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1FilmGrain
 * \endcond
 */
