/**
 * @struct wisInitRaytracingExtension
 * @ingroup Functions Raytracing
 *
 * Initializes a WisRaytracingExtension handle. This function must be called before passing the handle to the
 * `WisDeviceRequirements` when creating a device. It sets up the necessary state for the raytracing extension to be
 * used on a device.
 *
 * @section wisInitRaytracingExtension_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisInitRaytracingExtension(WisRaytracingExtension* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVKInitRaytracingExtension(WisVKRaytracingExtension* self);
 *
 * // Provided by Wisdom 0.7.1.
 * void wisDX12InitRaytracingExtension(WisDX12RaytracingExtension* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * RaytracingExtension::RaytracingExtension() noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * VKRaytracingExtension::VKRaytracingExtension() noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * DX12RaytracingExtension::DX12RaytracingExtension() noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisInitRaytracingExtension_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` is a pointer to uninitialized WisRaytracingExtension instance memory. It will be initialized by
 * this function.
 * **note** The corresponding destroy function is `wisDestroyRaytracingExtension`.
 * \endcond
 *
 * @section wisInitRaytracingExtension_descr Description
 * <hr>
 *
 * Passing an uninitialized handle to `WisDeviceRequirements` when creating a device will not enable raytracing features
 * on the device.
 *
 * \note WisRaytracingExtension references device. That means the device resources will not be released until the
 * extension is destroyed.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisInitRaytracingExtension_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
