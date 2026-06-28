/**
 * @struct wisVideoDecodingExtensionCreateParameters
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodingExtensionCreateParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVideoDecodingExtensionCreateParameters(const WisVideoDecodingExtension* self,
 *                                                     const WisVideoDecoder*             decoder,
 *                                                     const WisVideoDecodeParameterDesc* params,
 *                                                     WisVideoDecoderParameters*         decoder_parameters);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKVideoDecodingExtensionCreateParameters(const WisVKVideoDecodingExtension* self,
 *                                                       const WisVKVideoDecoder*           decoder,
 *                                                       const WisVideoDecodeParameterDesc* params,
 *                                                       WisVKVideoDecoderParameters*       decoder_parameters);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12VideoDecodingExtensionCreateParameters(const WisDX12VideoDecodingExtension* self,
 *                                                         const WisDX12VideoDecoder*           decoder,
 *                                                         const WisVideoDecodeParameterDesc*   params,
 *                                                         WisDX12VideoDecoderParameters*       decoder_parameters);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecoderParameters VideoDecodingExtension::CreateParameters(const wis::VideoDecoder& decoder,
 *                                                                                    const
 * wis::VideoDecodeParameterDesc& params, wis::Result&                         out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VKVideoDecoderParameters VKVideoDecodingExtension::CreateParameters(const wis::VKVideoDecoder&
 * decoder, const wis::VideoDecodeParameterDesc& params, wis::Result&                         out_result) const
 * noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::DX12VideoDecoderParameters DX12VideoDecodingExtension::CreateParameters(const
 * wis::DX12VideoDecoder&         decoder, const wis::VideoDecodeParameterDesc& params, wis::Result& out_result) const
 * noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateParameters_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecodingExtension instance.
 * - `decoder` The video decoder that will use these parameters.
 * - `params` Codec-specific parameter data. Contains either AV1 or H.265 parameters depending on the codec field.
 * - `decoder_parameters` Output parameter that holds the created video decoder parameters handle if the operation is
 * successful.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
