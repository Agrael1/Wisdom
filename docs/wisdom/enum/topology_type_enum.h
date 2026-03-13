/**
 * @struct WisTopologyType WisTopologyType
 * @ingroup Enumerations
 *
 * @section WisTopologyType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisTopologyType {
 *     WisTopologyTypePoint = 1,
 *     WisTopologyTypeLine = 2,
 *     WisTopologyTypeTriangle = 3,
 *     WisTopologyTypePatch = 4,
 * } WisTopologyType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class TopologyType {
 *     Point = 1,
 *     Line = 2,
 *     Triangle = 3,
 *     Patch = 4,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTopologyType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Primitive topology type for WisGraphicsPipelineDesc.
 * 
 * \note Translates to  `D3D12_PRIMITIVE_TOPOLOGY_TYPE` for DirectX 12 implementation, and `VkPrimitiveTopology` for Vulkan implementation.
 * 
 * Values:
 * - `WisTopologyTypePoint = 1`: Render points for each vertex.
 * - `WisTopologyTypeLine = 2`: Render lines between vertices.
 * - `WisTopologyTypeTriangle = 3`: Render triangles between vertices.
 * - `WisTopologyTypePatch = 4`: Vertices are interpret as patch list. Used in tesselation process.
 * \endcond
 *
 *
 * @section WisTopologyType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisGraphicsPipelineDesc
 * \endcond
 */