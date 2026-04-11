/**
 * @struct wisCommandListBeginRenderPass
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListBeginRenderPass_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListBeginRenderPass(const WisCommandList* self,
 *                                    const WisRenderPassDesc* desc);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListBeginRenderPass(const WisVKCommandList* self,
 *                                      const WisRenderPassDesc* desc);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListBeginRenderPass(const WisDX12CommandList* self,
 *                                        const WisRenderPassDesc*  desc);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::BeginRenderPass(const wis::RenderPassDesc& desc) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::BeginRenderPass(const wis::RenderPassDesc& desc) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::BeginRenderPass(const wis::RenderPassDesc& desc) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListBeginRenderPass_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `desc` indicates a pointer to WisRenderPassDesc, which describes the render pass to begin.
 * \endcond
 *
 * @section wisCommandListBeginRenderPass_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListBeginRenderPass_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
