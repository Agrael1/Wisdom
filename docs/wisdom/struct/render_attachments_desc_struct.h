/**
 * @struct WisRenderAttachmentsDesc
 * @ingroup Structures Core
 *
 *
 * @section WisRenderAttachmentsDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisRenderAttachmentsDesc {
 *     WisDataFormat attachment_formats[8];
 *     uint32_t      attachments_count;
 *     WisDataFormat depth_attachment;
 *     uint32_t      view_mask;
 * } WisRenderAttachmentsDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  RenderAttachmentsDesc {
 *     std::array<wis::DataFormat, 8> attachment_formats;
 *     std::uint32_t   attachments_count;
 *     wis::DataFormat depth_attachment;
 *     std::uint32_t   view_mask;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderAttachmentsDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `attachment_formats` defines attachment formats array. Describes the format of the render target.
 * - `attachments_count` defines attachment formats count. Max is 8.
 * - `depth_attachment` describes depth attachment format. Describes the format of the depth buffer.
 * - `view_mask` specifies view mask for multiview rendering. Each bit represents a view that can be rendered to with the pipeline. Default is 0, meaning no multiview support.
 * \endcond
 *
 * @section WisRenderAttachmentsDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRenderAttachmentsDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisGraphicsPipelineDesc
 * \endcond
 */
