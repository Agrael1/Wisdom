/**
 * @struct WisSwapchainScaling WisSwapchainScaling
 * @ingroup Enumerations Core
 *
 * @section WisSwapchainScaling_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisSwapchainScaling {
 *     WisSwapchainScalingNone = 0,
 *     WisSwapchainScalingStretch = 1,
 *     WisSwapchainScalingAspect = 2,
 * } WisSwapchainScaling;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class SwapchainScaling {
 *     None = 0,
 *     Stretch = 1,
 *     Aspect = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSwapchainScaling_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Swapchain scaling mode.
 * 
 * \note Translates to  `DXGI_SCALING` for DirectX 12 implementation, and `VkPresentScalingFlagsEXT` for Vulkan implementation.
 * 
 * Values:
 * - `WisSwapchainScalingNone = 0`: No scaling. The swapchain size is equal to the window size.
 * - `WisSwapchainScalingStretch = 1`: Stretch scaling. The swapchain size is stretched to the window size.
 * - `WisSwapchainScalingAspect = 2`: Aspect scaling. The swapchain size is scaled to the window size with aspect ratio preserved.
 * \endcond
 *
 *
 * @section WisSwapchainScaling_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSwapchainDesc
 * \endcond
 */
