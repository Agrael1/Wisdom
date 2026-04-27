/**
 * @struct WisBottomLevelStructureBuildDesc
 * @ingroup Structures Raytracing
 *
 *
 * @section WisBottomLevelStructureBuildDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisBottomLevelStructureBuildDesc {
 *     WisAccelerationStructureFlags      flags;
 *     uint32_t                           geometry_count;
 *     const WisAcceleratedGeometryDesc*  geometries;
 *     const WisAcceleratedGeometryDesc** indirect_geometries;
 * } WisBottomLevelStructureBuildDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  BottomLevelStructureBuildDesc {
 *     wis::AccelerationStructureFlags      flags;
 *     std::uint32_t                        geometry_count;
 *     const wis::AcceleratedGeometryDesc*  geometries;
 *     const wis::AcceleratedGeometryDesc** indirect_geometries;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBottomLevelStructureBuildDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` The build flags for the acceleration structure build.
 * - `geometry_count` The number of geometry instances in the bottom-level acceleration structure.
 * - `geometries` The array of geometry descriptions for the bottom-level acceleration structure.
 * - `indirect_geometries` The array of geometry descriptions for indirect build of the bottom-level acceleration
 * structure. This input is ignored unless `WisAccelerationStructureFlagsIndirectInput` is specified.
 * \endcond
 *
 * @section WisBottomLevelStructureBuildDesc_descr Description
 * <hr>
 *
 * `geometries` and `indirect_geometries` are mutually exclusive. If both are provided, `geometries` will be used and
 * `indirect_geometries` will be ignored.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBottomLevelStructureBuildDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisRaytracingExtensionGetBottomLevelStructureInfo
 * \endcond
 */
