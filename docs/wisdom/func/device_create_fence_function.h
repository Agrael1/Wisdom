/**
 * @struct wisDeviceCreateFence
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreateFence_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateFence(const WisDevice* self,
 *                                uint64_t         initial_value,
 *                                WisFence*        fence);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateFence(const WisVKDevice* self,
 *                                  uint64_t           initial_value,
 *                                  WisVKFence*        fence);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateFence(const WisDX12Device* self,
 *                                    uint64_t             initial_value,
 *                                    WisDX12Fence*        fence);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Fence Device::CreateFence(std::uint64_t initial_value,
 *                                              wis::Result&  out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKFence VKDevice::CreateFence(std::uint64_t initial_value,
 *                                                  wis::Result&  out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Fence DX12Device::CreateFence(std::uint64_t initial_value,
 *                                                      wis::Result&  out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateFence_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `initial_value` defines the initial value of the fence.
 * - `fence` points to WisFence, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateFence_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateFence_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */