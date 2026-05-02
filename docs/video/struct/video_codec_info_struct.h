/**
 * @struct WisVideoCodecInfo
 * @ingroup Structures Video
 *
 *
 * @section WisVideoCodecInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisVideoCodecInfo {
 *     WisComponentBitDepth bit_depths;
 *     WisChromaSubsampling chroma_subsamplings;
 * } WisVideoCodecInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoCodecInfo {
 *     wis::ComponentBitDepth bit_depths;
 *     wis::ChromaSubsampling chroma_subsamplings;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoCodecInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `bit_depths` Supported bit depths for this codec, represented as a bitmask of ComponentBitDepth flags.
 * - `chroma_subsamplings` Supported chroma subsampling formats for this codec, represented as a bitmask of
 * ChromaSubsampling flags.
 * \endcond
 *
 * @section WisVideoCodecInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoCodecInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisVideoDecodingExtensionQueryCodecCaps
 * \endcond
 */
