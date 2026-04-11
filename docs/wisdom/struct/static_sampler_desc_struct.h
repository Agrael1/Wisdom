/**
 * @struct WisStaticSamplerDesc
 * @ingroup Structures Core
 *
 *
 * @section WisStaticSamplerDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisStaticSamplerDesc {
 *     WisSamplerDesc      sampler;
 *     WisShaderVisibility visibility;
 *     uint32_t            bind_register;
 * } WisStaticSamplerDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  StaticSamplerDesc {
 *     wis::SamplerDesc      sampler;
 *     wis::ShaderVisibility visibility;
 *     std::uint32_t         bind_register;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStaticSamplerDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `sampler` defines sampler description.
 * - `visibility` defines shader stage visibility; it defines the stage where the sampler is used.
 * - `bind_register` describes bind register number in HLSL.
 * \endcond
 *
 * @section WisStaticSamplerDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStaticSamplerDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
