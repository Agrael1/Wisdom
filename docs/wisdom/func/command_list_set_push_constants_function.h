/**
 * @struct wisCommandListSetPushConstants
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetPushConstants_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListSetPushConstants(const WisCommandList* self,
 *                                     const WisPushConstantDataDesc* data);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListSetPushConstants(const WisVKCommandList* self,
 *                                       const WisPushConstantDataDesc* data);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListSetPushConstants(const WisDX12CommandList* self,
 *                                         const WisPushConstantDataDesc* data);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::SetPushConstants(const wis::PushConstantDataDesc& data) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::SetPushConstants(const wis::PushConstantDataDesc& data) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::SetPushConstants(const wis::PushConstantDataDesc& data) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetPushConstants_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `data` specifies a pointer to , which describes the push constant data to set.
 * \endcond
 *
 * @section wisCommandListSetPushConstants_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetPushConstants_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */