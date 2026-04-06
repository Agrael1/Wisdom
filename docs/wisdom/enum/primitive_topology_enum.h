/**
 * @struct WisPrimitiveTopology WisPrimitiveTopology
 * @ingroup Enumerations Core
 *
 * @section WisPrimitiveTopology_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisPrimitiveTopology {
 *     WisPrimitiveTopologyPointList = 1,
 *     WisPrimitiveTopologyLineList = 2,
 *     WisPrimitiveTopologyLineStrip = 3,
 *     WisPrimitiveTopologyTriangleList = 4,
 *     WisPrimitiveTopologyTriangleStrip = 5,
 *     WisPrimitiveTopologyTriangleFan = 6,
 *     WisPrimitiveTopologyLineListAdj = 10,
 *     WisPrimitiveTopologyLineStripAdj = 11,
 *     WisPrimitiveTopologyTriangleListAdj = 12,
 *     WisPrimitiveTopologyTriangleStripAdj = 13,
 * } WisPrimitiveTopology;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class PrimitiveTopology {
 *     PointList = 1,
 *     LineList = 2,
 *     LineStrip = 3,
 *     TriangleList = 4,
 *     TriangleStrip = 5,
 *     TriangleFan = 6,
 *     LineListAdj = 10,
 *     LineStripAdj = 11,
 *     TriangleListAdj = 12,
 *     TriangleStripAdj = 13,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPrimitiveTopology_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Primitive topology for rendering.
 * More info could be found [here](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies).
 * 
 * \note Translates to  `D3D_PRIMITIVE_TOPOLOGY` for DirectX 12 implementation, and `VkPrimitiveTopology` for Vulkan implementation.
 * 
 * Values:
 * - `WisPrimitiveTopologyPointList = 1`: Render points for each vertex.
 * - `WisPrimitiveTopologyLineList = 2`: Render lines between vertices.
 * - `WisPrimitiveTopologyLineStrip = 3`: Render lines between vertices in a strip.
 * - `WisPrimitiveTopologyTriangleList = 4`: Render triangles between vertices.
 * - `WisPrimitiveTopologyTriangleStrip = 5`: Render triangles between vertices in a strip.
 * - `WisPrimitiveTopologyTriangleFan = 6`: Interpret vertex data to form a fan of triangles.
 * - `WisPrimitiveTopologyLineListAdj = 10`: Render lines between vertices with adjacency.
 * - `WisPrimitiveTopologyLineStripAdj = 11`: Render lines between vertices in a strip with adjacency.
 * - `WisPrimitiveTopologyTriangleListAdj = 12`: Render triangles between vertices with adjacency.
 * - `WisPrimitiveTopologyTriangleStripAdj = 13`: Render triangles between vertices in a strip with adjacency.
 * \endcond
 *
 *
 * @section WisPrimitiveTopology_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetPrimitiveTopology
 * \endcond
 */