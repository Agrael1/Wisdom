/**
 * @struct WisLoadOp WisLoadOp
 * @ingroup Enumerations
 *
 * @section WisLoadOp_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisLoadOp {
 *     WisLoadOpLoad = 0,
 *     WisLoadOpClear = 1,
 *     WisLoadOpDontCare = 2,
 * } WisLoadOp;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class LoadOp {
 *     Load = 0,
 *     Clear = 1,
 *     DontCare = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisLoadOp_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Attachment load operation for render pass.
 * 
 * \note Translates to  `D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE` for DirectX 12 implementation, and `VkAttachmentLoadOp` for Vulkan implementation.
 * 
 * Values:
 * - `WisLoadOpLoad = 0`: Load the attachment contents.
 * - `WisLoadOpClear = 1`: Clear the attachment contents.
 * - `WisLoadOpDontCare = 2`: Do not care about the attachment contents.
 * \endcond
 *
 *
 * @section WisLoadOp_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassRenderTargetDesc, WisRenderPassDepthStencilDesc
 * \endcond
 */