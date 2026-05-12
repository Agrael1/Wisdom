/**
 * @struct WisStdVideoAV1GlobalMotion
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1GlobalMotion_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1GlobalMotion {
 *     uint8_t GmType[8];
 *     int32_t gm_params[8*6];
 * } WisStdVideoAV1GlobalMotion;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1GlobalMotion {
 *     std::array<std::uint8_t, 8> GmType;
 *     std::array<std::int32_t, 8*6> gm_params;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1GlobalMotion_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `GmType` Array specifying the global motion type for each reference frame.
 * - `gm_params` Array of global motion parameters.
 * \endcond
 *
 * @section WisStdVideoAV1GlobalMotion_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1GlobalMotion_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
