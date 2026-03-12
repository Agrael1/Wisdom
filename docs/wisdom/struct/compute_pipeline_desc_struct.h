/**
 * @struct WisComputePipelineDesc
 * @ingroup Structures
 *
 *
 * @section WisComputePipelineDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisComputePipelineDesc {
 *     WisRootSignatureView root_signature;
 *     WisShaderView        compute_shader;
 *     WisPipelineCacheView cache;
 *     WisPipelineFlags     flags;
 * } WisComputePipelineDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKComputePipelineDesc {
 *     WisVKRootSignatureView root_signature;
 *     WisVKShaderView        compute_shader;
 *     WisVKPipelineCacheView cache;
 *     WisPipelineFlags       flags;
 * } WisVKComputePipelineDesc;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12ComputePipelineDesc {
 *     WisDX12RootSignatureView root_signature;
 *     WisDX12ShaderView        compute_shader;
 *     WisDX12PipelineCacheView cache;
 *     WisPipelineFlags         flags;
 * } WisDX12ComputePipelineDesc;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct ComputePipelineDesc {
 *     wis::RootSignatureView root_signature;
 *     wis::ShaderView        compute_shader;
 *     wis::PipelineCacheView cache;
 *     wis::PipelineFlags     flags;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKComputePipelineDesc {
 *     wis::VKRootSignatureView root_signature;
 *     wis::VKShaderView        compute_shader;
 *     wis::VKPipelineCacheView cache;
 *     wis::PipelineFlags       flags;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12ComputePipelineDesc {
 *     wis::DX12RootSignatureView root_signature;
 *     wis::DX12ShaderView        compute_shader;
 *     wis::DX12PipelineCacheView cache;
 *     wis::PipelineFlags         flags;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisComputePipelineDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `root_signature` Root signature description for the pipeline.
 * - `compute_shader` Compute shader bytecode.
 * - `cache` Pipeline cache data. Used to speed up pipeline creation if available.
 * - `flags` Pipeline flags. Describe additional options for the pipeline.
 * \endcond
 *
 * @section WisComputePipelineDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisComputePipelineDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisDeviceCreateComputePipeline
 * \endcond
 */