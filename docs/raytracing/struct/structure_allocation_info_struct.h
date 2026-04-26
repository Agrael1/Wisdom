/**
 * @struct WisStructureAllocationInfo
 * @ingroup Structures Raytracing
 *
 *
 * @section WisStructureAllocationInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStructureAllocationInfo {
 *     uint64_t structure_size;
 *     uint64_t scratch_size;
 *     uint64_t update_size;
 * } WisStructureAllocationInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StructureAllocationInfo {
 *     std::uint64_t structure_size;
 *     std::uint64_t scratch_size;
 *     std::uint64_t update_size;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStructureAllocationInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `structure_size` The size of the acceleration structure in bytes.
 * - `scratch_size` The size of the scratch buffer needed to build the acceleration structure in bytes.
 * - `update_size` The size of the scratch buffer needed to update the acceleration structure in bytes.
 * \endcond
 *
 * @section WisStructureAllocationInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStructureAllocationInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisRaytracingExtensionGetBottomLevelStructureInfo
 * \endcond
 */
