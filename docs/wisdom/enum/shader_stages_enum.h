/**
 * @struct WisShaderStages WisShaderStages
 * @ingroup Enumerations
 *
 * @section WisShaderStages_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisShaderStages {
 *     WisShaderStagesAll = 0,
 *     WisShaderStagesVertex = 1,
 *     WisShaderStagesHull = 2,
 *     WisShaderStagesDomain = 3,
 *     WisShaderStagesGeometry = 4,
 *     WisShaderStagesPixel = 5,
 *     WisShaderStagesAmplification = 6,
 *     WisShaderStagesMesh = 7,
 *     WisShaderStagesCount = 8,
 * } WisShaderStages;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class ShaderStages {
 *     All = 0,
 *     Vertex = 1,
 *     Hull = 2,
 *     Domain = 3,
 *     Geometry = 4,
 *     Pixel = 5,
 *     Amplification = 6,
 *     Mesh = 7,
 *     Count = 8,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisShaderStages_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Shader stages that can be used in the pipeline. Main use is Root signature and descriptor management. Stages have no granularity, either all or one can be selected.
 * 
 * \note Translates to  `D3D12_SHADER_VISIBILITY` for DirectX 12 implementation, and `VkShaderStageFlags` for Vulkan implementation.
 * 
 * Values:
 * - `WisShaderStagesAll = 0`: All shader stages.
 * - `WisShaderStagesVertex = 1`: Vertex shader stage.
 * - `WisShaderStagesHull = 2`: Hull/Tessellation control shader stage.
 * - `WisShaderStagesDomain = 3`: Domain/Tessellation evaluation shader stage.
 * - `WisShaderStagesGeometry = 4`: Geometry shader stage.
 * - `WisShaderStagesPixel = 5`: Pixel/Fragment shader stage.
 * - `WisShaderStagesAmplification = 6`: Amplification shader stage.
 * - `WisShaderStagesMesh = 7`: Mesh shader stage.
 * - `WisShaderStagesCount = 8`: Number of stages.
 * \endcond
 *
 *
 * @section WisShaderStages_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisStaticSamplerDesc, WisPushConstant, WisPushDescriptor
 * \endcond
 */