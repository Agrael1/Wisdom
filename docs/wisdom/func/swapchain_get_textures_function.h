/**
 * @struct wisSwapchainGetTextures
 * @ingroup Functions
 *
 *
 * @section wisSwapchainGetTextures_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisSwapchainGetTextures(const WisSwapchain* self,
 *                                   WisTexture*         buffers,
 *                                   size_t              buffer_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKSwapchainGetTextures(const WisVKSwapchain* self,
 *                                     WisVKTexture*         buffers,
 *                                     size_t                buffer_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12SwapchainGetTextures(const WisDX12Swapchain* self,
 *                                       WisDX12Texture*         buffers,
 *                                       size_t                  buffer_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result Swapchain::GetTextures(wis::span<wis::Texture> buffers) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKSwapchain::GetTextures(wis::span<wis::VKTexture> buffers) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12Swapchain::GetTextures(wis::span<wis::DX12Texture> buffers) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisSwapchainGetTextures_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisSwapchain instance.
 * - `buffers` points to an array of texture views, which are filled with swapchain backbuffers on success.
 * - `buffer_count` specifies the number of buffers to get. It @wis_must be greater or equal to the value, supplied to `WisSwapchainDesc::image_count` or subsequent `WisSwapchainUpdateDesc::image_count`.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisSwapchainGetTextures_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisSwapchainGetTextures_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */