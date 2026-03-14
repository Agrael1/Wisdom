/**
 * @struct wisCommandListRSSetViewports
 * @ingroup Functions
 *
 *
 * @section wisCommandListRSSetViewports_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListRSSetViewports(WisCommandList* self,
 *                                   const WisViewport* viewports,
 *                                   size_t             count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListRSSetViewports(WisVKCommandList* self,
 *                                     const WisViewport* viewports,
 *                                     size_t             count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListRSSetViewports(WisDX12CommandList* self,
 *                                       const WisViewport*  viewports,
 *                                       size_t              count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section wisCommandListRSSetViewports_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `viewports` The viewports to set.
 * - `count` The number of viewports to set.
 * 
 * \endcond
 *
 * @section wisCommandListRSSetViewports_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListRSSetViewports_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */