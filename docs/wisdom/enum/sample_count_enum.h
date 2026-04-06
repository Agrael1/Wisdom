/**
 * @struct WisSampleCount WisSampleCount
 * @ingroup Enumerations Core
 *
 * @section WisSampleCount_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisSampleCount {
 *     WisSampleCountS1 = 1,
 *     WisSampleCountS2 = 2,
 *     WisSampleCountS4 = 4,
 *     WisSampleCountS8 = 8,
 *     WisSampleCountS16 = 16,
 * } WisSampleCount;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class SampleCount {
 *     S1 = 1,
 *     S2 = 2,
 *     S4 = 4,
 *     S8 = 8,
 *     S16 = 16,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSampleCount_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Sample rate for multisampling.
 *
 * \note Translates to  `uint32_t` for DirectX 12 implementation, and `VkSampleCountFlagBits` for Vulkan implementation.
 *
 * Values:
 * - `WisSampleCountS1 = 1`: 1 sample per pixel.
 * - `WisSampleCountS2 = 2`: 2 samples per pixel.
 * - `WisSampleCountS4 = 4`: 4 samples per pixel.
 * - `WisSampleCountS8 = 8`: 8 samples per pixel.
 * - `WisSampleCountS16 = 16`: 16 samples per pixel.
 * \endcond
 *
 *
 * @section WisSampleCount_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureDesc, WisSampleDesc, WisFormatProperties
 * \endcond
 */
