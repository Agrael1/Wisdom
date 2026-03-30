/**
 * @struct wisDeviceQueryProperties
 * @ingroup Functions
 *
 *
 * @section wisDeviceQueryProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisDeviceQueryProperties(const WisDevice* self,
 *                               void*            properties);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKDeviceQueryProperties(const WisVKDevice* self,
 *                                 void*              properties);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12DeviceQueryProperties(const WisDX12Device* self,
 *                                   void*                properties);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void Device::QueryProperties(void* properties) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKDevice::QueryProperties(void* properties) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12Device::QueryProperties(void* properties) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceQueryProperties_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `properties` describes a pointer to one of the query structs, which is filled with device properties.
 * \endcond
 *
 * @section wisDeviceQueryProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceQueryProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */