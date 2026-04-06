/**
 * @struct WisStencilOperation WisStencilOperation
 * @ingroup Enumerations Core
 *
 * @section WisStencilOperation_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisStencilOperation {
 *     WisStencilOperationKeep = 1,
 *     WisStencilOperationZero = 2,
 *     WisStencilOperationReplace = 3,
 *     WisStencilOperationIncClamp = 4,
 *     WisStencilOperationDecClamp = 5,
 *     WisStencilOperationInvert = 6,
 *     WisStencilOperationIncWrap = 7,
 *     WisStencilOperationDecWrap = 8,
 * } WisStencilOperation;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class StencilOperation {
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
 * @section WisStencilOperation_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Stencil operation for depth and stencil operations.
 *
 * \note Translates to  `D3D12_STENCIL_OP` for DirectX 12 implementation, and `VkStencilOp` for Vulkan implementation.
 *
 * Values:
 * - `WisStencilOperationKeep = 1`: Keep the current value.
 * - `WisStencilOperationZero = 2`: Set the value to zero.
 * - `WisStencilOperationReplace = 3`: Replace the value with the reference value.
 * - `WisStencilOperationIncClamp = 4`: Increment the value and clamp to the maximum value.
 * - `WisStencilOperationDecClamp = 5`: Decrement the value and clamp to the minimum value.
 * - `WisStencilOperationInvert = 6`: Invert the value.
 * - `WisStencilOperationIncWrap = 7`: Increment the value and wrap to zero when the maximum value is exceeded.
 * - `WisStencilOperationDecWrap = 8`: Decrement the value and wrap to the maximum value when the minimum value is
 * exceeded. \endcond
 *
 *
 * @section WisStencilOperation_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisStencilDesc, WisStencilDesc, WisStencilDesc
 * \endcond
 */
