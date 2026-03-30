/**
 * @struct WisPushConstant
 * @ingroup Structures
 *
 *
 * @section WisPushConstant_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisPushConstant {
 *     WisShaderVisibility visibility;
 *     uint32_t            size_bytes;
 *     uint32_t            bind_register;
 *     uint32_t            bind_space;
 * } WisPushConstant;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PushConstant {
 *     wis::ShaderVisibility visibility;
 *     std::uint32_t         size_bytes;
 *     std::uint32_t         bind_register;
 *     std::uint32_t         bind_space;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPushConstant_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `visibility` defines shader stage visibility for the push constant.
 * - `size_bytes` describes size of the constant in bytes; it @wis_must be divisible by 4.
 * - `bind_register` describes bind register number in HLSL.
 * - `bind_space` defines bind space number in HLSL as `register(regN, spaceN)`.
 * \endcond
 *
 * @section WisPushConstant_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisPushConstant_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRootSignatureDesc
 * \endcond
 */