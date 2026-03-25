/**
 * @struct WisSwapchainDesc
 * @ingroup Structures
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
 * - `width` Swapchain image width in pixels.
 * - `height` Swapchain image height in pixels.
 * - `image_count` Number of images in the swapchain.
 * - `texture_usage_flags` Texture usage flags for the swapchain images. Describe how the swapchain images will be used.
 * - `format` Swapchain image format.
 * - `scaling` Swapchain scaling mode.
 * - `flags` Swapchain flags. Describe additional options for the swapchain.
 * - `composite_alpha` Composite alpha mode. Describe how the alpha channel of the swapchain images is treated during compositing.
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