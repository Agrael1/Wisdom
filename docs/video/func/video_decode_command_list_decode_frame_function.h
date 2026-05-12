/**
 * @struct wisVideoDecodeCommandListDecodeFrame
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodeCommandListDecodeFrame_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVideoDecodeCommandListDecodeFrame(const WisVideoDecodeCommandList* self,
 *                                           const WisVideoDecoder*           decoder,
 *                                           const WisVideoDecodeInputDesc*   input_desc);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKVideoDecodeCommandListDecodeFrame(const WisVKVideoDecodeCommandList* self,
 *                                             const WisVKVideoDecoder*           decoder,
 *                                             const WisVKVideoDecodeInputDesc*   input_desc);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12VideoDecodeCommandListDecodeFrame(const WisDX12VideoDecodeCommandList* self,
 *                                               const WisDX12VideoDecoder*           decoder,
 *                                               const WisDX12VideoDecodeInputDesc*   input_desc);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VideoDecodeCommandList::DecodeFrame(const wis::VideoDecoder&         decoder,
 *                                          const wis::VideoDecodeInputDesc& input_desc) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKVideoDecodeCommandList::DecodeFrame(const wis::VKVideoDecoder&         decoder,
 *                                            const wis::VKVideoDecodeInputDesc& input_desc) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12VideoDecodeCommandList::DecodeFrame(const wis::DX12VideoDecoder&         decoder,
 *                                              const wis::DX12VideoDecodeInputDesc& input_desc) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecodeCommandListDecodeFrame_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecodeCommandList instance.
 * - `decoder` The video decoder that will be used for decoding the video frame.
 * - `input_desc` Description of the input data for the video decode operation. This field specifies the type and
 * location of the input data that will be used for decoding the video frame.
 * \endcond
 *
 * @section wisVideoDecodeCommandListDecodeFrame_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodeCommandListDecodeFrame_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
