/**
 * @struct WisVideoDecodeParameters
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecodeParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisVideoDecodeParameters {
 *     WisStdCodecProfile                  codec;
 *     const WisVideoDecodeAV1Parameters*  av1;
 *     const WisVideoDecodeH265Parameters* h265;
 * } WisVideoDecodeParameters;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoDecodeParameters {
 *     wis::StdCodecProfile                  codec;
 *     const wis::VideoDecodeAV1Parameters*  av1;
 *     const wis::VideoDecodeH265Parameters* h265;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoDecodeParameters_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `codec` Codec type selector. Determines which parameter set is valid.
 * - `av1` AV1 decoder parameters (valid when codec is an AV1 profile).
 * - `h265` H.265 decoder parameters (valid when codec is an H.265 profile).
 * \endcond
 *
 * @section WisVideoDecodeParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecodeParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisVideoDecodingExtensionCreateParameters
 * \endcond
 */
