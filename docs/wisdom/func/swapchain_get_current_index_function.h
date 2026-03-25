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
 * uint32_t wisSwapchainGetCurrentIndex(const WisSwapchain* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint32_t wisVKSwapchainGetCurrentIndex(const WisVKSwapchain* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint32_t wisDX12SwapchainGetCurrentIndex(const WisDX12Swapchain* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint32_t Swapchain::GetCurrentIndex() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint32_t VKSwapchain::GetCurrentIndex() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint32_t DX12Swapchain::GetCurrentIndex() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisSwapchainGetCurrentIndex_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisSwapchain instance.
 * 
 * - **return** Index of the current backbuffer.
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