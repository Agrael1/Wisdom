/**
 * @struct WisColorComponents WisColorComponents
 * @ingroup Enumerations Core
 *
 * @section WisColorComponents_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisColorComponents {
 *     WisColorComponentsNone = 0,
 *     WisColorComponentsR = (1u << 0),
 *     WisColorComponentsG = (1u << 1),
 *     WisColorComponentsB = (1u << 2),
 *     WisColorComponentsA = (1u << 3),
 *     WisColorComponentsAll = 15,
 * } WisColorComponents;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class ColorComponents : uint32_t {
 *     None = 0,
 *     R = (1u << 0),
 *     G = (1u << 1),
 *     B = (1u << 2),
 *     A = (1u << 3),
 *     All = 15,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisColorComponents_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Color component flags.
 * Used for color blending operations.
 * 
 * Values:
 * - `WisColorComponentsNone = 0`: No flags set. Color blending is not used.
 * - `WisColorComponentsR = (1 << 0)`: Use red component for blending.
 * - `WisColorComponentsG = (1 << 1)`: Use green component for blending.
 * - `WisColorComponentsB = (1 << 2)`: Use blue component for blending.
 * - `WisColorComponentsA = (1 << 3)`: Use alpha component for blending.
 * - `WisColorComponentsAll = 15`: Use all color components for blending.
 * \endcond
 *
 *
 * @section WisColorComponents_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBlendAttachmentDesc
 * \endcond
 */