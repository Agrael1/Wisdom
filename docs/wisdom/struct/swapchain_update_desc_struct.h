/**
 * @struct WisSwapchainUpdateDesc
 * @ingroup Structures Core
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
 * - `width` defines new swapchain image width in pixels.
 * - `height` describes new swapchain image height in pixels.
 * - `image_count` indicates number of images in the swapchain.
 * - `format` describes swapchain image format.
 * - `vsync` indicates controls vsync; when true, presentation is synchronized to the vertical blanking interval to
 * reduce tearing, whereas false can improve frame rate but can introduce tearing. \endcond
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
