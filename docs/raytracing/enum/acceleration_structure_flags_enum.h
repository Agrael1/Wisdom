/**
 * @struct WisAccelerationStructureFlags WisAccelerationStructureFlags
 * @ingroup Enumerations Raytracing
 *
 * @section WisAccelerationStructureFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisAccelerationStructureFlags {
 *     WisAccelerationStructureFlagsNone = 0,
 *     WisAccelerationStructureFlagsAllowUpdate = (1u << 0),
 *     WisAccelerationStructureFlagsAllowCompaction = (1u << 1),
 *     WisAccelerationStructureFlagsPreferFastTrace = (1u << 2),
 *     WisAccelerationStructureFlagsPreferFastBuild = (1u << 3),
 *     WisAccelerationStructureFlagsMinimizeMemory = (1u << 4),
 *     WisAccelerationStructureFlagsPerformUpdate = (1u << 5),
 *     WisAccelerationStructureFlagsIndirectInput = (1u << 6),
 * } WisAccelerationStructureFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class AccelerationStructureFlags : uint32_t {
 *     None = 0,
 *     AllowUpdate = (1u << 0),
 *     AllowCompaction = (1u << 1),
 *     PreferFastTrace = (1u << 2),
 *     PreferFastBuild = (1u << 3),
 *     MinimizeMemory = (1u << 4),
 *     PerformUpdate = (1u << 5),
 *     IndirectInput = (1u << 6),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAccelerationStructureFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Acceleration structure flags for additional acceleration structure features
 *
 * \note Translates to DirectX 12 as D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS, Vulkan as
 * VkBuildAccelerationStructureFlagsKHR.
 *
 * Values:
 * - `WisAccelerationStructureFlagsNone = 0`: No flags set. Acceleration structure is regular.
 * - `WisAccelerationStructureFlagsAllowUpdate = (1 << 0)`: Acceleration structure is allowed to be updated.
 * - `WisAccelerationStructureFlagsAllowCompaction = (1 << 1)`: Acceleration structure is allowed to be compacted.
 * - `WisAccelerationStructureFlagsPreferFastTrace = (1 << 2)`: Acceleration structure is preferred to be fast traced.
 * - `WisAccelerationStructureFlagsPreferFastBuild = (1 << 3)`: Acceleration structure is preferred to be fast built.
 * - `WisAccelerationStructureFlagsMinimizeMemory = (1 << 4)`: Acceleration structure is minimized for memory usage.
 * - `WisAccelerationStructureFlagsPerformUpdate = (1 << 5)`: Acceleration structure build is performed as an update.
 * Only used for update builds.
 * - `WisAccelerationStructureFlagsIndirectInput = (1 << 6)`: Acceleration structure build uses indirect input.
 * \endcond
 *
 *
 * @section WisAccelerationStructureFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBottomLevelStructureBuildDesc, WisTopLevelStructureBuildDesc
 * \endcond
 */
