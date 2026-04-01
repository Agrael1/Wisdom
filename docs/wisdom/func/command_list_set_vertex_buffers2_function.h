/**
 * @struct wisCommandListSetVertexBuffers2
 * @ingroup Functions
 *
 *
 * @section wisCommandListSetVertexBuffers2_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetVertexBuffers2(WisCommandList* self,
 *                                      const WisVertexBufferAddressDesc* buffers,
 *                                      size_t                            buffer_count,
 *                                      uint32_t                          start_slot);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetVertexBuffers2(WisVKCommandList* self,
 *                                        const WisVertexBufferAddressDesc* buffers,
 *                                        size_t                            buffer_count,
 *                                        uint32_t                          start_slot);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetVertexBuffers2(WisDX12CommandList* self,
 *                                          const WisVertexBufferAddressDesc* buffers,
 *                                          size_t                            buffer_count,
 *                                          uint32_t                          start_slot);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetVertexBuffers2(wis::span<const wis::VertexBufferAddressDesc> buffers,
 *                                     std::uint32_t                                 start_slot) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetVertexBuffers2(wis::span<const wis::VertexBufferAddressDesc> buffers,
 *                                       std::uint32_t                                 start_slot) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetVertexBuffers2(wis::span<const wis::VertexBufferAddressDesc> buffers,
 *                                         std::uint32_t                                 start_slot) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetVertexBuffers2_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `buffers` The vertex buffers to set.
 * - `buffer_count` The number of vertex buffers to set.
 * - `start_slot` The start slot to set the vertex buffers to. Default is 0.
 * \endcond
 *
 * @section wisCommandListSetVertexBuffers2_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetVertexBuffers2_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */