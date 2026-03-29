/**
 * @struct WisRenderPassDepthStencilDesc
 * @ingroup Structures
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
 *     uint64_t             target;
 *     WisLoadOp            load_op_depth;
 *     WisLoadOp            load_op_stencil;
 *     WisStoreOp           store_op_depth;
 *     WisStoreOp           store_op_stencil;
 *     WisDepthStencilFlags flags;
 *     float                clear_depth;
 *     uint8_t              clear_stencil;
 * } WisRenderPassDepthStencilDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  RenderPassDepthStencilDesc {
 *     std::uint64_t          target;
 *     wis::LoadOp            load_op_depth;
 *     wis::LoadOp            load_op_stencil;
 *     wis::StoreOp           store_op_depth;
 *     wis::StoreOp           store_op_stencil;
 *     wis::DepthStencilFlags flags;
 *     float                  clear_depth;
 *     std::uint8_t           clear_stencil;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderPassDepthStencilDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `target` Depth stencil view.
 * - `load_op_depth` Load operation on beginning of render pass for depth. Default is `WisLoadOpLoad`.
 * - `load_op_stencil` Load operation on beginning of render pass for stencil. Default is `WisLoadOpLoad`.
 * - `store_op_depth` Store operation on end of render pass for depth. Default is `WisStoreOpStore`.
 * - `store_op_stencil` Store operation on end of render pass for stencil. Default is `WisStoreOpStore`.
 * - `flags` Depth stencil select.
 * - `clear_depth` Clear depth value for `WisLoadOpClear`. Default is 1.0f.
 * - `clear_stencil` Clear stencil value for `WisLoadOpClear`. Default is 0.
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