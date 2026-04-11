/**
 * @struct WisResolveMode WisResolveMode
 * @ingroup Enumerations Core
 *
 * @section WisResolveMode_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisResolveMode {
 *     WisResolveModeNone = 0,
 *     WisResolveModeAverage = 1,
 *     WisResolveModeMin = 2,
 *     WisResolveModeMax = 3,
 * } WisResolveMode;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class ResolveMode {
 *     None = 0,
 *     Average = 1,
 *     Min = 2,
 *     Max = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisResolveMode_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Resolve mode used for multisample resolve in render pass attachments.
 *
 * \note Translates to  `D3D12_RESOLVE_MODE` for DirectX 12 implementation, and `VkResolveModeFlagBits` for Vulkan
 * implementation.
 *
 * Values:
 * - `WisResolveModeNone = 0`: Do not resolve the attachment.
 * - `WisResolveModeAverage = 1`: Resolve by averaging samples.
 * - `WisResolveModeMin = 2`: Resolve by selecting minimum sample value.
 * - `WisResolveModeMax = 3`: Resolve by selecting maximum sample value.
 * \endcond
 *
 *
 * @section WisResolveMode_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisResolveDesc
 * \endcond
 */
