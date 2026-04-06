/**
 * @struct WisLogicOp WisLogicOp
 * @ingroup Enumerations Core
 *
 * @section WisLogicOp_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisLogicOp {
 *     WisLogicOpClear = 0,
 *     WisLogicOpSet = 1,
 *     WisLogicOpCopy = 2,
 *     WisLogicOpCopyInverted = 3,
 *     WisLogicOpNoop = 4,
 *     WisLogicOpInvert = 5,
 *     WisLogicOpAnd = 6,
 *     WisLogicOpNand = 7,
 *     WisLogicOpOr = 8,
 *     WisLogicOpNor = 9,
 *     WisLogicOpXor = 10,
 *     WisLogicOpEquiv = 11,
 *     WisLogicOpAndReverse = 12,
 *     WisLogicOpAndInverted = 13,
 *     WisLogicOpOrReverse = 14,
 *     WisLogicOpOrInverted = 15,
 * } WisLogicOp;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class LogicOp {
 *     Clear = 0,
 *     Set = 1,
 *     Copy = 2,
 *     CopyInverted = 3,
 *     Noop = 4,
 *     Invert = 5,
 *     And = 6,
 *     Nand = 7,
 *     Or = 8,
 *     Nor = 9,
 *     Xor = 10,
 *     Equiv = 11,
 *     AndReverse = 12,
 *     AndInverted = 13,
 *     OrReverse = 14,
 *     OrInverted = 15,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisLogicOp_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Logic operation for color blending operations.
 *
 * \note Translates to  `D3D12_LOGIC_OP` for DirectX 12 implementation, and `VkLogicOp` for Vulkan implementation.
 *
 * Values:
 * - `WisLogicOpClear = 0`: Clear the destination value.
 * - `WisLogicOpSet = 1`: Set the destination value.
 * - `WisLogicOpCopy = 2`: Copy the source value to the destination.
 * - `WisLogicOpCopyInverted = 3`: Copy the inverted source value to the destination.
 * - `WisLogicOpNoop = 4`: Do not modify the destination value.
 * - `WisLogicOpInvert = 5`: Invert the destination value.
 * - `WisLogicOpAnd = 6`: Perform a bitwise AND operation on the source and destination values.
 * - `WisLogicOpNand = 7`: Perform a bitwise NAND operation on the source and destination values.
 * - `WisLogicOpOr = 8`: Perform a bitwise OR operation on the source and destination values.
 * - `WisLogicOpNor = 9`: Perform a bitwise NOR operation on the source and destination values.
 * - `WisLogicOpXor = 10`: Perform a bitwise XOR operation on the source and destination values.
 * - `WisLogicOpEquiv = 11`: Perform a bitwise equivalent operation on the source and destination values.
 * - `WisLogicOpAndReverse = 12`: Perform a bitwise AND operation on the source and inverted destination values.
 * - `WisLogicOpAndInverted = 13`: Perform a bitwise AND operation on the inverted source and destination values.
 * - `WisLogicOpOrReverse = 14`: Perform a bitwise OR operation on the source and inverted destination values.
 * - `WisLogicOpOrInverted = 15`: Perform a bitwise OR operation on the inverted source and destination values.
 * \endcond
 *
 *
 * @section WisLogicOp_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBlendStateDesc
 * \endcond
 */
