/**
 * @struct WisPipelineType WisPipelineType
 * @ingroup Enumerations
 *
 * @section WisPipelineType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisPipelineType {
 *     WisPipelineTypeGraphics = 0,
 *     WisPipelineTypeCompute = 1,
 *     WisPipelineTypeRayTracing = 2,
 * } WisPipelineType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class PipelineType {
 *     Graphics = 0,
 *     Compute = 1,
 *     RayTracing = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPipelineType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Defines types of pipelines that can be created on the device. Each type corresponds to a specific kind of workload that the GPU can handle.
 * 
 * \note Translates to  `VkPipelineBindPoint` for Vulkan implementation.
 * 
 * Values:
 * - `WisPipelineTypeGraphics = 0`: Pipeline for graphics operations, including rendering tasks.
 * - `WisPipelineTypeCompute = 1`: Pipeline dedicated to compute operations, optimized for parallel processing tasks.
 * - `WisPipelineTypeRayTracing = 2`: Pipeline for ray tracing operations, optimized for ray tracing workloads.
 * \endcond
 *
 *
 * @section WisPipelineType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisPushConstantDataDesc, WisPushDescriptorDataDesc, WisDescriptorTableDataDesc
 * Functions:
 * wisCommandListSetRootSignature, wisCommandListSetPipeline
 * \endcond
 */