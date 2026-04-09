/**
 * @struct WisFilter WisFilter
 * @ingroup Enumerations Core
 *
 * @section WisFilter_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisFilter {
 *     WisFilterPoint = 0,
 *     WisFilterLinear = 1,
 * } WisFilter;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class Filter {
 *     Point = 0,
 *     Linear = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisFilter_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Filtering mode for texture sampling.
 * 
 * \note Translates to  `D3D12_FILTER_TYPE` for DirectX 12 implementation, and `VkFilter` for Vulkan implementation.
 * 
 * Values:
 * - `WisFilterPoint = 0`: Nearest neighbor filtering.
 * - `WisFilterLinear = 1`: Linear filtering.
 * \endcond
 *
 *
 * @section WisFilter_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSamplerDesc
 * \endcond
 */
