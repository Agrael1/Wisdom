/**
 * @struct WisPrimitiveRestartValue WisPrimitiveRestartValue
 * @ingroup Enumerations Core
 *
 * @section WisPrimitiveRestartValue_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisPrimitiveRestartValue {
 *     WisPrimitiveRestartValueNone = 0,
 *     WisPrimitiveRestartValueUInt16Max = 1,
 *     WisPrimitiveRestartValueUInt32Max = 2,
 * } WisPrimitiveRestartValue;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class PrimitiveRestartValue {
 *     None = 0,
 *     UInt16Max = 1,
 *     UInt32Max = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPrimitiveRestartValue_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Primitive restart value for indexed drawing with primitive restart enabled.
 *
 * \note Translates to  `D3D12_INDEX_BUFFER_STRIP_CUT_VALUE` for DirectX 12 implementation.
 *
 * Values:
 * - `WisPrimitiveRestartValueNone = 0`: Primitive restart is disabled. No primitive restart value is used.
 * - `WisPrimitiveRestartValueUInt16Max = 1`: Use the maximum value of uint16_t as the primitive restart value.
 * - `WisPrimitiveRestartValueUInt32Max = 2`: Use the maximum value of uint32_t as the primitive restart value.
 * \endcond
 *
 *
 * @section WisPrimitiveRestartValue_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetPrimitiveRestartValue
 * \endcond
 */
