/**
 * @struct WisVideoDecodeParameterDesc
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecodeParameterDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisVideoDecodeParameterDesc {
 *     WisStdCodecProfile            codec;
 *     const WisVideoDecodeAV1Desc*  av1;
 *     const WisVideoDecodeH265Desc* h265;
 * } WisVideoDecodeParameterDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoDecodeParameterDesc {
 *     wis::StdCodecProfile            codec;
 *     const wis::VideoDecodeAV1Desc*  av1;
 *     const wis::VideoDecodeH265Desc* h265;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoDecodeParameterDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `codec` Codec type selector. Determines which parameter set is valid.
 * - `av1` AV1 decoder parameters (valid when codec is an AV1 profile).
 * - `h265` H.265 decoder parameters (valid when codec is an H.265 profile).
 * \endcond
 *
 * @section WisVideoDecodeParameterDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecodeParameterDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisVideoDecodingExtensionCreateParameters
 * \endcond
 */
