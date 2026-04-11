/**
 * @struct wisCommandListEnd
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListEnd_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisCommandListEnd(const WisCommandList* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKCommandListEnd(const WisVKCommandList* self);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12CommandListEnd(const WisDX12CommandList* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result CommandList::End() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKCommandList::End() const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12CommandList::End() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListEnd_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandListEnd_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListEnd_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
