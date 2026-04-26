/**
 * @struct wisDestroyRaytracingExtension
 * @ingroup Functions Raytracing
 *
 * Destroys a WisRaytracingExtension handle. This function should be called to clean up any resources associated with
 * the raytracing extension when it is no longer needed.
 *
 * @section wisDestroyRaytracingExtension_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisDestroyRaytracingExtension(WisRaytracingExtension* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVKDestroyRaytracingExtension(WisVKRaytracingExtension* self);
 *
 * // Provided by Wisdom 0.7.1.
 * void wisDX12DestroyRaytracingExtension(WisDX12RaytracingExtension* self);
 * ```
 * </details>
 *
 * \endcond
 *
 * @section wisDestroyRaytracingExtension_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 * \endcond
 *
 * @section wisDestroyRaytracingExtension_descr Description
 * <hr>
 *
 * \note WisRaytracingExtension references device. That means the device resources will not be released until the
 * extension is destroyed.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDestroyRaytracingExtension_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
