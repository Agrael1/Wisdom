/**
 * @struct WisCompareOperation WisCompareOperation
 * @ingroup Enumerations Core
 *
 * @section WisCompareOperation_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisCompareOperation {
 *     WisCompareOperationNone = 0,
 *     WisCompareOperationNever = 1,
 *     WisCompareOperationLess = 2,
 *     WisCompareOperationEqual = 3,
 *     WisCompareOperationLessEqual = 4,
 *     WisCompareOperationGreater = 5,
 *     WisCompareOperationNotEqual = 6,
 *     WisCompareOperationGreaterEqual = 7,
 *     WisCompareOperationAlways = 8,
 * } WisCompareOperation;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class CompareOperation {
 *     None = 0,
 *     Never = 1,
 *     Less = 2,
 *     Equal = 3,
 *     LessEqual = 4,
 *     Greater = 5,
 *     NotEqual = 6,
 *     GreaterEqual = 7,
 *     Always = 8,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCompareOperation_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Comparison function for depth and stencil operations.
 *
 * \note Translates to  `D3D12_COMPARISON_FUNC` for DirectX 12 implementation, and `VkCompareOp` for Vulkan implementation.
 *
 * Values:
 * - `WisCompareOperationNone = 0`: No comparison.
 * - `WisCompareOperationNever = 1`: Always fail the comparison.
 * - `WisCompareOperationLess = 2`: Pass the comparison if the source value is less than the destination value.
 * - `WisCompareOperationEqual = 3`: Pass the comparison if the source value is equal to the destination value.
 * - `WisCompareOperationLessEqual = 4`: Pass the comparison if the source value is less than or equal to the destination value.
 * - `WisCompareOperationGreater = 5`: Pass the comparison if the source value is greater than the destination value.
 * - `WisCompareOperationNotEqual = 6`: Pass the comparison if the source value is not equal to the destination value.
 * - `WisCompareOperationGreaterEqual = 7`: Pass the comparison if the source value is greater than or equal to the destination value.
 * - `WisCompareOperationAlways = 8`: Always pass the comparison.
 * \endcond
 *
 *
 * @section WisCompareOperation_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisSamplerDesc, WisStencilDesc, WisDepthStencilDesc
 * \endcond
 */
