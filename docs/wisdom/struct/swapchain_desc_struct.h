/**
 * @struct WisSwapchainDesc
 * @ingroup Structures Core
 *
 *
 * @section WisSwapchainDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisSwapchainDesc {
 *     uint32_t             width;
 *     uint32_t             height;
 *     uint32_t             image_count;
 *     WisTextureUsageFlags texture_usage_flags;
 *     WisDataFormat        format;
 *     WisSwapchainScaling  scaling;
 *     WisSwapchainFlags    flags;
 *     WisCompositeAlpha    composite_alpha;
 * } WisSwapchainDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  SwapchainDesc {
 *     std::uint32_t          width;
 *     std::uint32_t          height;
 *     std::uint32_t          image_count;
 *     wis::TextureUsageFlags texture_usage_flags;
 *     wis::DataFormat        format;
 *     wis::SwapchainScaling  scaling;
 *     wis::SwapchainFlags    flags;
 *     wis::CompositeAlpha    composite_alpha;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSwapchainDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `width` defines swapchain image width in pixels.
 * - `height` describes swapchain image height in pixels.
 * - `image_count` indicates number of images in the swapchain.
 * - `texture_usage_flags` describes texture usage flags for the swapchain images. Describe how the swapchain images
 * will be used.
 * - `format` describes swapchain image format.
 * - `scaling` describes swapchain scaling mode.
 * - `flags` describes swapchain flags. Describe additional options for the swapchain.
 * - `composite_alpha` defines composite alpha mode. Describe how the alpha channel of the swapchain images is treated
 * during compositing.
 * \endcond
 *
 * @section WisSwapchainDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSwapchainDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceCreateSwapchain
 * \endcond
 */
