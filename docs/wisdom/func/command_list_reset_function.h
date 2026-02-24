/**
 * @struct wisCommandListReset
 * @ingroup Functions
 *
 *
 * @section wisCommandListReset_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisCommandListReset(const WisCommandList* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKCommandListReset(const WisVKCommandList* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12CommandListReset(const WisDX12CommandList* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result CommandList::Reset() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKCommandList::Reset() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12CommandList::Reset() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListReset_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandListReset_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListReset_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */