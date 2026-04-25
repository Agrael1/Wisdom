/**
 * @struct wisRaytracingExtensionSupported
 * @ingroup Functions Raytracing
 *
 * To check if raytracing is supported on a device, you can use the `wisRaytracingExtensionSupported` function.
 *
 * @section wisRaytracingExtensionSupported_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * bool wisRaytracingExtensionSupported(WisRaytracingExtension* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * bool wisVKRaytracingExtensionSupported(WisVKRaytracingExtension* self);
 *
 * // Provided by Wisdom 0.7.1.
 * bool wisDX12RaytracingExtensionSupported(WisDX12RaytracingExtension* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis
 * ```
 * </details>
 *
 * \endcond
 *
 * @section wisRaytracingExtensionSupported_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 *
 * - **return** true if raytracing is supported, false otherwise.
 *
 * \endcond
 *
 * @section wisRaytracingExtensionSupported_descr Description
 * <hr>
 * 
 * This function must be called after device creation, where the handle is passed to the `WisDeviceRequirements` when
 * creating the device. It checks if raytracing is supported on the current device and returns a boolean value indicating the
 * result.
 * 
 * The result is not cached, so the call may be expensive. It is recommended to call this function once and cache the
 * result if you need to check for raytracing support multiple times.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisRaytracingExtensionSupported_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
