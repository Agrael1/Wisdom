/**
 * @struct WisTopLevelStructureBuildDesc
 * @ingroup Structures Raytracing
 *
 *
 * @section WisTopLevelStructureBuildDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisTopLevelStructureBuildDesc {
 *     WisAccelerationStructureFlags flags;
 *     uint32_t                      instance_count;
 *     uint64_t                      instance_buffer_address;
 * } WisTopLevelStructureBuildDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  TopLevelStructureBuildDesc {
 *     wis::AccelerationStructureFlags flags;
 *     std::uint32_t                   instance_count;
 *     std::uint64_t                   instance_buffer_address;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTopLevelStructureBuildDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` The build flags for the acceleration structure build.
 * - `instance_count` The number of instances in the top-level acceleration structure.
 * - `instance_buffer_address` The GPU address of the instance buffer for the top-level acceleration structure.
 * \endcond
 *
 * @section WisTopLevelStructureBuildDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTopLevelStructureBuildDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisRaytracingExtensionGetTopLevelStructureInfo
 * \endcond
 */
