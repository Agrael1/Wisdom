/**
 * @struct WisCompositeAlpha WisCompositeAlpha
 * @ingroup Enumerations Core
 *
 * @section WisCompositeAlpha_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisCompositeAlpha {
 *     WisCompositeAlphaOpaque = 0,
 *     WisCompositeAlphaPreMultiplied = 1,
 *     WisCompositeAlphaPostMultiplied = 2,
 *     WisCompositeAlphaInherit = 3,
 * } WisCompositeAlpha;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class CompositeAlpha {
 *     Opaque = 0,
 *     PreMultiplied = 1,
 *     PostMultiplied = 2,
 *     Inherit = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCompositeAlpha_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Composite alpha flags for swapchain creation.
 *
 * \note Translates to  `DXGI_ALPHA_MODE` for DirectX 12 implementation, and `VkCompositeAlphaFlagBitsKHR` for Vulkan implementation.
 *
 * Values:
 * - `WisCompositeAlphaOpaque = 0`: The alpha channel, if it exists, is ignored. The image is treated as opaque.
 * - `WisCompositeAlphaPreMultiplied = 1`: The alpha channel, if it exists, is respected and used in compositing. The premultiplied alpha format is expected.
 * - `WisCompositeAlphaPostMultiplied = 2`: The alpha channel, if it exists, is respected and used in compositing. The postmultiplied alpha format is expected.
 * - `WisCompositeAlphaInherit = 3`: The alpha channel, if it exists, is respected and used in compositing based on the platform's default behavior.
 * \endcond
 *
 *
 * @section WisCompositeAlpha_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSwapchainDesc
 * \endcond
 */
