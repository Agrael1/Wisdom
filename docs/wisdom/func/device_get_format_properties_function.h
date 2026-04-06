/**
 * @struct wisDeviceGetFormatProperties
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceGetFormatProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceGetFormatProperties(const WisDevice* self,
 *                                        WisDataFormat       format,
 *                                        WisFormatProperties*properties);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceGetFormatProperties(const WisVKDevice* self,
 *                                          WisDataFormat       format,
 *                                          WisFormatProperties*properties);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceGetFormatProperties(const WisDX12Device* self,
 *                                            WisDataFormat        format,
 *                                            WisFormatProperties* properties);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::FormatProperties Device::GetFormatProperties(wis::DataFormat format,
 *                                                                 wis::Result&    out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::FormatProperties VKDevice::GetFormatProperties(wis::DataFormat format,
 *                                                                   wis::Result&    out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::FormatProperties DX12Device::GetFormatProperties(wis::DataFormat format,
 *                                                                     wis::Result&    out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceGetFormatProperties_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `format` describes the format to get properties for.
 * - `properties` Properties of the format.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceGetFormatProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceGetFormatProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */