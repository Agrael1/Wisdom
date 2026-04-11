/**
 * @struct WisComponentSwizzle WisComponentSwizzle
 * @ingroup Enumerations Core
 *
 * @section WisComponentSwizzle_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisComponentSwizzle {
 *     WisComponentSwizzleIdentity = 0,
 *     WisComponentSwizzleRed = 1,
 *     WisComponentSwizzleGreen = 2,
 *     WisComponentSwizzleBlue = 3,
 *     WisComponentSwizzleAlpha = 4,
 *     WisComponentSwizzleZero = 5,
 *     WisComponentSwizzleOne = 6,
 * } WisComponentSwizzle;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class ComponentSwizzle {
 *     Identity = 0,
 *     Red = 1,
 *     Green = 2,
 *     Blue = 3,
 *     Alpha = 4,
 *     Zero = 5,
 *     One = 6,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisComponentSwizzle_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Component swizzle for texture sampling.
 *
 * \note Translates to  `D3D12_SHADER_COMPONENT_MAPPING` for DirectX 12 implementation, and `VkComponentSwizzle` for Vulkan implementation.
 *
 * Values:
 * - `WisComponentSwizzleIdentity = 0`: Use the component as is for sampling.
 * - `WisComponentSwizzleRed = 1`: Use the red component for sampling.
 * - `WisComponentSwizzleGreen = 2`: Use the green component for sampling.
 * - `WisComponentSwizzleBlue = 3`: Use the blue component for sampling.
 * - `WisComponentSwizzleAlpha = 4`: Use the alpha component for sampling.
 * - `WisComponentSwizzleZero = 5`: Use zero for sampling.
 * - `WisComponentSwizzleOne = 6`: Use one for sampling.
 * \endcond
 *
 *
 * @section WisComponentSwizzle_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisComponentMapping
 * \endcond
 */
