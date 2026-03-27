/**
 * @struct wisSwapchainGetCurrentIndex
 * @ingroup Functions
 *
 *
 * @section wisSwapchainGetCurrentIndex_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisSwapchainGetCurrentIndex(const WisSwapchain* self,
 * uint32_t*           index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKSwapchainGetCurrentIndex(const WisVKSwapchain* self,
 * uint32_t*             index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12SwapchainGetCurrentIndex(const WisDX12Swapchain* self,
 * uint32_t*               index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint32_t Swapchain::GetCurrentIndex(wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint32_t VKSwapchain::GetCurrentIndex(wis::Result& out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint32_t DX12Swapchain::GetCurrentIndex(wis::Result& out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisSwapchainGetCurrentIndex_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisSwapchain instance.
 * - `index` Index of the current backbuffer.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisSwapchainGetCurrentIndex_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisSwapchainGetCurrentIndex_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */