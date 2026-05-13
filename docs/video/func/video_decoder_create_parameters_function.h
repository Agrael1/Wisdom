/**
 * @struct wisVideoDecoderCreateParameters
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecoderCreateParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVideoDecoderCreateParameters(const WisVideoDecoder* self,
 *                                           const void*               sequence_parameters,
 *                                           WisVideoDecoderParameters*decoder_parameters);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKVideoDecoderCreateParameters(const WisVKVideoDecoder* self,
 *                                             const void*                 sequence_parameters,
 *                                             WisVKVideoDecoderParameters*decoder_parameters);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12VideoDecoderCreateParameters(const WisDX12VideoDecoder* self,
 *                                               const void*                   sequence_parameters,
 *                                               WisDX12VideoDecoderParameters*decoder_parameters);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecoderParameters VideoDecoder::CreateParameters(const void*  sequence_parameters,
 *                                                                          wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VKVideoDecoderParameters VKVideoDecoder::CreateParameters(const void*  sequence_parameters,
 *                                                                              wis::Result& out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::DX12VideoDecoderParameters DX12VideoDecoder::CreateParameters(const void*  sequence_parameters,
 *                                                                                  wis::Result& out_result) const
 * noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecoderCreateParameters_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecoder instance.
 * - `sequence_parameters` Pointer to the sequence header or parameter set data. The format and content of this data
 * @wis_should be appropriate for the codec being used, and @wis_should contain the necessary information to initialize
 * the video decoder parameters.
 * - `decoder_parameters` Output parameter that holds the created video decoder parameters handle if the operation is
 * successful.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisVideoDecoderCreateParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecoderCreateParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
