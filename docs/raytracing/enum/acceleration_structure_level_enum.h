/**
 * @struct WisAccelerationStructureLevel WisAccelerationStructureLevel
 * @ingroup Enumerations Raytracing
 *
 * Acceleration structure level enumeration for raytracing.
 *
 * @section WisAccelerationStructureLevel_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisAccelerationStructureLevel {
 *     WisAccelerationStructureLevelTopLevel = 0,
 *     WisAccelerationStructureLevelBottomLevel = 1,
 * } WisAccelerationStructureLevel;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class AccelerationStructureLevel {
 *     TopLevel = 0,
 *     BottomLevel = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAccelerationStructureLevel_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Enumeration for the level of an acceleration structure in raytracing.
 *
 * \note Translates to  `VkAccelerationStructureTypeKHR` for Vulkan implementation.
 *
 * Values:
 * - `WisAccelerationStructureLevelTopLevel = 0`: Top-level acceleration structure, which contains instances of
 * bottom-level structures.
 * - `WisAccelerationStructureLevelBottomLevel = 1`: Bottom-level acceleration structure, which contains geometry data
 * such as triangles or AABBs.
 * \endcond
 *
 *
 * @section WisAccelerationStructureLevel_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisAccelerationStructureDesc
 * \endcond
 */
