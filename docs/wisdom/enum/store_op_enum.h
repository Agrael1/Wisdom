/**
 * @struct WisStoreOp WisStoreOp
 * @ingroup Enumerations Core
 *
 * @section WisStoreOp_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisStoreOp {
 *     WisStoreOpStore = 0,
 *     WisStoreOpDontCare = 1,
 * } WisStoreOp;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class StoreOp {
 *     Store = 0,
 *     DontCare = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStoreOp_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Attachment store operation for render pass.
 * 
 * \note Translates to  `D3D12_RENDER_PASS_ENDING_ACCESS_TYPE` for DirectX 12 implementation, and `VkAttachmentStoreOp` for Vulkan implementation.
 * 
 * Values:
 * - `WisStoreOpStore = 0`: Store the attachment contents.
 * - `WisStoreOpDontCare = 1`: Do not care about the attachment contents.
 * \endcond
 *
 *
 * @section WisStoreOp_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassRenderTargetDesc, WisRenderPassDepthStencilDesc
 * \endcond
 */
