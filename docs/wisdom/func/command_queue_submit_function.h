/**
 * @struct wisCommandQueueSubmit
 * @ingroup Functions
 *
 *
 * @section wisCommandQueueSubmit_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisCommandQueueSubmit(const WisCommandQueue* self,
 *                                 const WisCommandListView* lists,
 *                                 size_t                    list_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKCommandQueueSubmit(const WisVKCommandQueue* self,
 *                                   const WisVKCommandListView* lists,
 *                                   size_t                      list_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12CommandQueueSubmit(const WisDX12CommandQueue* self,
 *                                     const WisDX12CommandListView* lists,
 *                                     size_t                        list_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result CommandQueue::Submit(wis::span<const wis::CommandListView> lists) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKCommandQueue::Submit(wis::span<const wis::VKCommandListView> lists) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12CommandQueue::Submit(wis::span<const wis::DX12CommandListView> lists) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandQueueSubmit_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandQueue instance.
 * - `lists` points to an array of command lists to execute.
 * - `list_count` defines number of command lists to execute.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandQueueSubmit_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandQueueSubmit_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */