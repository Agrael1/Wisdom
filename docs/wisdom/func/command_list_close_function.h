/**
 * @struct wisCommandListClose
 * @ingroup Functions
 *
 *
 * @section wisCommandListClose_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisCommandListClose(const WisCommandList* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKCommandListClose(const WisVKCommandList* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12CommandListClose(const WisDX12CommandList* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result CommandList::Close() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKCommandList::Close() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12CommandList::Close() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListClose_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandListClose_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListClose_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */