/**
 * @struct wisCommandListBufferReleaseBarriers
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListBufferReleaseBarriers_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListBufferReleaseBarriers(const WisCommandList* self,
 *                                          const WisBufferQueueOwnershipBarrier2* barriers,
 *                                          size_t                                 barrier_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListBufferReleaseBarriers(const WisVKCommandList* self,
 *                                            const WisVKBufferQueueOwnershipBarrier2* barriers,
 *                                            size_t                                   barrier_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListBufferReleaseBarriers(const WisDX12CommandList* self,
 *                                              const WisDX12BufferQueueOwnershipBarrier2* barriers,
 *                                              size_t                                     barrier_count);
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
 * @section wisCommandListBufferReleaseBarriers_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `barriers` points to an array of WisBufferQueueOwnershipBarrier2 release barriers.
 * - `barrier_count` counts the number of barriers in the `barriers` array.
 * 
 * \endcond
 *
 * @section wisCommandListBufferReleaseBarriers_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListBufferReleaseBarriers_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
