/**
 * @struct WisGraphicsStages
 * @ingroup Structures Core
 *
 *
 * @section WisGraphicsStages_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisGraphicsStages {
 *     WisShaderView vertex_shader;
 *     WisShaderView hull_shader;
 *     WisShaderView domain_shader;
 *     WisShaderView geometry_shader;
 *     WisShaderView pixel_shader;
 * } WisGraphicsStages;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKGraphicsStages {
 *     WisVKShaderView vertex_shader;
 *     WisVKShaderView hull_shader;
 *     WisVKShaderView domain_shader;
 *     WisVKShaderView geometry_shader;
 *     WisVKShaderView pixel_shader;
 * } WisVKGraphicsStages;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12GraphicsStages {
 *     WisDX12ShaderView vertex_shader;
 *     WisDX12ShaderView hull_shader;
 *     WisDX12ShaderView domain_shader;
 *     WisDX12ShaderView geometry_shader;
 *     WisDX12ShaderView pixel_shader;
 * } WisDX12GraphicsStages;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct GraphicsStages 
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKGraphicsStages ;
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12GraphicsStages 
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section WisGraphicsStages_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `vertex_shader` Vertex shader bytecode.
 * - `hull_shader` Hull shader bytecode. If not set, the pipeline will be created without a hull shader.
 * - `domain_shader` Domain shader bytecode. If not set, the pipeline will be created without a domain shader.
 * - `geometry_shader` Geometry shader bytecode. If not set, the pipeline will be created without a geometry shader.
 * - `pixel_shader` Pixel shader bytecode. If not set, the pipeline will be created without a pixel shader.
 * 
 * \endcond
 *
 * @section WisGraphicsStages_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisGraphicsStages_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
