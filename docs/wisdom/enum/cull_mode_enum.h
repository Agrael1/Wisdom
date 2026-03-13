/**
 * @struct WisCullMode WisCullMode
 * @ingroup Enumerations
 *
 * @section WisCullMode_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisCullMode {
 *     WisCullModeNone = 1,
 *     WisCullModeFront = 2,
 *     WisCullModeBack = 3,
 * } WisCullMode;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class CullMode {
 *     None = 1,
 *     Front = 2,
 *     Back = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCullMode_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Cull mode for rasterizer.
 * Triangle culling depends on wis::WindingOrder option.
 * 
 * \note Translates to  `D3D12_CULL_MODE` for DirectX 12 implementation, and `VkCullModeFlags` for Vulkan implementation.
 * 
 * Values:
 * - `WisCullModeNone = 1`: No culling.
 * - `WisCullModeFront = 2`: Cull front-facing triangles.
 * - `WisCullModeBack = 3`: Cull back-facing triangles.
 * \endcond
 *
 *
 * @section WisCullMode_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisRasterizerDesc
 * \endcond
 */