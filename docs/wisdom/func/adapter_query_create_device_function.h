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
 *                                       size_t                     index,
 *                                       WisDeviceExtensionHeader** extensions,
 *                                       size_t                     extension_count,
 *                                       WisDevice*                 device);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKAdapterQueryCreateDevice(const WisVKAdapterQuery* self,
 *                                         size_t                       index,
 *                                         WisVKDeviceExtensionHeader** extensions,
 *                                         size_t                       extension_count,
 *                                         WisVKDevice*                 device);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12AdapterQueryCreateDevice(const WisDX12AdapterQuery* self,
 *                                           size_t                         index,
 *                                           WisDX12DeviceExtensionHeader** extensions,
 *                                           size_t                         extension_count,
 *                                           WisDX12Device*                 device);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Device AdapterQuery::CreateDevice(std::size_t                            index,
 *                                                      wis::span<wis::DeviceExtensionHeader*> extensions,
 *                                                      wis::Result&                           out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKDevice VKAdapterQuery::CreateDevice(std::size_t                              index,
 *                                                          wis::span<wis::VKDeviceExtensionHeader*> extensions,
 *                                                          wis::Result&                             out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Device DX12AdapterQuery::CreateDevice(std::size_t                                index,
 *                                                              wis::span<wis::DX12DeviceExtensionHeader*> extensions,
 *                                                              wis::Result&                               out_result) const noexcept;
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
 * - `extensions` points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
 * - `extension_count` counts the number of extensions in the `extensions` array.
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