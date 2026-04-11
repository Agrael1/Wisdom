/**
 * @struct wisCommandListSetPushDescriptor
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetPushDescriptor_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListSetPushDescriptor(const WisCommandList* self,
 *                                      const WisPushDescriptorDataDesc* data);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListSetPushDescriptor(const WisVKCommandList* self,
 *                                        const WisPushDescriptorDataDesc* data);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListSetPushDescriptor(const WisDX12CommandList* self,
 *                                          const WisPushDescriptorDataDesc* data);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::SetPushDescriptor(const wis::PushDescriptorDataDesc& data) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::SetPushDescriptor(const wis::PushDescriptorDataDesc& data) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::SetPushDescriptor(const wis::PushDescriptorDataDesc& data) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetPushDescriptor_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `data` specifies a pointer to WisPushDescriptorDataDesc, which describes the push descriptors to set.
 * \endcond
 *
 * @section wisCommandListSetPushDescriptor_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetPushDescriptor_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
