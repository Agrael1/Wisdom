/**
 * @struct wisInitVideoDecodingExtension
 * @ingroup Functions Video
 *
 *
 * @section wisInitVideoDecodingExtension_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisInitVideoDecodingExtension(WisVideoDecodingExtension* self,
 *                                    WisVideoCodecFlags         request_codecs);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVKInitVideoDecodingExtension(WisVKVideoDecodingExtension* self,
 *                                      WisVideoCodecFlags           request_codecs);
 *
 * // Provided by Wisdom 0.7.1.
 * void wisDX12InitVideoDecodingExtension(WisDX12VideoDecodingExtension* self,
 *                                        WisVideoCodecFlags             request_codecs);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * VideoDecodingExtension::VideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * VKVideoDecodingExtension::VKVideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * DX12VideoDecodingExtension::DX12VideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisInitVideoDecodingExtension_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` is a pointer to uninitialized WisVideoDecodingExtension instance memory. It will be initialized by
 * this function.
 * **note** The corresponding destroy function is `wisDestroyVideoDecodingExtension`.
 * - `request_codecs` Bitmask of requested video codecs. The extension will attempt to initialize with support for these
 * codecs.
 * \endcond
 *
 * @section wisInitVideoDecodingExtension_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisInitVideoDecodingExtension_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
