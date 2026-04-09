/**
 * @struct WisCompareOp WisCompareOp
 * @ingroup Enumerations Core
 *
 * @section WisCompareOp_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisCompareOp {
 *     WisCompareOpNone = 0,
 *     WisCompareOpNever = 1,
 *     WisCompareOpLess = 2,
 *     WisCompareOpEqual = 3,
 *     WisCompareOpLessEqual = 4,
 *     WisCompareOpGreater = 5,
 *     WisCompareOpNotEqual = 6,
 *     WisCompareOpGreaterEqual = 7,
 *     WisCompareOpAlways = 8,
 * } WisCompareOp;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class CompareOp {
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
 * @section WisCompareOp_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Comparison function for depth and stencil operations.
 * 
 * \note Translates to  `D3D12_COMPARISON_FUNC` for DirectX 12 implementation, and `VkCompareOp` for Vulkan implementation.
 * 
 * Values:
 * - `WisCompareOpNone = 0`: No comparison.
 * - `WisCompareOpNever = 1`: Always fail the comparison.
 * - `WisCompareOpLess = 2`: Pass the comparison if the source value is less than the destination value.
 * - `WisCompareOpEqual = 3`: Pass the comparison if the source value is equal to the destination value.
 * - `WisCompareOpLessEqual = 4`: Pass the comparison if the source value is less than or equal to the destination value.
 * - `WisCompareOpGreater = 5`: Pass the comparison if the source value is greater than the destination value.
 * - `WisCompareOpNotEqual = 6`: Pass the comparison if the source value is not equal to the destination value.
 * - `WisCompareOpGreaterEqual = 7`: Pass the comparison if the source value is greater than or equal to the destination value.
 * - `WisCompareOpAlways = 8`: Always pass the comparison.
 * \endcond
 *
 *
 * @section WisCompareOp_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSamplerDesc, WisStencilDesc, WisDepthStencilDesc
 * \endcond
 */
