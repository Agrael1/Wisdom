/**
 * @struct wisVideoDecodingExtensionCreateDecoder
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodingExtensionCreateDecoder_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVideoDecodingExtensionCreateDecoder(const WisVideoDecodingExtension* self,
 *                                                  const WisVideoDecoderDesc*       decoder_desc,
 *                                                  WisVideoDecoder*                 video_decoder);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKVideoDecodingExtensionCreateDecoder(const WisVKVideoDecodingExtension* self,
 *                                                    const WisVideoDecoderDesc*         decoder_desc,
 *                                                    WisVKVideoDecoder*                 video_decoder);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12VideoDecodingExtensionCreateDecoder(const WisDX12VideoDecodingExtension* self,
 *                                                      const WisVideoDecoderDesc*           decoder_desc,
 *                                                      WisDX12VideoDecoder*                 video_decoder);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecoder VideoDecodingExtension::CreateDecoder(const wis::VideoDecoderDesc& decoder_desc,
 *                                                                       wis::Result&                 out_result) const
 * noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VKVideoDecoder VKVideoDecodingExtension::CreateDecoder(const wis::VideoDecoderDesc& decoder_desc,
 *                                                                           wis::Result&                 out_result)
 * const noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::DX12VideoDecoder DX12VideoDecodingExtension::CreateDecoder(const wis::VideoDecoderDesc&
 * decoder_desc, wis::Result&                 out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateDecoder_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecodingExtension instance.
 * - `decoder_desc` Information about the video decoder to create.
 * - `video_decoder` Output parameter that holds the created video decoder handle if the operation is successful.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateDecoder_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateDecoder_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
