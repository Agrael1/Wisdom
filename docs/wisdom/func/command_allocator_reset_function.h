/**
 * @struct wisCommandAllocatorReset
 * @ingroup Functions Core
 *
 *
 * @section wisCommandAllocatorReset_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisCommandAllocatorReset(const WisCommandAllocator* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKCommandAllocatorReset(const WisVKCommandAllocator* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12CommandAllocatorReset(const WisDX12CommandAllocator* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result CommandAllocator::Reset() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKCommandAllocator::Reset() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12CommandAllocator::Reset() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandAllocatorReset_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandAllocator instance.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandAllocatorReset_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandAllocatorReset_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */