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
 *     WisShaderVisibility visibility;
 *     WisDescriptorType   type;
 *     uint32_t            bind_register;
 *     uint32_t            bind_space;
 * } WisPushDescriptor;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PushDescriptor {
 *     wis::ShaderVisibility visibility;
 *     wis::DescriptorType   type;
 *     std::uint32_t         bind_register;
 *     std::uint32_t         bind_space;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPushDescriptor_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `visibility` Shader stage visibility for the push descriptor.
 * - `type` Descriptor type. Works only with buffer bindings.
 * - `bind_register` Bind register number in HLSL.
 * - `bind_space` Bind space number in HLSL. `register(regN, spaceN)`
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
 * @see WisRootSignatureDesc
 * \endcond
 */