/**
 * @struct WisAcceleratedGeometryDesc
 * @ingroup Structures Raytracing
 *
 *
 * @section WisAcceleratedGeometryDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisAcceleratedGeometryDesc {
 *     WisGeometryType  type;
 *     WisGeometryFlags flags;
 *     uint64_t         vertex_or_aabb_buffer_address;
 *     uint64_t         index_buffer_address;
 *     uint64_t         transform_matrix_address;
 *     uint32_t         vertex_or_aabb_stride;
 *     uint32_t         vertex_count;
 *     uint32_t         triangle_or_aabb_count;
 *     WisDataFormat    vertex_format;
 *     WisIndexType     index_format;
 * } WisAcceleratedGeometryDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  AcceleratedGeometryDesc {
 *     wis::GeometryType  type;
 *     wis::GeometryFlags flags;
 *     std::uint64_t      vertex_or_aabb_buffer_address;
 *     std::uint64_t      index_buffer_address;
 *     std::uint64_t      transform_matrix_address;
 *     std::uint32_t      vertex_or_aabb_stride;
 *     std::uint32_t      vertex_count;
 *     std::uint32_t      triangle_or_aabb_count;
 *     wis::DataFormat    vertex_format;
 *     wis::IndexType     index_format;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAcceleratedGeometryDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` The type of geometry (triangles or AABBs).
 * - `flags` The geometry flags for this geometry instance.
 * - `vertex_or_aabb_buffer_address` The GPU address of the vertex buffer for this geometry instance.
 * - `index_buffer_address` The GPU address of the index buffer for this geometry instance. Only used for triangles
 * geometry type.
 * - `transform_matrix_address` The GPU address of the transform matrix (float [3][4]) for this geometry instance. Only
 * used for triangles geometry type.
 * - `vertex_or_aabb_stride` The stride in bytes between vertices or AABBs in the buffer.
 * - `vertex_count` The number of vertices in the vertex buffer. Only used for triangles geometry type.
 * - `triangle_or_aabb_count` For triangles it is equal to (index_count/3) and count for AABBs.
 * - `vertex_format` The format of the vertex data in the vertex buffer. Only used for triangles geometry type.
 * - `index_format` The format of the index data in the index buffer. Only used for triangles geometry type.
 * \endcond
 *
 * @section WisAcceleratedGeometryDesc_descr Description
 * <hr>
 *
 * If the geometry type is `WisGeometryType::Triangles`, the geometry instance represents a triangle mesh.
 * If the `index_buffer_address` is 0, the geometry is non-indexed and `vertex_count` is used to determine the number of
 * triangles (vertex_count/3).
 *
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisAcceleratedGeometryDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBottomLevelStructureBuildDesc
 * \endcond
 */
