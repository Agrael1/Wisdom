/**
 * @struct WisLineRasterization WisLineRasterization
 * @ingroup Enumerations Core
 *
 * @section WisLineRasterization_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisLineRasterization {
 *     WisLineRasterizationDefault = 0,
 *     WisLineRasterizationRectangular = 1,
 *     WisLineRasterizationAlphaAntialiased = 2,
 * } WisLineRasterization;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class LineRasterization {
 *     Default = 0,
 *     Rectangular = 1,
 *     AlphaAntialiased = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisLineRasterization_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Line rasterization mode.
 *
 * \note Translates to  `D3D12_LINE_RASTERIZATION_MODE` for DirectX 12 implementation, and `VkLineRasterizationModeEXT` for Vulkan implementation.
 *
 * Values:
 * - `WisLineRasterizationDefault = 0`: Line rasterization is disabled. Lines are not rendered.
 * - `WisLineRasterizationRectangular = 1`: Lines are rasterized as rectangles.
 * - `WisLineRasterizationAlphaAntialiased = 2`: Lines are rasterized as rectangles with anti-aliasing.
 * \endcond
 *
 *
 * @section WisLineRasterization_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRasterizerDesc
 * \endcond
 */
