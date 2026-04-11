/**
 * @struct wisDeviceGetSurfaceParameters
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceGetSurfaceParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceGetSurfaceParameters(const WisDevice* self,
 *                                         WisSurfaceView       surface,
 *                                         WisSurfaceParameters*params);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceGetSurfaceParameters(const WisVKDevice* self,
 *                                           WisVKSurfaceView     surface,
 *                                           WisSurfaceParameters*params);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceGetSurfaceParameters(const WisDX12Device* self,
 *                                             WisDX12SurfaceView   surface,
 *                                             WisSurfaceParameters*params);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::SurfaceParameters Device::GetSurfaceParameters(wis::SurfaceView surface,
 *                                                                   wis::Result&     out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::SurfaceParameters VKDevice::GetSurfaceParameters(wis::VKSurfaceView surface,
 *                                                                     wis::Result&       out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::SurfaceParameters DX12Device::GetSurfaceParameters(wis::DX12SurfaceView surface,
 *                                                                       wis::Result&         out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceGetSurfaceParameters_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `surface` describes a pointer to WisSurface to check the presentation support for.
 * - `params` Parameters of the surface.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceGetSurfaceParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceGetSurfaceParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
