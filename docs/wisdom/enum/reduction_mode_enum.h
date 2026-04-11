/**
 * @struct WisReductionMode WisReductionMode
 * @ingroup Enumerations Core
 *
 * @section WisReductionMode_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisReductionMode {
 *     WisReductionModeStandard = 0,
 *     WisReductionModeMin = 1,
 *     WisReductionModeMax = 2,
 * } WisReductionMode;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class ReductionMode {
 *     Standard = 0,
 *     Min = 1,
 *     Max = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisReductionMode_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Reduction mode for texture sampling when min and mag filters are different.
 *
 * \note Translates to  `D3D12_FILTER_REDUCTION_TYPE` for DirectX 12 implementation, and `VkSamplerReductionModeEXT` for Vulkan implementation.
 *
 * Values:
 * - `WisReductionModeStandard = 0`: Standard filtering behavior.
 * - `WisReductionModeMin = 1`: Use the minimum value of the samples for filtering.
 * - `WisReductionModeMax = 2`: Use the maximum value of the samples for filtering.
 * \endcond
 *
 *
 * @section WisReductionMode_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSamplerDesc
 * \endcond
 */
