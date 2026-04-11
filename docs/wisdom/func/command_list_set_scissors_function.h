/**
 * @struct wisCommandListSetScissors
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetScissors_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetScissors(WisCommandList* self,
 *                                const WisRect*  scissor_rect,
 *                                size_t          rect_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetScissors(WisVKCommandList* self,
 *                                  const WisRect*    scissor_rect,
 *                                  size_t            rect_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetScissors(WisDX12CommandList* self,
 *                                    const WisRect*      scissor_rect,
 *                                    size_t              rect_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetScissors(wis::span<const wis::Rect> scissor_rect) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetScissors(wis::span<const wis::Rect> scissor_rect) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetScissors(wis::span<const wis::Rect> scissor_rect) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetScissors_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `scissor_rect` points to an array of scissor rectangles to set.
 * - `rect_count` defines number of scissor rectangles to set.
 * \endcond
 *
 * @section wisCommandListSetScissors_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetScissors_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
