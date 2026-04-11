/**
 * @struct WisGraphicsPipelineDesc
 * @ingroup Structures Core
 *
 *
 * @section WisGraphicsPipelineDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct WisGraphicsPipelineDesc {
 *     WisRootSignatureView       root_signature;
 *     WisShaderView              vertex_shader;
 *     WisShaderView              hull_shader;
 *     WisShaderView              domain_shader;
 *     WisShaderView              geometry_shader;
 *     WisShaderView              pixel_shader;
 *     WisRenderAttachmentsDesc   render_attachments;
 *     WisInputLayout             input_layout;
 *     WisTopologyType            topology_type;
 *     const WisRasterizerDesc*   rasterizer_desc;
 *     const WisSampleDesc*       sample_desc;
 *     const WisDepthStencilDesc* depth_stencil_desc;
 *     const WisBlendStateDesc*   blend_state_desc;
 *     WisPipelineCacheView       cache;
 *     WisPipelineFlags           flags;
 * } WisGraphicsPipelineDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct WisVKGraphicsPipelineDesc {
 *     WisVKRootSignatureView     root_signature;
 *     WisVKShaderView            vertex_shader;
 *     WisVKShaderView            hull_shader;
 *     WisVKShaderView            domain_shader;
 *     WisVKShaderView            geometry_shader;
 *     WisVKShaderView            pixel_shader;
 *     WisRenderAttachmentsDesc   render_attachments;
 *     WisInputLayout             input_layout;
 *     WisTopologyType            topology_type;
 *     const WisRasterizerDesc*   rasterizer_desc;
 *     const WisSampleDesc*       sample_desc;
 *     const WisDepthStencilDesc* depth_stencil_desc;
 *     const WisBlendStateDesc*   blend_state_desc;
 *     WisVKPipelineCacheView     cache;
 *     WisPipelineFlags           flags;
 * } WisVKGraphicsPipelineDesc;
 *
 * // Provided by Wisdom 0.7.0.
 * typedef struct WisDX12GraphicsPipelineDesc {
 *     WisDX12RootSignatureView   root_signature;
 *     WisDX12ShaderView          vertex_shader;
 *     WisDX12ShaderView          hull_shader;
 *     WisDX12ShaderView          domain_shader;
 *     WisDX12ShaderView          geometry_shader;
 *     WisDX12ShaderView          pixel_shader;
 *     WisRenderAttachmentsDesc   render_attachments;
 *     WisInputLayout             input_layout;
 *     WisTopologyType            topology_type;
 *     const WisRasterizerDesc*   rasterizer_desc;
 *     const WisSampleDesc*       sample_desc;
 *     const WisDepthStencilDesc* depth_stencil_desc;
 *     const WisBlendStateDesc*   blend_state_desc;
 *     WisDX12PipelineCacheView   cache;
 *     WisPipelineFlags           flags;
 * } WisDX12GraphicsPipelineDesc;
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct GraphicsPipelineDesc {
 *     wis::RootSignatureView       root_signature;
 *     wis::ShaderView              vertex_shader;
 *     wis::ShaderView              hull_shader;
 *     wis::ShaderView              domain_shader;
 *     wis::ShaderView              geometry_shader;
 *     wis::ShaderView              pixel_shader;
 *     wis::RenderAttachmentsDesc   render_attachments;
 *     wis::InputLayout             input_layout;
 *     wis::TopologyType            topology_type;
 *     const wis::RasterizerDesc*   rasterizer_desc;
 *     const wis::SampleDesc*       sample_desc;
 *     const wis::DepthStencilDesc* depth_stencil_desc;
 *     const wis::BlendStateDesc*   blend_state_desc;
 *     wis::PipelineCacheView       cache;
 *     wis::PipelineFlags           flags;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct VKGraphicsPipelineDesc {
 *     wis::VKRootSignatureView     root_signature;
 *     wis::VKShaderView            vertex_shader;
 *     wis::VKShaderView            hull_shader;
 *     wis::VKShaderView            domain_shader;
 *     wis::VKShaderView            geometry_shader;
 *     wis::VKShaderView            pixel_shader;
 *     wis::RenderAttachmentsDesc   render_attachments;
 *     wis::InputLayout             input_layout;
 *     wis::TopologyType            topology_type;
 *     const wis::RasterizerDesc*   rasterizer_desc;
 *     const wis::SampleDesc*       sample_desc;
 *     const wis::DepthStencilDesc* depth_stencil_desc;
 *     const wis::BlendStateDesc*   blend_state_desc;
 *     wis::VKPipelineCacheView     cache;
 *     wis::PipelineFlags           flags;
 * };
 *
 * // Provided by Wisdom 0.7.0.
 * struct DX12GraphicsPipelineDesc {
 *     wis::DX12RootSignatureView   root_signature;
 *     wis::DX12ShaderView          vertex_shader;
 *     wis::DX12ShaderView          hull_shader;
 *     wis::DX12ShaderView          domain_shader;
 *     wis::DX12ShaderView          geometry_shader;
 *     wis::DX12ShaderView          pixel_shader;
 *     wis::RenderAttachmentsDesc   render_attachments;
 *     wis::InputLayout             input_layout;
 *     wis::TopologyType            topology_type;
 *     const wis::RasterizerDesc*   rasterizer_desc;
 *     const wis::SampleDesc*       sample_desc;
 *     const wis::DepthStencilDesc* depth_stencil_desc;
 *     const wis::BlendStateDesc*   blend_state_desc;
 *     wis::DX12PipelineCacheView   cache;
 *     wis::PipelineFlags           flags;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisGraphicsPipelineDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `root_signature` describes root signature description for the pipeline.
 * - `vertex_shader` defines vertex shader bytecode.
 * - `hull_shader` indicates hull shader bytecode. If not set, the pipeline will be created without a hull shader.
 * - `domain_shader` specifies domain shader bytecode. If not set, the pipeline will be created without a domain shader.
 * - `geometry_shader` specifies geometry shader bytecode. If not set, the pipeline will be created without a geometry
 * shader.
 * - `pixel_shader` defines pixel shader bytecode. If not set, the pipeline will be created without a pixel shader.
 * - `render_attachments` indicates render attachments description for the pipeline. Used to create the compatible
 * render pass for the pipeline.
 * - `input_layout` describes input layout description for the pipeline. If not set, the pipeline will be created
 * without an input layout.
 * - `topology_type` specifies topology type. Default is `WisTopologyTypeTriangle`.
 * - `rasterizer_desc` describes rasterizer description for the pipeline. If not set, the pipeline will be created with
 * default rasterizer state.
 * - `sample_desc` defines sample description for the pipeline. If not set, the pipeline will be created with default
 * sample state (no multisampling).
 * - `depth_stencil_desc` defines depth stencil description for the pipeline. If not set, the pipeline will be created
 * with depth testing and stencil testing disabled.
 * - `blend_state_desc` indicates blend state description for the pipeline. If not set, the pipeline will be created
 * with blending disabled.
 * - `cache` defines pipeline cache data. Used to speed up pipeline creation if available.
 * - `flags` describes pipeline flags. Describe additional options for the pipeline.
 * \endcond
 *
 * @section WisGraphicsPipelineDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisGraphicsPipelineDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceCreateGraphicsPipeline
 * \endcond
 */
