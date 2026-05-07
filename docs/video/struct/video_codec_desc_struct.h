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
 *     WisStdCodecProfile codec_profile;
 *     WisDataFormat      image_format;
 *     uint32_t           width;
 *     uint32_t           height;
 * } WisVideoCodecDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoCodecDesc {
 *     wis::StdCodecProfile codec_profile;
 *     wis::DataFormat      image_format;
 *     std::uint32_t        width;
 *     std::uint32_t        height;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoCodecDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `codec_profile` The video codec to query capabilities for.
 * - `image_format` The data format of the video frames for this codec. This field is used to specify the expected
 * format of the video frames that will be decoded using this codec, and can influence the supported bit depths and
 * chroma subsampling formats.
 * - `width` Max width of the video frame in pixels.
 * - `height` Max height of the video frame in pixels.
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
