/**
 * @struct wisDeviceCreateSwapchain
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateSwapchain_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceCreateSwapchain(const WisDevice* self,
 *                                    const WisSurface*       surface,
 *                                    const WisCommandQueue*  queue,
 *                                    const WisSwapchainDesc* desc,
 *                                    WisSwapchain*           swapchain);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceCreateSwapchain(const WisVKDevice* self,
 *                                      const WisVKSurface*      surface,
 *                                      const WisVKCommandQueue* queue,
 *                                      const WisSwapchainDesc*  desc,
 *                                      WisVKSwapchain*          swapchain);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceCreateSwapchain(const WisDX12Device* self,
 *                                        const WisDX12Surface*      surface,
 *                                        const WisDX12CommandQueue* queue,
 *                                        const WisSwapchainDesc*    desc,
 *                                        WisDX12Swapchain*          swapchain);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::Swapchain Device::CreateSwapchain(const wis::Surface&       surface,
 *                                                      const wis::CommandQueue&  queue,
 *                                                      const wis::SwapchainDesc& desc,
 *                                                      wis::Result&              out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKSwapchain VKDevice::CreateSwapchain(const wis::VKSurface&      surface,
 *                                                          const wis::VKCommandQueue& queue,
 *                                                          const wis::SwapchainDesc&  desc,
 *                                                          wis::Result&               out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12Swapchain DX12Device::CreateSwapchain(const wis::DX12Surface&      surface,
 *                                                              const wis::DX12CommandQueue& queue,
 *                                                              const wis::SwapchainDesc&    desc,
 *                                                              wis::Result&                 out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateSwapchain_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `surface` describes a pointer to WisSurface to create the swapchain for. Surface is ref-counted.
 * - `queue` describes a pointer to WisCommandQueue to create the swapchain for. Queue is ref-counted and @wis_must be a graphics queue.
 * - `desc` indicates a pointer to WisSwapchainDesc, which describes the swapchain to create.
 * - `swapchain` points to WisSwapchain, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateSwapchain_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateSwapchain_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */