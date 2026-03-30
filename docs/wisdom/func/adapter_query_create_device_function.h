/**
 * @struct wisAdapterQueryCreateDevice
 * @ingroup Functions
 *
 *
 * @section wisAdapterQueryCreateDevice_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisAdapterQueryCreateDevice(const WisAdapterQuery* self,
 *                                       size_t                       index,
 *                                       const WisDeviceRequirements* requirements,
 *                                       WisDevice*                   device);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKAdapterQueryCreateDevice(const WisVKAdapterQuery* self,
 *                                         size_t                         index,
 *                                         const WisVKDeviceRequirements* requirements,
 *                                         WisVKDevice*                   device);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12AdapterQueryCreateDevice(const WisDX12AdapterQuery* self,
 *                                           size_t                           index,
 *                                           const WisDX12DeviceRequirements* requirements,
 *                                           WisDX12Device*                   device);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Device AdapterQuery::CreateDevice(std::size_t                    index,
 *                                                      const wis::DeviceRequirements& requirements,
 *                                                      wis::Result&                   out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKDevice VKAdapterQuery::CreateDevice(std::size_t                      index,
 *                                                          const wis::VKDeviceRequirements& requirements,
 *                                                          wis::Result&                     out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Device DX12AdapterQuery::CreateDevice(std::size_t                        index,
 *                                                              const wis::DX12DeviceRequirements& requirements,
 *                                                              wis::Result&                       out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisAdapterQueryCreateDevice_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 * - `index` defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * - `requirements` defines a pointer to WisDeviceRequirements, which defines required features and properties for device creation.
 * - `device` points to WisDevice, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisAdapterQueryCreateDevice_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisAdapterQueryCreateDevice_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */