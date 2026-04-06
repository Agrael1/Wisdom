/**
 * @struct WisDepthStencilDesc
 * @ingroup Structures Core
 *
 *
 * @section WisDepthStencilDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDepthStencilDesc {
 *     bool           depth_enable;
 *     bool           depth_write_enable;
 *     WisCompareOp   depth_comp;
 *     bool           stencil_enable;
 *     WisStencilDesc stencil_front;
 *     WisStencilDesc stencil_back;
 *     bool           depth_bound_test;
 * } WisDepthStencilDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DepthStencilDesc {
 *     bool             depth_enable;
 *     bool             depth_write_enable;
 *     wis::CompareOp   depth_comp;
 *     bool             stencil_enable;
 *     wis::StencilDesc stencil_front;
 *     wis::StencilDesc stencil_back;
 *     bool             depth_bound_test;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDepthStencilDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `depth_enable` indicates depth test enable. Default is false.
 * - `depth_write_enable` describes depth write enable. Default is false.
 * - `depth_comp` indicates depth comparison function. Default is .
 * - `stencil_enable` defines stencil test enable. Default is false.
 * - `stencil_front` specifies stencil description for front faces.
 * - `stencil_back` specifies stencil description for back faces.
 * - `depth_bound_test` indicates depth bound test enable. Default is false.
 * \endcond
 *
 * @section WisDepthStencilDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDepthStencilDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisGraphicsPipelineDesc
 * \endcond
 */