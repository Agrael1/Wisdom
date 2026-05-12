/**
 * @struct wisVideoDecodingExtensionCreateCommandList
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodingExtensionCreateCommandList_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVideoDecodingExtensionCreateCommandList(WisVideoDecodingExtension* self,
 *                                                      const WisCommandAllocator* command_allocator,
 *                                                      WisVideoDecodeCommandList* command_list);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKVideoDecodingExtensionCreateCommandList(WisVKVideoDecodingExtension* self,
 *                                                        const WisVKCommandAllocator* command_allocator,
 *                                                        WisVKVideoDecodeCommandList* command_list);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12VideoDecodingExtensionCreateCommandList(WisDX12VideoDecodingExtension* self,
 *                                                          const WisDX12CommandAllocator* command_allocator,
 *                                                          WisDX12VideoDecodeCommandList* command_list);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VideoDecodeCommandList VideoDecodingExtension::CreateCommandList(const wis::CommandAllocator&
 * command_allocator, wis::Result&                 out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VKVideoDecodeCommandList VKVideoDecodingExtension::CreateCommandList(const
 * wis::VKCommandAllocator& command_allocator, wis::Result&                   out_result) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::DX12VideoDecodeCommandList DX12VideoDecodingExtension::CreateCommandList(const
 * wis::DX12CommandAllocator& command_allocator, wis::Result&                     out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateCommandList_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecodingExtension instance.
 * - `command_allocator` The command allocator that the command list will use for memory management of command buffers.
 * It @wis_must be created with the same WisDevice as the extension and have `WisCommandQueueTypeVideoDecode` or
 * `WisCommandQueueTypeVideoEncode` specified.
 * - `command_list` Output parameter that holds the created video command list handle if the operation is successful.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateCommandList_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodingExtensionCreateCommandList_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
