/**
 * @struct wisRaytracingExtensionDestroyAccelerationStructures
 * @ingroup Functions Raytracing
 *
 *
 * @section wisRaytracingExtensionDestroyAccelerationStructures_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisRaytracingExtensionDestroyAccelerationStructures(WisRaytracingExtension* self,
 *                                                          WisAccelerationStructure* acceleration_structures,
 *                                                          size_t                    structure_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVKRaytracingExtensionDestroyAccelerationStructures(WisVKRaytracingExtension* self,
 *                                                            WisVKAccelerationStructure* acceleration_structures,
 *                                                            size_t                      structure_count);
 *
 * // Provided by Wisdom 0.7.1.
 * void wisDX12RaytracingExtensionDestroyAccelerationStructures(WisDX12RaytracingExtension* self,
 *                                                              WisDX12AccelerationStructure* acceleration_structures,
 *                                                              size_t                        structure_count);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * void RaytracingExtension::DestroyAccelerationStructures(wis::span<wis::AccelerationStructure>
 * acceleration_structures) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * void VKRaytracingExtension::DestroyAccelerationStructures(wis::span<wis::VKAccelerationStructure>
 * acceleration_structures) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * void DX12RaytracingExtension::DestroyAccelerationStructures(wis::span<wis::DX12AccelerationStructure>
 * acceleration_structures) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisRaytracingExtensionDestroyAccelerationStructures_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 * - `acceleration_structures` The acceleration structures to destroy.
 * - `structure_count` The number of acceleration structures to destroy.
 * \endcond
 *
 * @section wisRaytracingExtensionDestroyAccelerationStructures_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisRaytracingExtensionDestroyAccelerationStructures_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
