/**
 * @struct WisBlendFactor WisBlendFactor
 * @ingroup Enumerations
 *
 * @section WisBlendFactor_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisBlendFactor {
 *     WisBlendFactorZero = 1,
 *     WisBlendFactorOne = 2,
 *     WisBlendFactorSrcColor = 3,
 *     WisBlendFactorInvSrcColor = 4,
 *     WisBlendFactorSrcAlpha = 5,
 *     WisBlendFactorInvSrcAlpha = 6,
 *     WisBlendFactorDestAlpha = 7,
 *     WisBlendFactorInvDestAlpha = 8,
 *     WisBlendFactorDestColor = 9,
 *     WisBlendFactorInvDestColor = 10,
 *     WisBlendFactorSrcAlphaSat = 11,
 *     WisBlendFactorConstantColor = 14,
 *     WisBlendFactorInvBlendFactor = 15,
 *     WisBlendFactorSrc1Color = 16,
 *     WisBlendFactorInvSrc1Color = 17,
 *     WisBlendFactorSrc1Alpha = 18,
 *     WisBlendFactorInvSrc1Alpha = 19,
 * } WisBlendFactor;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class BlendFactor {
 *     Zero = 1,
 *     One = 2,
 *     SrcColor = 3,
 *     InvSrcColor = 4,
 *     SrcAlpha = 5,
 *     InvSrcAlpha = 6,
 *     DestAlpha = 7,
 *     InvDestAlpha = 8,
 *     DestColor = 9,
 *     InvDestColor = 10,
 *     SrcAlphaSat = 11,
 *     ConstantColor = 14,
 *     InvBlendFactor = 15,
 *     Src1Color = 16,
 *     InvSrc1Color = 17,
 *     Src1Alpha = 18,
 *     InvSrc1Alpha = 19,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBlendFactor_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Blend factor for color blending operations.
 * 
 * \note Translates to  `D3D12_BLEND` for DirectX 12 implementation, and `VkBlendFactor` for Vulkan implementation.
 * 
 * Values:
 * - `WisBlendFactorZero = 1`: Use zero for blending.
 * - `WisBlendFactorOne = 2`: Use one for blending.
 * - `WisBlendFactorSrcColor = 3`: Use the source color for blending.
 * - `WisBlendFactorInvSrcColor = 4`: Use the inverse source color for blending.
 * - `WisBlendFactorSrcAlpha = 5`: Use the source alpha for blending.
 * - `WisBlendFactorInvSrcAlpha = 6`: Use the inverse source alpha for blending.
 * - `WisBlendFactorDestAlpha = 7`: Use the destination alpha for blending.
 * - `WisBlendFactorInvDestAlpha = 8`: Use the inverse destination alpha for blending.
 * - `WisBlendFactorDestColor = 9`: Use the destination color for blending.
 * - `WisBlendFactorInvDestColor = 10`: Use the inverse destination color for blending.
 * - `WisBlendFactorSrcAlphaSat = 11`: Use the source alpha saturated for blending.
 * - `WisBlendFactorConstantColor = 14`: Use a constant blend factor for blending.
 * - `WisBlendFactorInvBlendFactor = 15`: Use the inverse constant blend factor for blending.
 * - `WisBlendFactorSrc1Color = 16`: Use the source color for blending. Dual source blending mode.
 * - `WisBlendFactorInvSrc1Color = 17`: Use the inverse source color for blending. Dual source blending mode.
 * - `WisBlendFactorSrc1Alpha = 18`: Use the source alpha for blending. Dual source blending mode.
 * - `WisBlendFactorInvSrc1Alpha = 19`: Use the inverse source alpha for blending. Dual source blending mode.
 * \endcond
 *
 *
 * @section WisBlendFactor_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBlendAttachmentDesc
 * \endcond
 */