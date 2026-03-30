/**
 * @struct wisDeviceCreateCommandAllocator
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreateCommandAllocator_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateCommandAllocator(const WisDevice* self,
 *                                           WisCommandQueueType type,
 *                                           WisCommandAllocator*allocator);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateCommandAllocator(const WisVKDevice* self,
 *                                             WisCommandQueueType   type,
 *                                             WisVKCommandAllocator*allocator);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateCommandAllocator(const WisDX12Device* self,
 *                                               WisCommandQueueType     type,
 *                                               WisDX12CommandAllocator*allocator);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::CommandAllocator Device::CreateCommandAllocator(wis::CommandQueueType type,
 *                                                                    wis::Result&          out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKCommandAllocator VKDevice::CreateCommandAllocator(wis::CommandQueueType type,
 *                                                                        wis::Result&          out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12CommandAllocator DX12Device::CreateCommandAllocator(wis::CommandQueueType type,
 *                                                                            wis::Result&          out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateCommandAllocator_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `type` specifies the type of the command list this pool is able to allocate.
 * - `allocator` points to WisCommandAllocator, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateCommandAllocator_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateCommandAllocator_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */