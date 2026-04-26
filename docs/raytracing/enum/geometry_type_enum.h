/**
 * @struct WisGeometryType WisGeometryType
 * @ingroup Enumerations Raytracing
 *
 * @section WisGeometryType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisGeometryType {
 *     WisGeometryTypeTriangles = 0,
 *     WisGeometryTypeAABBs = 1,
 * } WisGeometryType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class GeometryType {
 *     Triangles = 0,
 *     AABBs = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisGeometryType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Enumeration for the type of geometry in a bottom-level acceleration structure.
 *
 * \note Translates to  `D3D12_RAYTRACING_GEOMETRY_TYPE` for DirectX 12 implementation, and `VkGeometryTypeKHR` for
 * Vulkan implementation.
 *
 * Values:
 * - `WisGeometryTypeTriangles = 0`: Triangles geometry type. Used for triangle meshes.
 * - `WisGeometryTypeAABBs = 1`: Axis Aligned Bounding Boxes geometry type. Used for bounding volume hierarchies.
 * \endcond
 *
 *
 * @section WisGeometryType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisAcceleratedGeometryDesc
 * \endcond
 */
