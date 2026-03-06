/**
 * @struct WisComponentMapping
 * @ingroup Structures
 *
 *
 * @section WisComponentMapping_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisComponentMapping {
 *     WisComponentSwizzle r;
 *     WisComponentSwizzle g;
 *     WisComponentSwizzle b;
 *     WisComponentSwizzle a;
 * } WisComponentMapping;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  ComponentMapping {
 *     wis::ComponentSwizzle r;
 *     wis::ComponentSwizzle g;
 *     wis::ComponentSwizzle b;
 *     wis::ComponentSwizzle a;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisComponentMapping_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `r` Component mapping for Red channel. Default is `WisComponentSwizzleRed`.
 * - `g` Component mapping for Green channel. Default is `WisComponentSwizzleGreen`.
 * - `b` Component mapping for Blue channel. Default is `WisComponentSwizzleBlue`.
 * - `a` Component mapping for Alpha channel. Default is `WisComponentSwizzleAlpha`.
 * \endcond
 *
 * @section WisComponentMapping_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisComponentMapping_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisTextureBinding
 * \endcond
 */