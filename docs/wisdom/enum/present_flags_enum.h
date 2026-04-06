/**
 * @struct WisPresentFlags WisPresentFlags
 * @ingroup Enumerations Core
 *
 * @section WisPresentFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisPresentFlags {
 *     WisPresentFlagsNone = 0,
 *     WisPresentFlagsTimeoutOnBlock = (1u << 0),
 * } WisPresentFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class PresentFlags : uint32_t {
 *     None = 0,
 *     TimeoutOnBlock = (1u << 0),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPresentFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Swapchain creation flags.
 *
 * \note Translates to DirectX 12 as uint32_t.
 * Values:
 * - `WisPresentFlagsNone = 0`: No flags set. Swapchain is regular.
 * - `WisPresentFlagsTimeoutOnBlock = (1 << 0)`: Fail present if the presentation engine is busy. If not set, the implementation @wis_may choose to block until the presentation engine is available.
 * \endcond
 *
 *
 * @section WisPresentFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisSwapchainPresent
 * \endcond
 */