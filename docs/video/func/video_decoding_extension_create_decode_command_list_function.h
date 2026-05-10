/**
 * @struct wisVideoDecodingExtensionCreateDecodeCommandList
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodingExtensionCreateDecodeCommandList_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVideoDecodingExtensionCreateDecodeCommandList(WisVideoDecodingExtension* self,
 *                                                            const WisCommandAllocator* command_allocator,
 *                                                            WisVideoDecodeCommandList* command_list);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKVideoDecodingExtensionCreateDecodeCommandList(WisVKVideoDecodingExtension* self,
 *                                                              const WisVKCommandAllocator* command_allocator,
 *                                                              WisVKVideoDecodeCommandList* command_list);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12VideoDecodingExtensionCreateDecodeCommandList(WisDX12VideoDecodingExtension* self,
 *                                                                const WisDX12CommandAllocator* command_allocator,
 *                                                                WisDX12VideoDecodeCommandList* command_list);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis
 * ```
 * </details>
 *
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateDecodeCommandList_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisVideoDecodingExtension instance.
 * - `command_allocator` The command allocator that the command list will use for memory management of command buffers.
 * It @wis_must be created with the same WisDevice as the extension and have `WisCommandQueueTypeVideoDecode` or
 * `WisCommandQueueTypeVideoEncode` specified.
 * - `command_list` Output parameter that holds the created video command list handle if the operation is successful.
 *
 * - **return** denoting the outcome of operation.
 *
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateDecodeCommandList_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateDecodeCommandList_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
