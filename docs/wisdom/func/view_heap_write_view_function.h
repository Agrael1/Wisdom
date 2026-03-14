/**
 * @struct wisViewHeapWriteView
 * @ingroup Functions
 *
 *
 * @section wisViewHeapWriteView_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisViewHeapWriteView(const WisViewHeap* self,
 *                           const WisRenderTargetDesc* render_target);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKViewHeapWriteView(const WisVKViewHeap* self,
 *                             const WisRenderTargetDesc* render_target);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12ViewHeapWriteView(const WisDX12ViewHeap* self,
 *                               const WisRenderTargetDesc* render_target);
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
 * @section wisViewHeapWriteView_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * - `render_target` points to WisRenderTargetDesc, which describes the render target view to write.
 * 
 * \endcond
 *
 * @section wisViewHeapWriteView_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapWriteView_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */