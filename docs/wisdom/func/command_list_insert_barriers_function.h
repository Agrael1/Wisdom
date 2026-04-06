/**
 * @struct wisCommandListInsertBarriers
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListInsertBarriers_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListInsertBarriers(const WisCommandList* self,
 *                                   const WisBarrierGroup* barriers);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListInsertBarriers(const WisVKCommandList* self,
 *                                     const WisVKBarrierGroup* barriers);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListInsertBarriers(const WisDX12CommandList* self,
 *                                       const WisDX12BarrierGroup* barriers);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::InsertBarriers(const wis::BarrierGroup& barriers) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::InsertBarriers(const wis::VKBarrierGroup& barriers) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::InsertBarriers(const wis::DX12BarrierGroup& barriers) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListInsertBarriers_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `barriers` specifies a pointer to an array of barriers to insert.
 * \endcond
 *
 * @section wisCommandListInsertBarriers_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListInsertBarriers_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */