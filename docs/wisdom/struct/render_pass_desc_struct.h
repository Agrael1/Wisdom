/**
 * @struct WisRenderPassDesc
 * @ingroup Structures Core
 *
 *
 * @section WisRenderPassDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisRenderPassDesc {
 *     WisRenderPassFlags            flags;
 *     WisRenderPassRenderTargetDesc render_targets[8];
 *     uint32_t                      render_target_count;
 *     uint32_t                      view_mask;
 *     WisRenderPassDepthStencilDesc depth_stencil;
 * } WisRenderPassDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  RenderPassDesc {
 *     wis::RenderPassFlags            flags;
 *     std::array<wis::RenderPassRenderTargetDesc, 8> render_targets;
 *     std::uint32_t                   render_target_count;
 *     std::uint32_t                   view_mask;
 *     wis::RenderPassDepthStencilDesc depth_stencil;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderPassDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` describes render pass flags.
 * - `render_targets` describes an array of render target descriptions. Max size is 8.
 * - `render_target_count` specifies number of render targets in the render pass.
 * - `view_mask` specifies view mask for multiview rendering. Each bit represents a view that can be rendered to with the render pass. Default is 0, meaning no multiview support.
 * - `depth_stencil` specifies depth stencil description; if depth stencil is not used, the target field @wis_must be set to 0.
 * \endcond
 *
 * @section WisRenderPassDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRenderPassDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListBeginRenderPass
 * \endcond
 */