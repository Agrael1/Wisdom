/**
 * @struct WisStencilDesc
 * @ingroup Structures Core
 *
 *
 * @section WisStencilDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisStencilDesc {
 *     WisStencilOp fail_op;
 *     WisStencilOp depth_fail_op;
 *     WisStencilOp pass_op;
 *     WisCompareOp stencil_comp;
 *     uint8_t      read_mask;
 *     uint8_t      write_mask;
 * } WisStencilDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  StencilDesc {
 *     wis::StencilOp fail_op;
 *     wis::StencilOp depth_fail_op;
 *     wis::StencilOp pass_op;
 *     wis::CompareOp stencil_comp;
 *     std::uint8_t   read_mask;
 *     std::uint8_t   write_mask;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStencilDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `fail_op` specifies stencil operation if the stencil test fails. Default is `WisStencilOpKeep`.
 * - `depth_fail_op` specifies stencil operation if the stencil test passes and the depth test fails. Default is `WisStencilOpKeep`.
 * - `pass_op` describes stencil operation if the stencil test passes. Default is `WisStencilOpKeep`.
 * - `stencil_comp` defines stencil comparison function. Default is .
 * - `read_mask` indicates stencil read mask. Default is 0xff.
 * - `write_mask` specifies stencil write mask. Default is 0xff.
 * \endcond
 *
 * @section WisStencilDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStencilDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisDepthStencilDesc
 * \endcond
 */
