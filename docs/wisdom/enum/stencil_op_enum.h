/**
 * @struct WisStencilOp WisStencilOp
 * @ingroup Enumerations Core
 *
 * @section WisStencilOp_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisStencilOp {
 *     WisStencilOpKeep = 1,
 *     WisStencilOpZero = 2,
 *     WisStencilOpReplace = 3,
 *     WisStencilOpIncClamp = 4,
 *     WisStencilOpDecClamp = 5,
 *     WisStencilOpInvert = 6,
 *     WisStencilOpIncWrap = 7,
 *     WisStencilOpDecWrap = 8,
 * } WisStencilOp;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class StencilOp {
 *     Keep = 1,
 *     Zero = 2,
 *     Replace = 3,
 *     IncClamp = 4,
 *     DecClamp = 5,
 *     Invert = 6,
 *     IncWrap = 7,
 *     DecWrap = 8,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStencilOp_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Stencil operation for depth and stencil operations.
 *
 * \note Translates to  `D3D12_STENCIL_OP` for DirectX 12 implementation, and `VkStencilOp` for Vulkan implementation.
 *
 * Values:
 * - `WisStencilOpKeep = 1`: Keep the current value.
 * - `WisStencilOpZero = 2`: Set the value to zero.
 * - `WisStencilOpReplace = 3`: Replace the value with the reference value.
 * - `WisStencilOpIncClamp = 4`: Increment the value and clamp to the maximum value.
 * - `WisStencilOpDecClamp = 5`: Decrement the value and clamp to the minimum value.
 * - `WisStencilOpInvert = 6`: Invert the value.
 * - `WisStencilOpIncWrap = 7`: Increment the value and wrap to zero when the maximum value is exceeded.
 * - `WisStencilOpDecWrap = 8`: Decrement the value and wrap to the maximum value when the minimum value is exceeded.
 * \endcond
 *
 *
 * @section WisStencilOp_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStencilDesc
 * \endcond
 */