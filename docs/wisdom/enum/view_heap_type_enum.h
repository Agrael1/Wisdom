/**
 * @struct WisViewHeapType WisViewHeapType
 * @ingroup Enumerations
 *
 * @section WisViewHeapType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisViewHeapType {
 *     WisViewHeapTypeRenderTarget = 0,
 *     WisViewHeapTypeDepthStencil = 1,
 * } WisViewHeapType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class ViewHeapType {
 *     RenderTarget = 0,
 *     DepthStencil = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisViewHeapType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Type of the view descriptor heap.
 * 
 * \note Translates to  `D3D12_DESCRIPTOR_HEAP_TYPE` for DirectX 12 implementation.
 * 
 * Values:
 * - `WisViewHeapTypeRenderTarget = 0`: Descriptor heap for render target views.
 * - `WisViewHeapTypeDepthStencil = 1`: Descriptor heap for depth stencil views.
 * \endcond
 *
 *
 * @section WisViewHeapType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see wisDeviceCreateViewHeap
 * \endcond
 */