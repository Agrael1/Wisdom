/**
 * @struct wisDestroyAccelerationStructure
 * @ingroup Functions Raytracing
 *
 *
 * @section wisDestroyAccelerationStructure_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisDestroyAccelerationStructure(WisAccelerationStructure* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVKDestroyAccelerationStructure(WisVKAccelerationStructure* self);
 *
 * // Provided by Wisdom 0.7.1.
 * void wisDX12DestroyAccelerationStructure(WisDX12AccelerationStructure* self);
 * ```
 * </details>
 *
 * \endcond
 *
 * @section wisDestroyAccelerationStructure_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAccelerationStructure instance.
 * \endcond
 *
 * @section wisDestroyAccelerationStructure_descr Description
 * <hr>
 *
 * @warning The handle does not reference the underlying buffer or device.
 * The caller is responsible for ensuring that the buffer and device remain valid until the acceleration structure is
 * destroyed. For Top Level Acceleration Structures, the caller @wis_must ensure that the acceleration structure is not
 * in use by any command.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDestroyAccelerationStructure_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
