/**
 * @struct wisSwapchainPresent
 * @ingroup Functions Core
 *
 *
 * @section wisSwapchainPresent_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisSwapchainPresent(const WisSwapchain* self,
 *                               WisPresentFlags     flags,
 *                               const WisRect*      rects,
 *                               size_t              rect_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKSwapchainPresent(const WisVKSwapchain* self,
 *                                 WisPresentFlags       flags,
 *                                 const WisRect*        rects,
 *                                 size_t                rect_count);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12SwapchainPresent(const WisDX12Swapchain* self,
 *                                   WisPresentFlags         flags,
 *                                   const WisRect*          rects,
 *                                   size_t                  rect_count);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result Swapchain::Present(wis::PresentFlags          flags,
 *                                wis::span<const wis::Rect> rects) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKSwapchain::Present(wis::PresentFlags          flags,
 *                                  wis::span<const wis::Rect> rects) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12Swapchain::Present(wis::PresentFlags          flags,
 *                                    wis::span<const wis::Rect> rects) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisSwapchainPresent_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisSwapchain instance.
 * - `flags` describes the presentation options.
 * - `rects` points to an array of rectangles to present. If `nullptr`, the entire image is presented.
 * - `rect_count` specifies the number of rectangles to present. It @wis_must be 0 if rects is `nullptr`.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisSwapchainPresent_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisSwapchainPresent_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */