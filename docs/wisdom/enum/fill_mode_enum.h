/**
 * @struct WisFillMode WisFillMode
 * @ingroup Enumerations Core
 *
 * @section WisFillMode_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisFillMode {
 *     WisFillModeLines = 2,
 *     WisFillModeSolid = 3,
 * } WisFillMode;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class FillMode {
 *     Lines = 2,
 *     Solid = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisFillMode_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Fill mode for rasterizer.
 * 
 * \note Translates to  `D3D12_FILL_MODE` for DirectX 12 implementation, and `VkPolygonMode` for Vulkan implementation.
 * 
 * Values:
 * - `WisFillModeLines = 2`: Draw lines between vertices. Wireframe rendering.
 * - `WisFillModeSolid = 3`: Fill the area between vertices forming polygons.
 * \endcond
 *
 *
 * @section WisFillMode_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRasterizerDesc
 * \endcond
 */