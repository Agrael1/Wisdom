/**
 * @struct WisVideoDecodePictureDesc
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecodePictureDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVideoDecodePictureDesc {
 *     WisStdCodecProfile                        codec;
 *     const WisStdVideoDecodeAV1PictureInfo*    av1_picture_info;
 *     const WisStdVideoDecodeH265PictureInfo*   h265_picture_info;
 *     const WisStdVideoDecodeAV1ReferenceInfo*  av1_reference_info;
 *     const WisStdVideoDecodeH265ReferenceInfo* h265_reference_info;
 *     uint32_t                                  reference_frame_count;
 * } WisVideoDecodePictureDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVKVideoDecodePictureDesc {
 *     WisStdCodecProfile                        codec;
 *     const WisStdVideoDecodeAV1PictureInfo*    av1_picture_info;
 *     const WisStdVideoDecodeH265PictureInfo*   h265_picture_info;
 *     const WisStdVideoDecodeAV1ReferenceInfo*  av1_reference_info;
 *     const WisStdVideoDecodeH265ReferenceInfo* h265_reference_info;
 *     uint32_t                                  reference_frame_count;
 * } WisVKVideoDecodePictureDesc;
 *
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisDX12VideoDecodePictureDesc {
 *     WisStdCodecProfile                        codec;
 *     const WisStdVideoDecodeAV1PictureInfo*    av1_picture_info;
 *     const WisStdVideoDecodeH265PictureInfo*   h265_picture_info;
 *     const WisStdVideoDecodeAV1ReferenceInfo*  av1_reference_info;
 *     const WisStdVideoDecodeH265ReferenceInfo* h265_reference_info;
 *     uint32_t                                  reference_frame_count;
 * } WisDX12VideoDecodePictureDesc;
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VideoDecodePictureDesc
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VKVideoDecodePictureDesc ;
 *
 * // Provided by Wisdom 0.7.1.
 * struct DX12VideoDecodePictureDesc
 * ```
 * </details>
 *
 * \endcond
 *
 * @section WisVideoDecodePictureDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `codec` The codec profile of the current frame. Determines which picture info struct is valid.
 * - `av1_picture_info` AV1 picture information (valid when codec is an AV1 profile).
 * - `h265_picture_info` H.265 picture information (valid when codec is an H.265 profile).
 * - `av1_reference_info` AV1 reference information array (valid when codec is an AV1 profile).
 * - `h265_reference_info` H.265 reference information array (valid when codec is an H.265 profile).
 * - `reference_frame_count` Number of reference frames for this decode operation.
 *
 * \endcond
 *
 * @section WisVideoDecodePictureDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecodePictureDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
