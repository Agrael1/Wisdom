/**
 * @struct WisTextureBindingFlags WisTextureBindingFlags
 * @ingroup Enumerations Core
 *
 * @section WisTextureBindingFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisTextureBindingFlags {
 *     WisTextureBindingFlagsNone = 0,
 *     WisTextureBindingFlagsDepthView = (1u << 0),
 *     WisTextureBindingFlagsStencilView = (1u << 1),
 * } WisTextureBindingFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class TextureBindingFlags : uint32_t {
 *     None = 0,
 *     DepthView = (1u << 0),
 *     StencilView = (1u << 1),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureBindingFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Texture binding flags, used for extra options.
 *
 * Values:
 * - `WisTextureBindingFlagsNone = 0`: No flags set. Texture view is regular. Implies color read.
 * - `WisTextureBindingFlagsDepthView = (1 << 0)`: Texture view is used to read depth. Used for special formats that
 * feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader.
 * - `WisTextureBindingFlagsStencilView = (1 << 1)`: Texture view is used to read stencil. Used for special formats that
 * feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader.
 * Cannot be combined with `WisTextureBindingFlagsDepthView`.
 * \endcond
 *
 *
 * @section WisTextureBindingFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureBinding
 * \endcond
 */
