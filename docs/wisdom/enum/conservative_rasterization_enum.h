/**
 * @struct WisConservativeRasterization WisConservativeRasterization
 * @ingroup Enumerations
 *
 * @section WisConservativeRasterization_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisConservativeRasterization {
 *     WisConservativeRasterizationOff = 0,
 *     WisConservativeRasterizationOverestimate = 1,
 * } WisConservativeRasterization;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class ConservativeRasterization {
 *     Off = 0,
 *     Overestimate = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisConservativeRasterization_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Conservative rasterization mode.
 * 
 * \note Translates to  `D3D12_CONSERVATIVE_RASTERIZATION_MODE` for DirectX 12 implementation, and `VkConservativeRasterizationModeEXT` for Vulkan implementation.
 * 
 * Values:
 * - `WisConservativeRasterizationOff = 0`: Conservative rasterization is disabled.
 * - `WisConservativeRasterizationOverestimate = 1`: Conservative rasterization is enabled.
 * \endcond
 *
 *
 * @section WisConservativeRasterization_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRasterizerDesc
 * \endcond
 */