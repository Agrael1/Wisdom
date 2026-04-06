/**
 * @struct WisRenderPassDepthStencilDesc
 * @ingroup Structures Core
 *
 *
 * @section WisRenderPassDepthStencilDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisRenderPassDepthStencilDesc {
 *     uint64_t              target;
 *     WisLoadOp             load_op_depth;
 *     WisLoadOp             load_op_stencil;
 *     WisStoreOp            store_op_depth;
 *     WisStoreOp            store_op_stencil;
 *     WisDepthStencilFlags  flags;
 *     float                 clear_depth;
 *     uint8_t               clear_stencil;
 *     const WisResolveDesc* resolve_depth_desc;
 *     const WisResolveDesc* resolve_stencil_desc;
 * } WisRenderPassDepthStencilDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  RenderPassDepthStencilDesc {
 *     std::uint64_t           target;
 *     wis::LoadOp             load_op_depth;
 *     wis::LoadOp             load_op_stencil;
 *     wis::StoreOp            store_op_depth;
 *     wis::StoreOp            store_op_stencil;
 *     wis::DepthStencilFlags  flags;
 *     float                   clear_depth;
 *     std::uint8_t            clear_stencil;
 *     const wis::ResolveDesc* resolve_depth_desc;
 *     const wis::ResolveDesc* resolve_stencil_desc;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderPassDepthStencilDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `target` indicates depth stencil view.
 * - `load_op_depth` specifies load operation on beginning of render pass for depth. Default is `WisLoadOpLoad`.
 * - `load_op_stencil` indicates load operation on beginning of render pass for stencil. Default is `WisLoadOpLoad`.
 * - `store_op_depth` indicates store operation on end of render pass for depth. Default is `WisStoreOpStore`.
 * - `store_op_stencil` defines store operation on end of render pass for stencil. Default is `WisStoreOpStore`.
 * - `flags` describes depth stencil select.
 * - `clear_depth` indicates clear depth value for `WisLoadOpClear`. Default is 1.0f.
 * - `clear_stencil` defines clear stencil value for `WisLoadOpClear`. Default is 0.
 * - `resolve_depth_desc` describes resolve parameters for multisampled depth.
 * - `resolve_stencil_desc` describes resolve parameters for multisampled stencil.
 * \endcond
 *
 * @section WisRenderPassDepthStencilDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRenderPassDepthStencilDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassDesc
 * \endcond
 */
