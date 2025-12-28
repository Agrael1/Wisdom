/**
 * @struct wisDeviceCreateCommandList
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreateCommandList_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateCommandList(const WisDevice* self,
 *                                      WisCommandQueueType type,
 *                                      WisCommandList*     list);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateCommandList(const WisVKDevice* self,
 *                                        WisCommandQueueType type,
 *                                        WisVKCommandList*   list);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateCommandList(const WisDX12Device* self,
 *                                          WisCommandQueueType  type,
 *                                          WisDX12CommandList*  list);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::CommandList Device::CreateCommandList(wis::CommandQueueType type,
 *                                                          wis::Result&          out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKCommandList VKDevice::CreateCommandList(wis::CommandQueueType type,
 *                                                              wis::Result&          out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12CommandList DX12Device::CreateCommandList(wis::CommandQueueType type,
 *                                                                  wis::Result&          out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateCommandList_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `type` defines the type of the command list to create.
 * - `list` points to WisCommandList, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateCommandList_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateCommandList_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */