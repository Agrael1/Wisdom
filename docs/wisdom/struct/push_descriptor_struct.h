/**
 * @struct WisPushDescriptor
 * @ingroup Structures
 *
 *
 * @section WisPushDescriptor_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisPushDescriptor {
 *     WisShaderStages   stage;
 *     WisDescriptorType type;
 * } WisPushDescriptor;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PushDescriptor {
 *     wis::ShaderStages   stage;
 *     wis::DescriptorType type;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPushDescriptor_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `stage` Shader stage. Defines the stage where the descriptor is used.
 * - `type` Descriptor type. Works only with buffer bindings.
 * \endcond
 *
 * @section WisPushDescriptor_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisPushDescriptor_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisPipelineLayoutDesc
 * \endcond
 */