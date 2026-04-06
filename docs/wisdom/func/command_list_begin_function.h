/**
 * @struct wisCommandListBegin
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListBegin_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisCommandListBegin(const WisCommandList* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKCommandListBegin(const WisVKCommandList* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12CommandListBegin(const WisDX12CommandList* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result CommandList::Begin() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKCommandList::Begin() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12CommandList::Begin() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListBegin_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandListBegin_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListBegin_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */