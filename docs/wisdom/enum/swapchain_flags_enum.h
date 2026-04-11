/**
 * @struct WisSwapchainFlags WisSwapchainFlags
 * @ingroup Enumerations Core
 *
 * @section WisSwapchainFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisSwapchainFlags {
 *     WisSwapchainFlagsNone = 0,
 *     WisSwapchainFlagsAllowTearing = (1u << 0),
 *     WisSwapchainFlagsVSync = (1u << 1),
 *     WisSwapchainFlagsStereo = (1u << 2),
 * } WisSwapchainFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class SwapchainFlags : uint32_t {
 *     None = 0,
 *     AllowTearing = (1u << 0),
 *     VSync = (1u << 1),
 *     Stereo = (1u << 2),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSwapchainFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Swapchain creation flags.
 *
 * Values:
 * - `WisSwapchainFlagsNone = 0`: No flags set. Swapchain is regular.
 * - `WisSwapchainFlagsAllowTearing = (1 << 0)`: Allow tearing.
 * - `WisSwapchainFlagsVSync = (1 << 1)`: Present with vertical sync. If set, the swapchain is presented with vertical sync pulse.
 * - `WisSwapchainFlagsStereo = (1 << 2)`: Stereo swapchain. If set, the swapchain is created for stereo rendering. If not set, the swapchain is created for mono rendering.
 * \endcond
 *
 *
 * @section WisSwapchainFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSwapchainDesc
 * \endcond
 */
