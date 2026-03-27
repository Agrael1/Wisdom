/**
 * @struct WisSwapchainUpdateDesc
 * @ingroup Structures
 *
 *
 * @section WisSwapchainUpdateDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisSwapchainUpdateDesc {
 *     uint32_t      width;
 *     uint32_t      height;
 *     uint32_t      image_count;
 *     WisDataFormat format;
 *     bool          vsync;
 * } WisSwapchainUpdateDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  SwapchainUpdateDesc {
 *     std::uint32_t   width;
 *     std::uint32_t   height;
 *     std::uint32_t   image_count;
 *     wis::DataFormat format;
 *     bool            vsync;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSwapchainUpdateDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `width` New swapchain image width in pixels.
 * - `height` New swapchain image height in pixels.
 * - `image_count` Number of images in the swapchain.
 * - `format` Swapchain image format.
 * - `vsync` Vsync enabled or not. If true, the presentation is synchronized to the vertical blanking interval, which can help prevent screen tearing. If false, the presentation is not synchronized, which can result in higher frame rates but @wis_may cause screen tearing.
 * \endcond
 *
 * @section WisSwapchainUpdateDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSwapchainUpdateDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisSwapchainUpdate
 * \endcond
 */