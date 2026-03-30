/**
 * @struct WisRenderPassRenderTargetDesc
 * @ingroup Structures
 *
 *
 * @section WisRenderPassRenderTargetDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisRenderPassRenderTargetDesc {
 *     uint64_t   target;
 *     WisLoadOp  load_op;
 *     WisStoreOp store_op;
 *     float      clear_value[4];
 * } WisRenderPassRenderTargetDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  RenderPassRenderTargetDesc {
 *     std::uint64_t target;
 *     wis::LoadOp   load_op;
 *     wis::StoreOp  store_op;
 *     std::array<float, 4> clear_value;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderPassRenderTargetDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `target` indicates render target view; it @wis_must reside on WisViewHeap.
 * - `load_op` specifies load operation on beginning of render pass. Default is `WisLoadOpLoad`.
 * - `store_op` indicates store operation on end of render pass. Default is `WisStoreOpStore`.
 * - `clear_value` indicates clear value for `WisLoadOpClear`.
 * \endcond
 *
 * @section WisRenderPassRenderTargetDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRenderPassRenderTargetDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassDesc
 * \endcond
 */