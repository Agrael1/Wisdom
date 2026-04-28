/**
 * @struct WisAccelerationStructureDesc
 * @ingroup Structures Raytracing
 *
 *
 * @section WisAccelerationStructureDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisAccelerationStructureDesc {
 *     WisAccelerationStructureLevel level;
 *     uint64_t                      offset;
 *     uint64_t                      size;
 * } WisAccelerationStructureDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVKAccelerationStructureDesc {
 *     WisAccelerationStructureLevel level;
 *     uint64_t                      offset;
 *     uint64_t                      size;
 * } WisVKAccelerationStructureDesc;
 *
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisDX12AccelerationStructureDesc {
 *     WisAccelerationStructureLevel level;
 *     uint64_t                      offset;
 *     uint64_t                      size;
 * } WisDX12AccelerationStructureDesc;
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct AccelerationStructureDesc {
 *     wis::AccelerationStructureLevel level;
 *     std::uint64_t                   offset;
 *     std::uint64_t                   size;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VKAccelerationStructureDesc {
 *     wis::AccelerationStructureLevel level;
 *     std::uint64_t                   offset;
 *     std::uint64_t                   size;
 * };
 *
 * // Provided by Wisdom 0.7.1.
 * struct DX12AccelerationStructureDesc {
 *     wis::AccelerationStructureLevel level;
 *     std::uint64_t                   offset;
 *     std::uint64_t                   size;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisAccelerationStructureDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `level` The level of the acceleration structure (top-level or bottom-level).
 * - `offset` The offset in bytes from the start of the buffer where the acceleration structure is located.
 * - `size` The size of the acceleration structure in bytes.
 * \endcond
 *
 * @section WisAccelerationStructureDesc_descr Description
 * <hr>
 *
 * `size` of the acceleration structure can be queried using `wisRaytracingExtensionGetBottomLevelStructureInfo`
 * function, which provides the necessary size based on the build description of the acceleration structure.
 *
 * `offset` allows for creating multiple acceleration structures within the same buffer by specifying different offsets
 * for each structure. It @wis_must be aligned to `AccelerationStructureAlignment`.
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisAccelerationStructureDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisRaytracingExtensionCreateAccelerationStructures
 * \endcond
 */
