/**
 * @struct wisCommandListSetPrimitiveRestartValue
 * @ingroup Functions
 *
 *
 * @section wisCommandListSetPrimitiveRestartValue_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetPrimitiveRestartValue(WisCommandList* self,
 *                                             WisPrimitiveRestartValue restart_value);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetPrimitiveRestartValue(WisVKCommandList* self,
 *                                               WisPrimitiveRestartValue restart_value);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetPrimitiveRestartValue(WisDX12CommandList* self,
 *                                                 WisPrimitiveRestartValue restart_value);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetPrimitiveRestartValue(wis::PrimitiveRestartValue restart_value) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetPrimitiveRestartValue(wis::PrimitiveRestartValue restart_value) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetPrimitiveRestartValue(wis::PrimitiveRestartValue restart_value) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetPrimitiveRestartValue_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `restart_value` describes primitive restart value to set.
 * \endcond
 *
 * @section wisCommandListSetPrimitiveRestartValue_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetPrimitiveRestartValue_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */