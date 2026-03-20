/**
 * @struct WisBlendStateDesc
 * @ingroup Structures
 *
 *
 * @section WisBlendStateDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisBlendStateDesc {
 *     bool                   logic_op_enable;
 *     WisLogicOp             logic_op;
 *     WisBlendAttachmentDesc attachments[8];
 *     uint32_t               attachment_count;
 * } WisBlendStateDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  BlendStateDesc {
 *     bool                     logic_op_enable;
 *     wis::LogicOp             logic_op;
 *     std::array<wis::BlendAttachmentDesc, 8> attachments;
 *     std::uint32_t            attachment_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBlendStateDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `logic_op_enable` Logic operation enable. Default is false.
 * - `logic_op` Logic operation. Default is `WisLogicOpNoop`.
 * - `attachments` Blend attachment descriptions. Max Array size is 8.
 * - `attachment_count` Blend attachment count. If set as 0 - broadcast to all attachments. Max is 8.
 * \endcond
 *
 * @section WisBlendStateDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBlendStateDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisGraphicsPipelineDesc
 * \endcond
 */