/**
 * @struct wisDeviceGetFormatPresentationSupport
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceGetFormatPresentationSupport_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * bool wisDeviceGetFormatPresentationSupport(const WisDevice* self,
 *                                            WisSurfaceView   surface,
 *                                            WisDataFormat    format);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * bool wisVKDeviceGetFormatPresentationSupport(const WisVKDevice* self,
 *                                              WisVKSurfaceView   surface,
 *                                              WisDataFormat      format);
 *
 * // Provided by Wisdom 0.7.0.
 * bool wisDX12DeviceGetFormatPresentationSupport(const WisDX12Device* self,
 *                                                WisDX12SurfaceView   surface,
 *                                                WisDataFormat        format);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD bool Device::GetFormatPresentationSupport(wis::SurfaceView surface,
 *                                                         wis::DataFormat  format) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD bool VKDevice::GetFormatPresentationSupport(wis::VKSurfaceView surface,
 *                                                           wis::DataFormat    format) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD bool DX12Device::GetFormatPresentationSupport(wis::DX12SurfaceView surface,
 *                                                             wis::DataFormat      format) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceGetFormatPresentationSupport_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `surface` describes a pointer to WisSurface to check the presentation support for.
 * - `format` describes the format to check the presentation support for.
 *
 * - **return** Result of operation.
 * \endcond
 *
 * @section wisDeviceGetFormatPresentationSupport_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceGetFormatPresentationSupport_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
