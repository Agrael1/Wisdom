/**
 * @struct wisCommandListEndRenderPass
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListEndRenderPass_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListEndRenderPass(const WisCommandList* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListEndRenderPass(const WisVKCommandList* self);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListEndRenderPass(const WisDX12CommandList* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::EndRenderPass() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::EndRenderPass() const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::EndRenderPass() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListEndRenderPass_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * \endcond
 *
 * @section wisCommandListEndRenderPass_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListEndRenderPass_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */