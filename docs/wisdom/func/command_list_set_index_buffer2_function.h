/**
 * @struct wisCommandListSetIndexBuffer2
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetIndexBuffer2_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListSetIndexBuffer2(WisCommandList* self,
 *                                    const WisIndexBufferAddressDesc* buffer,
 *                                    WisIndexType                     index_type);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListSetIndexBuffer2(WisVKCommandList* self,
 *                                      const WisIndexBufferAddressDesc* buffer,
 *                                      WisIndexType                     index_type);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListSetIndexBuffer2(WisDX12CommandList* self,
 *                                        const WisIndexBufferAddressDesc* buffer,
 *                                        WisIndexType                     index_type);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::SetIndexBuffer2(const wis::IndexBufferAddressDesc* buffer,
 *                                   wis::IndexType                     index_type) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::SetIndexBuffer2(const wis::IndexBufferAddressDesc* buffer,
 *                                     wis::IndexType                     index_type) noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::SetIndexBuffer2(const wis::IndexBufferAddressDesc* buffer,
 *                                       wis::IndexType                     index_type) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetIndexBuffer2_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `buffer` The index buffer to set.
 * - `index_type` Defines index type. Used to determine the size of each index in the buffer. Must be either `WisIndexTypeUInt16` or `WisIndexTypeUInt32`.
 * \endcond
 *
 * @section wisCommandListSetIndexBuffer2_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetIndexBuffer2_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */