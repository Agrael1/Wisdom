/**
 * @struct wisAccelerationStructureGetGPUAddress
 * @ingroup Functions Raytracing
 *
 * @section wisAccelerationStructureGetGPUAddress_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * uint64_t wisAccelerationStructureGetGPUAddress(WisAccelerationStructure* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * uint64_t wisVKAccelerationStructureGetGPUAddress(WisVKAccelerationStructure* self);
 *
 * // Provided by Wisdom 0.7.1.
 * uint64_t wisDX12AccelerationStructureGetGPUAddress(WisDX12AccelerationStructure* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD std::uint64_t AccelerationStructure::GetGPUAddress() noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD std::uint64_t VKAccelerationStructure::GetGPUAddress() noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD std::uint64_t DX12AccelerationStructure::GetGPUAddress() noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisAccelerationStructureGetGPUAddress_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAccelerationStructure instance.
 *
 * - **return** The GPU address of the acceleration structure.
 * \endcond
 *
 * @section wisAccelerationStructureGetGPUAddress_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisAccelerationStructureGetGPUAddress_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
