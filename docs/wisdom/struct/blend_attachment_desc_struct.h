/**
 * @struct WisBlendAttachmentDesc
 * @ingroup Structures Core
 *
 *
 * @section WisBlendAttachmentDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisBlendAttachmentDesc {
 *     bool               blend_enable;
 *     WisBlendFactor     src_color_blend;
 *     WisBlendFactor     dst_color_blend;
 *     WisBlendOp         color_blend_op;
 *     WisBlendFactor     src_alpha_blend;
 *     WisBlendFactor     dst_alpha_blend;
 *     WisBlendOp         alpha_blend_op;
 *     WisColorComponents color_write_mask;
 * } WisBlendAttachmentDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  BlendAttachmentDesc {
 *     bool                 blend_enable;
 *     wis::BlendFactor     src_color_blend;
 *     wis::BlendFactor     dst_color_blend;
 *     wis::BlendOp         color_blend_op;
 *     wis::BlendFactor     src_alpha_blend;
 *     wis::BlendFactor     dst_alpha_blend;
 *     wis::BlendOp         alpha_blend_op;
 *     wis::ColorComponents color_write_mask;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBlendAttachmentDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `blend_enable` indicates blend enable. Default is false.
 * - `src_color_blend` specifies source color blend factor. Default is `WisBlendFactorOne`.
 * - `dst_color_blend` describes destination color blend factor. Default is `WisBlendFactorZero`.
 * - `color_blend_op` describes color blend operation. Default is `WisBlendOpAdd`.
 * - `src_alpha_blend` describes source alpha blend factor. Default is `WisBlendFactorOne`.
 * - `dst_alpha_blend` defines destination alpha blend factor. Default is `WisBlendFactorZero`.
 * - `alpha_blend_op` defines alpha blend operation. Default is `WisBlendOpAdd`.
 * - `color_write_mask` defines color write mask. Default is `WisColorComponentsAll`.
 * \endcond
 *
 * @section WisBlendAttachmentDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBlendAttachmentDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBlendStateDesc
 * \endcond
 */
