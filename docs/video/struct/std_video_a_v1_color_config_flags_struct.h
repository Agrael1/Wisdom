/**
 * @struct WisStdVideoAV1ColorConfigFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1ColorConfigFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1ColorConfigFlags {
 *     uint32_t mono_chrome : 1;
 *     uint32_t color_range : 1;
 *     uint32_t separate_uv_delta_q : 1;
 *     uint32_t color_description_present_flag : 1;
 *     uint32_t reserved : 28;
 * } WisStdVideoAV1ColorConfigFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1ColorConfigFlags {
 *     std::uint32_t mono_chrome : 1;
 *     std::uint32_t color_range : 1;
 *     std::uint32_t separate_uv_delta_q : 1;
 *     std::uint32_t color_description_present_flag : 1;
 *     std::uint32_t reserved : 28;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1ColorConfigFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `mono_chrome` Indicates if the video does not contain U and V color planes.
 * - `color_range` Flag indicating if full color range is used.
 * - `separate_uv_delta_q` Flag indicating U and V planes have separate delta quantization.
 * - `color_description_present_flag` Indicates if color description is present.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoAV1ColorConfigFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1ColorConfigFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1ColorConfig
 * \endcond
 */
