/**
 * @struct wisCommandListSetViewports
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetViewports_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListSetViewports(WisCommandList* self,
 *                                 const WisViewport* viewports,
 *                                 size_t             viewport_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListSetViewports(WisVKCommandList* self,
 *                                   const WisViewport* viewports,
 *                                   size_t             viewport_count);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListSetViewports(WisDX12CommandList* self,
 *                                     const WisViewport*  viewports,
 *                                     size_t              viewport_count);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::SetViewports(wis::span<const wis::Viewport> viewports) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::SetViewports(wis::span<const wis::Viewport> viewports) noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::SetViewports(wis::span<const wis::Viewport> viewports) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetViewports_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `viewports` points to an array of viewports to set.
 * - `viewport_count` defines number of viewports to set.
 * \endcond
 *
 * @section wisCommandListSetViewports_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetViewports_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
