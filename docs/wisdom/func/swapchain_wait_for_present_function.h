/**
 * @struct wisSwapchainWaitForPresent
 * @ingroup Functions
 *
 *
 * @section wisSwapchainWaitForPresent_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisSwapchainWaitForPresent(const WisSwapchain* self,
 *                                      uint64_t            timeout_ns);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKSwapchainWaitForPresent(const WisVKSwapchain* self,
 *                                        uint64_t              timeout_ns);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12SwapchainWaitForPresent(const WisDX12Swapchain* self,
 *                                          uint64_t                timeout_ns);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result Swapchain::WaitForPresent(std::uint64_t timeout_ns) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKSwapchain::WaitForPresent(std::uint64_t timeout_ns) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12Swapchain::WaitForPresent(std::uint64_t timeout_ns) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisSwapchainWaitForPresent_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisSwapchain instance.
 * - `timeout_ns` The timeout in nanoseconds. Default is infinite.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisSwapchainWaitForPresent_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisSwapchainWaitForPresent_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */