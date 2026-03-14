/**
 * @struct wisCommandListSetPrimitiveTopology
 * @ingroup Functions
 *
 *
 * @section wisCommandListSetPrimitiveTopology_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetPrimitiveTopology(WisCommandList* self,
 *                                         WisPrimitiveTopology topology);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetPrimitiveTopology(WisVKCommandList* self,
 *                                           WisPrimitiveTopology topology);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetPrimitiveTopology(WisDX12CommandList* self,
 *                                             WisPrimitiveTopology topology);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetPrimitiveTopology_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `topology` The primitive topology to set.
 * \endcond
 *
 * @section wisCommandListSetPrimitiveTopology_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetPrimitiveTopology_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */