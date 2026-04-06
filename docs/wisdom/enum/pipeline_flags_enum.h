/**
 * @struct WisPipelineFlags WisPipelineFlags
 * @ingroup Enumerations Core
 *
 * @section WisPipelineFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisPipelineFlags {
 *     WisPipelineFlagsNone = 0,
 *     WisPipelineFlagsFailOnCacheMiss = (1u << 0),
 *     WisPipelineFlagsEnablePrimitiveRestart = (1u << 1),
 *     WisPipelineFlagsDynamicDepthBias = (1u << 2),
 * } WisPipelineFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class PipelineFlags : uint32_t {
 *     None = 0,
 *     FailOnCacheMiss = (1u << 0),
 *     EnablePrimitiveRestart = (1u << 1),
 *     DynamicDepthBias = (1u << 2),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPipelineFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Pipeline creation flags.
 *
 * Values:
 * - `WisPipelineFlagsNone = 0`: No flags set. Pipeline is regular.
 * - `WisPipelineFlagsFailOnCacheMiss = (1 << 0)`: Fail pipeline creation if the pipeline cache is missing or incompatible. If not set, the implementation @wis_may choose to create the pipeline without using the cache, which @wis_may result in longer creation time.
 * - `WisPipelineFlagsEnablePrimitiveRestart = (1 << 1)`: Enable primitive restart for graphics pipelines. If not set, primitive restart is disabled and the implementation @wis_may choose to ignore restart indices in draw calls.
 * - `WisPipelineFlagsDynamicDepthBias = (1 << 2)`: Enable dynamic depth bias for graphics pipelines. If not set, depth bias is static and @wis_must be specified at pipeline creation time.
 * \endcond
 *
 *
 * @section WisPipelineFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisComputePipelineDesc, WisGraphicsPipelineDesc
 * \endcond
 */