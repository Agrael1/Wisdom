/**
 * @struct WisVideoDecoderDesc
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecoderDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisVideoDecoderDesc {
 *     uint32_t           width;
 *     uint32_t           height;
 *     WisDataFormat      image_format;
 *     WisStdCodecProfile codec_profile;
 *     uint32_t           decode_picture_buffer_count;
 * } WisVideoDecoderDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  VideoDecoderDesc {
 *     std::uint32_t        width;
 *     std::uint32_t        height;
 *     wis::DataFormat      image_format;
 *     wis::StdCodecProfile codec_profile;
 *     std::uint32_t        decode_picture_buffer_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoDecoderDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `width` Width of the video frame in pixels.
 * - `height` Height of the video frame in pixels.
 * - `image_format` The data format of the output video frames. This field specifies the expected format of the decoded
 * video frames that will be produced by the video decoder, and can influence the supported bit depths and chroma
 * subsampling formats.
 * - `codec_profile` The video codec profile that the decoder will use for decoding. This field specifies the profile of
 * the video codec that the decoder will use for decoding video frames, and can influence the supported bit depths and
 * chroma subsampling formats.
 * - `decode_picture_buffer_count` The number of decode buffers that the decoder will use for decoding video frames.
 * \endcond
 *
 * @section WisVideoDecoderDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecoderDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisVideoDecodingExtensionCreateDecoder
 * \endcond
 */
