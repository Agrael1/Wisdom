/**
 * @struct wisCommandListSetVertexBuffers
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetVertexBuffers_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetVertexBuffers(WisCommandList* self,
 *                                     const WisVertexBufferDesc* buffers,
 *                                     size_t                     buffer_count,
 *                                     uint32_t                   start_slot);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetVertexBuffers(WisVKCommandList* self,
 *                                       const WisVKVertexBufferDesc* buffers,
 *                                       size_t                       buffer_count,
 *                                       uint32_t                     start_slot);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetVertexBuffers(WisDX12CommandList* self,
 *                                         const WisDX12VertexBufferDesc* buffers,
 *                                         size_t                         buffer_count,
 *                                         uint32_t                       start_slot);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetVertexBuffers(const wis::VertexBufferDesc* buffers,
 *                                    std::size_t                  buffer_count,
 *                                    std::uint32_t                start_slot) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetVertexBuffers(const wis::VKVertexBufferDesc* buffers,
 *                                      std::size_t                    buffer_count,
 *                                      std::uint32_t                  start_slot) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetVertexBuffers(const wis::DX12VertexBufferDesc* buffers,
 *                                        std::size_t                      buffer_count,
 *                                        std::uint32_t                    start_slot) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetVertexBuffers_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `buffers` The vertex buffers to set.
 * - `buffer_count` The number of vertex buffers to set.
 * - `start_slot` The start slot to set the vertex buffers to. Default is 0.
 * \endcond
 *
 * @section wisCommandListSetVertexBuffers_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetVertexBuffers_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */