/**
 * @struct wisCommandAllocatorCreateCommandList
 * @ingroup Functions Core
 *
 *
 * @section wisCommandAllocatorCreateCommandList_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisCommandAllocatorCreateCommandList(const WisCommandAllocator* self,
 * WisCommandList*            list);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKCommandAllocatorCreateCommandList(const WisVKCommandAllocator* self,
 * WisVKCommandList*            list);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12CommandAllocatorCreateCommandList(const WisDX12CommandAllocator* self,
 * WisDX12CommandList*            list);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::CommandList CommandAllocator::CreateCommandList(wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKCommandList VKCommandAllocator::CreateCommandList(wis::Result& out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12CommandList DX12CommandAllocator::CreateCommandList(wis::Result& out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandAllocatorCreateCommandList_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandAllocator instance.
 * - `list` points to WisCommandList, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandAllocatorCreateCommandList_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandAllocatorCreateCommandList_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */