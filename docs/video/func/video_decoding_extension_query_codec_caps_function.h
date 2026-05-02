/**
 * @struct wisVideoDecodingExtensionQueryCodecCaps
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodingExtensionQueryCodecCaps_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVideoDecodingExtensionQueryCodecCaps(WisVideoDecodingExtension* self,
 *                                                   const WisVideoCodecDesc*   codec_desc,
 *                                                   WisVideoDecodeInfo*        decode_info);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKVideoDecodingExtensionQueryCodecCaps(WisVKVideoDecodingExtension* self,
 *                                                     const WisVideoCodecDesc*     codec_desc,
 *                                                     WisVideoDecodeInfo*          decode_info);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12VideoDecodingExtensionQueryCodecCaps(WisDX12VideoDecodingExtension* self,
 *                                                       const WisVideoCodecDesc*       codec_desc,
 *                                                       WisVideoDecodeInfo*            decode_info);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecodeInfo VideoDecodingExtension::QueryCodecCaps(const wis::VideoCodecDesc& codec_desc,
 *                                                                           wis::Result&               out_result)
 * noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecodeInfo VKVideoDecodingExtension::QueryCodecCaps(const wis::VideoCodecDesc& codec_desc,
 *                                                                             wis::Result&               out_result)
 * noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecodeInfo DX12VideoDecodingExtension::QueryCodecCaps(const wis::VideoCodecDesc& codec_desc,
 *                                                                               wis::Result&               out_result)
 * noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecodingExtensionQueryCodecCaps_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecodingExtension instance.
 * - `codec_desc` Information about the video codec to query capabilities for. The 'codec' field @wis_should specify the
 * codec to check, and the function will fill in the supported bit depths and chroma subsampling formats for that codec.
 * - `decode_info` Information about the video decode capabilities of the current device, including maximum supported
 * video frame dimensions.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisVideoDecodingExtensionQueryCodecCaps_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodingExtensionQueryCodecCaps_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
