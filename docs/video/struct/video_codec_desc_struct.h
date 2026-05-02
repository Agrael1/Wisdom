/**
 * @struct WisVideoCodecDesc
 * @ingroup Structures Video
 *
 *
 * @section WisVideoCodecDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisVideoCodecDesc {
 *     WisStdCodecProfile   codec_profile;
 *     WisComponentBitDepth bit_depth;
 *     WisChromaSubsampling chroma_subsampling;
 * } WisVideoCodecDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoCodecDesc {
 *     wis::StdCodecProfile   codec_profile;
 *     wis::ComponentBitDepth bit_depth;
 *     wis::ChromaSubsampling chroma_subsampling;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoCodecDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `codec_profile` The video codec to query capabilities for.
 * - `bit_depth` Supported bit depths for this codec, represented as a bitmask of ComponentBitDepth flags.
 * - `chroma_subsampling` Supported chroma subsampling formats for this codec, represented as a bitmask of
 * ChromaSubsampling flags.
 * \endcond
 *
 * @section WisVideoCodecDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoCodecDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisVideoDecodingExtensionQueryCodecCaps
 * \endcond
 */
