/**
 * @struct wisRaytracingExtensionCreateAccelerationStructures
 * @ingroup Functions Raytracing
 *
 *
 * @section wisRaytracingExtensionCreateAccelerationStructures_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisRaytracingExtensionCreateAccelerationStructures(WisRaytracingExtension* self,
 *                                                              WisBuffer*                          buffer,
 *                                                              const WisAccelerationStructureDesc* structures,
 *                                                              size_t                              structure_count,
 *                                                              WisAccelerationStructure* acceleration_structures);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKRaytracingExtensionCreateAccelerationStructures(WisVKRaytracingExtension* self,
 *                                                                WisVKBuffer*                          buffer,
 *                                                                const WisVKAccelerationStructureDesc* structures,
 *                                                                size_t                                structure_count,
 *                                                                WisVKAccelerationStructure* acceleration_structures);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12RaytracingExtensionCreateAccelerationStructures(WisDX12RaytracingExtension* self,
 *                                                                  WisDX12Buffer*                          buffer,
 *                                                                  const WisDX12AccelerationStructureDesc* structures,
 *                                                                  size_t structure_count,
 *                                                                  WisDX12AccelerationStructure*
 * acceleration_structures);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * wis::Result RaytracingExtension::CreateAccelerationStructures(wis::Buffer&                                    buffer,
 *                                                               wis::span<const wis::AccelerationStructureDesc>
 * structures, wis::AccelerationStructure*                     acceleration_structures) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * wis::Result VKRaytracingExtension::CreateAccelerationStructures(wis::VKBuffer& buffer, wis::span<const
 * wis::VKAccelerationStructureDesc> structures, wis::VKAccelerationStructure* acceleration_structures) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * wis::Result DX12RaytracingExtension::CreateAccelerationStructures(wis::DX12Buffer& buffer, wis::span<const
 * wis::DX12AccelerationStructureDesc> structures, wis::DX12AccelerationStructure* acceleration_structures) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisRaytracingExtensionCreateAccelerationStructures_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 * - `buffer` The buffer to write the acceleration structure data to.
 * - `structures` The descriptions of the acceleration structures to create.
 * - `structure_count` The number of acceleration structures to create.
 * - `acceleration_structures` The created acceleration structure handle.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisRaytracingExtensionCreateAccelerationStructures_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisRaytracingExtensionCreateAccelerationStructures_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
