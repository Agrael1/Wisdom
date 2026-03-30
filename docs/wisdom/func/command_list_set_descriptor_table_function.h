/**
 * @struct wisCommandListSetDescriptorTable
 * @ingroup Functions
 *
 *
 * @section wisCommandListSetDescriptorTable_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetDescriptorTable(const WisCommandList* self,
 *                                       const WisDescriptorTableDataDesc* data);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetDescriptorTable(const WisVKCommandList* self,
 *                                         const WisDescriptorTableDataDesc* data);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetDescriptorTable(const WisDX12CommandList* self,
 *                                           const WisDescriptorTableDataDesc* data);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetDescriptorTable(const wis::DescriptorTableDataDesc& data) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetDescriptorTable(const wis::DescriptorTableDataDesc& data) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetDescriptorTable(const wis::DescriptorTableDataDesc& data) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetDescriptorTable_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `data` specifies the root parameter index to set the descriptor table for.
 * \endcond
 *
 * @section wisCommandListSetDescriptorTable_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetDescriptorTable_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */