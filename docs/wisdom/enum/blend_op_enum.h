/**
 * @struct WisBlendOp WisBlendOp
 * @ingroup Enumerations
 *
 * @section WisBlendOp_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisBlendOp {
 *     WisBlendOpAdd = 1,
 *     WisBlendOpSubtract = 2,
 *     WisBlendOpRevSubtract = 3,
 *     WisBlendOpMin = 4,
 *     WisBlendOpMax = 5,
 * } WisBlendOp;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class BlendOp {
 *     Add = 1,
 *     Subtract = 2,
 *     RevSubtract = 3,
 *     Min = 4,
 *     Max = 5,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBlendOp_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Blend operation for color blending operations.
 * 
 * \note Translates to  `D3D12_BLEND_OP` for DirectX 12 implementation, and `VkBlendOp` for Vulkan implementation.
 * 
 * Values:
 * - `WisBlendOpAdd = 1`: Add the source and destination colors.
 * - `WisBlendOpSubtract = 2`: Subtract the source color from the destination color.
 * - `WisBlendOpRevSubtract = 3`: Subtract the destination color from the source color.
 * - `WisBlendOpMin = 4`: Use the minimum of the source and destination colors.
 * - `WisBlendOpMax = 5`: Use the maximum of the source and destination colors.
 * \endcond
 *
 *
 * @section WisBlendOp_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisBlendAttachmentDesc, WisBlendAttachmentDesc
 * \endcond
 */