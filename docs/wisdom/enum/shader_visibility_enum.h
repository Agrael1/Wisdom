/**
 * @struct WisShaderVisibility WisShaderVisibility
 * @ingroup Enumerations Core
 *
 * @section WisShaderVisibility_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisShaderVisibility {
 *     WisShaderVisibilityAll = 0,
 *     WisShaderVisibilityVertex = 1,
 *     WisShaderVisibilityHull = 2,
 *     WisShaderVisibilityDomain = 3,
 *     WisShaderVisibilityGeometry = 4,
 *     WisShaderVisibilityPixel = 5,
 *     WisShaderVisibilityAmplification = 6,
 *     WisShaderVisibilityMesh = 7,
 *     WisShaderVisibilityCount = 8,
 * } WisShaderVisibility;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class ShaderVisibility {
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
 * @section WisShaderVisibility_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Shader stages that can be used in the pipeline. Main use is Root signature and descriptor management. Stages have no granularity, either all or one can be selected.
 * 
 * \note Translates to  `D3D12_SHADER_VISIBILITY` for DirectX 12 implementation, and `VkShaderStageFlags` for Vulkan implementation.
 * 
 * Values:
 * - `WisShaderVisibilityAll = 0`: All shader stages.
 * - `WisShaderVisibilityVertex = 1`: Vertex shader stage.
 * - `WisShaderVisibilityHull = 2`: Hull/Tessellation control shader stage.
 * - `WisShaderVisibilityDomain = 3`: Domain/Tessellation evaluation shader stage.
 * - `WisShaderVisibilityGeometry = 4`: Geometry shader stage.
 * - `WisShaderVisibilityPixel = 5`: Pixel/Fragment shader stage.
 * - `WisShaderVisibilityAmplification = 6`: Amplification shader stage.
 * - `WisShaderVisibilityMesh = 7`: Mesh shader stage.
 * - `WisShaderVisibilityCount = 8`: Number of stages.
 * \endcond
 *
 *
 * @section WisShaderVisibility_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStaticSamplerDesc, WisPushConstant, WisPushDescriptor, WisDescriptorTable
 * \endcond
 */
