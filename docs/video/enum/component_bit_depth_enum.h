/**
 * @struct WisComponentBitDepth WisComponentBitDepth
 * @ingroup Enumerations Video
 *
 * @section WisComponentBitDepth_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisComponentBitDepth {
 *     WisComponentBitDepthNone = 0,
 *     WisComponentBitDepthBit8 = (1u << 0),
 *     WisComponentBitDepthBit10 = (1u << 1),
 *     WisComponentBitDepthBit12 = (1u << 2),
 *     WisComponentBitDepthBit16 = (1u << 3),
 * } WisComponentBitDepth;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class ComponentBitDepth : uint32_t {
 *     None = 0,
 *     Bit8 = (1u << 0),
 *     Bit10 = (1u << 1),
 *     Bit12 = (1u << 2),
 *     Bit16 = (1u << 3),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisComponentBitDepth_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Bit depth flags for video components. Used to specify supported bit depths for video decoding.
 *
 * \note Translates to Vulkan as VkVideoComponentBitDepthFlagsKHR .
 *
 * Values:
 * - `WisComponentBitDepthNone = 0`: No bit depth is specified.
 * - `WisComponentBitDepthBit8 = (1 << 0)`: 8-bit video component.
 * - `WisComponentBitDepthBit10 = (1 << 1)`: 10-bit video component.
 * - `WisComponentBitDepthBit12 = (1 << 2)`: 12-bit video component.
 * - `WisComponentBitDepthBit16 = (1 << 3)`: 16-bit video component.
 * \endcond
 *
 *
 * @section WisComponentBitDepth_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * \endcond
 */
