/**
 * @struct wisCommandListBufferBarriers
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListBufferBarriers_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListBufferBarriers(const WisCommandList* self,
 *                                   const WisBufferBarrier* barriers,
 *                                   size_t                  barrier_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListBufferBarriers(const WisVKCommandList* self,
 *                                     const WisVKBufferBarrier* barriers,
 *                                     size_t                    barrier_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListBufferBarriers(const WisDX12CommandList* self,
 *                                       const WisDX12BufferBarrier* barriers,
 *                                       size_t                      barrier_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::BufferBarriers(wis::span<const wis::BufferBarrier> barriers) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::BufferBarriers(wis::span<const wis::VKBufferBarrier> barriers) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::BufferBarriers(wis::span<const wis::DX12BufferBarrier> barriers) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListBufferBarriers_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `barriers` points to an array of WisBufferBarrier barrier descriptions.
 * - `barrier_count` counts the number of barriers in the `barriers` array.
 * \endcond
 *
 * @section wisCommandListBufferBarriers_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListBufferBarriers_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
