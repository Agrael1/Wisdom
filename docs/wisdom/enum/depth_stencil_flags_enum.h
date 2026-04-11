/**
 * @struct WisDepthStencilFlags WisDepthStencilFlags
 * @ingroup Enumerations Core
 *
 * @section WisDepthStencilFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisDepthStencilFlags {
 *     WisDepthStencilFlagsNone = 0,
 *     WisDepthStencilFlagsIgnoreDepth = (1u << 0),
 *     WisDepthStencilFlagsIgnoreStencil = (1u << 1),
 *     WisDepthStencilFlagsReadOnlyDepth = (1u << 2),
 *     WisDepthStencilFlagsReadOnlyStencil = (1u << 3),
 * } WisDepthStencilFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class DepthStencilFlags : uint32_t {
 *     None = 0,
 *     IgnoreDepth = (1u << 0),
 *     IgnoreStencil = (1u << 1),
 *     ReadOnlyDepth = (1u << 2),
 *     ReadOnlyStencil = (1u << 3),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDepthStencilFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Depth stencil select flags.
 * Affect which part of the depth stencil buffer is used.
 *
 * Values:
 * - `WisDepthStencilFlagsNone = 0`: No flags set. Both attachments are write.
 * - `WisDepthStencilFlagsIgnoreDepth = (1 << 0)`: Don't use depth part of the depth stencil buffer.
 * - `WisDepthStencilFlagsIgnoreStencil = (1 << 1)`: Don't use stencil part of the depth stencil buffer.
 * - `WisDepthStencilFlagsReadOnlyDepth = (1 << 2)`: Depth part is read only. Texture @wis_must be in either read state,
 * depending on the format.
 * - `WisDepthStencilFlagsReadOnlyStencil = (1 << 3)`: Stencil part is read only. Texture @wis_must be in either read
 * state, depending on the format. \endcond
 *
 *
 * @section WisDepthStencilFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassDepthStencilDesc
 * \endcond
 */
