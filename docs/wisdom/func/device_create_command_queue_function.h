/**
 * @struct wisDeviceCreateCommandQueue
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateCommandQueue_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceCreateCommandQueue(const WisDevice* self,
 *                                       WisCommandQueueType type,
 *                                       WisCommandQueue*    queue);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceCreateCommandQueue(const WisVKDevice* self,
 *                                         WisCommandQueueType type,
 *                                         WisVKCommandQueue*  queue);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceCreateCommandQueue(const WisDX12Device* self,
 *                                           WisCommandQueueType  type,
 *                                           WisDX12CommandQueue* queue);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::CommandQueue Device::CreateCommandQueue(wis::CommandQueueType type,
 *                                                            wis::Result&          out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKCommandQueue VKDevice::CreateCommandQueue(wis::CommandQueueType type,
 *                                                                wis::Result&          out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12CommandQueue DX12Device::CreateCommandQueue(wis::CommandQueueType type,
 *                                                                    wis::Result&          out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateCommandQueue_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `type` specifies the type of the command queue to create.
 * - `queue` points to WisCommandQueue, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateCommandQueue_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateCommandQueue_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
