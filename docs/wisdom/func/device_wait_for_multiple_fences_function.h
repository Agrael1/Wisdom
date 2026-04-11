/**
 * @struct wisDeviceWaitForMultipleFences
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceWaitForMultipleFences_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceWaitForMultipleFences(const WisDevice* self,
 *                                          const WisFenceView* fences,
 *                                          const uint64_t*     fence_values,
 *                                          size_t              fence_count,
 *                                          WisMultiWaitType    wait_for,
 *                                          uint64_t            timeout);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceWaitForMultipleFences(const WisVKDevice* self,
 *                                            const WisVKFenceView* fences,
 *                                            const uint64_t*       fence_values,
 *                                            size_t                fence_count,
 *                                            WisMultiWaitType      wait_for,
 *                                            uint64_t              timeout);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceWaitForMultipleFences(const WisDX12Device* self,
 *                                              const WisDX12FenceView* fences,
 *                                              const uint64_t*         fence_values,
 *                                              size_t                  fence_count,
 *                                              WisMultiWaitType        wait_for,
 *                                              uint64_t                timeout);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result Device::WaitForMultipleFences(const wis::FenceView* fences,
 *                                           const std::uint64_t*  fence_values,
 *                                           std::size_t           fence_count,
 *                                           wis::MultiWaitType    wait_for,
 *                                           std::uint64_t         timeout) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKDevice::WaitForMultipleFences(const wis::VKFenceView* fences,
 *                                             const std::uint64_t*    fence_values,
 *                                             std::size_t             fence_count,
 *                                             wis::MultiWaitType      wait_for,
 *                                             std::uint64_t           timeout) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12Device::WaitForMultipleFences(const wis::DX12FenceView* fences,
 *                                               const std::uint64_t*      fence_values,
 *                                               std::size_t               fence_count,
 *                                               wis::MultiWaitType        wait_for,
 *                                               std::uint64_t             timeout) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceWaitForMultipleFences_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `fences` defines an array of fence views to wait on.
 * - `fence_values` defines fence values to wait for; array @wis_must have fence_count values.
 * - `fence_count` describes number of fences to wait on.
 * - `wait_for` indicates the kind of wait, where All waits for all fences and Any waits for any fence; default is
 * `WisMultiWaitTypeAll`.
 * - `timeout` indicates timeout in nanoseconds; if UINT64_MAX, waits indefinitely.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceWaitForMultipleFences_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceWaitForMultipleFences_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
