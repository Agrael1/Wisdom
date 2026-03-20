/**
 * @struct WisStaticBorder WisStaticBorder
 * @ingroup Enumerations
 *
 * @section WisStaticBorder_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisStaticBorder {
 *     WisStaticBorderTransparentBlack = 0,
 *     WisStaticBorderOpaqueBlack = 1,
 *     WisStaticBorderOpaqueWhite = 2,
 * } WisStaticBorder;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class StaticBorder {
 *     TransparentBlack = 0,
 *     OpaqueBlack = 1,
 *     OpaqueWhite = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStaticBorder_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Predefined static border colors for samplers. Used when address mode is set to Border.
 * 
 * \note Translates to  `D3D12_STATIC_BORDER_COLOR` for DirectX 12 implementation, and `VkBorderColor` for Vulkan implementation.
 * 
 * Values:
 * - `WisStaticBorderTransparentBlack = 0`: Transparent black border color.
 * - `WisStaticBorderOpaqueBlack = 1`: Opaque black border color.
 * - `WisStaticBorderOpaqueWhite = 2`: Opaque white border color.
 * \endcond
 *
 *
 * @section WisStaticBorder_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSamplerDesc
 * \endcond
 */