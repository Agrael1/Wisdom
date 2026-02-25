/**
 * @struct wisCommandListBindDescriptorHeaps
 * @ingroup Functions
 *
 *
 * @section wisCommandListBindDescriptorHeaps_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListBindDescriptorHeaps(const WisCommandList* self,
 *                                        const WisDescriptorHeap* resource_heap,
 *                                        const WisDescriptorHeap* sampler_heap);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListBindDescriptorHeaps(const WisVKCommandList* self,
 *                                          const WisVKDescriptorHeap* resource_heap,
 *                                          const WisVKDescriptorHeap* sampler_heap);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListBindDescriptorHeaps(const WisDX12CommandList* self,
 *                                            const WisDX12DescriptorHeap* resource_heap,
 *                                            const WisDX12DescriptorHeap* sampler_heap);
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
 * @section wisCommandListBindDescriptorHeaps_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `resource_heap` points to WisDescriptorHeap with shader resource views, unordered access views and constant buffer views. If `nullptr`, no resource heap is bound.
 * - `sampler_heap` points to WisDescriptorHeap with samplers. If `nullptr`, no sampler heap is bound.
 * 
 * \endcond
 *
 * @section wisCommandListBindDescriptorHeaps_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListBindDescriptorHeaps_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */