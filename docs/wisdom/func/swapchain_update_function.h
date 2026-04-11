/**
 * @struct wisSwapchainUpdate
 * @ingroup Functions Core
 *
 *
 * @section wisSwapchainUpdate_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisSwapchainUpdate(const WisSwapchain* self,
 *                              const WisSwapchainUpdateDesc* desc);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKSwapchainUpdate(const WisVKSwapchain* self,
 *                                const WisSwapchainUpdateDesc* desc);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12SwapchainUpdate(const WisDX12Swapchain* self,
 *                                  const WisSwapchainUpdateDesc* desc);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result Swapchain::Update(const wis::SwapchainUpdateDesc& desc) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKSwapchain::Update(const wis::SwapchainUpdateDesc& desc) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12Swapchain::Update(const wis::SwapchainUpdateDesc& desc) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisSwapchainUpdate_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisSwapchain instance.
 * - `desc` indicates a pointer to WisSwapchainUpdateDesc, which describes the new swapchain parameters.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisSwapchainUpdate_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisSwapchainUpdate_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
