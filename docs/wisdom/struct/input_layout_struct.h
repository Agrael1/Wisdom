/**
 * @struct WisInputLayout
 * @ingroup Structures
 *
 *
 * @section WisInputLayout_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisInputLayout {
 *     const WisInputBindingDesc*   bindings;
 *     size_t                       binding_count;
 *     const WisInputAttributeDesc* attributes;
 *     size_t                       attribute_count;
 * } WisInputLayout;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  InputLayout {
 *     wis::span<const wis::InputBindingDesc>   bindings;
 *     wis::span<const wis::InputAttributeDesc> attributes;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisInputLayout_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `bindings` points to an array of input slots used to pick data from multiple vertex arrays.
 * - `binding_count` indicates input slots count. Max number is 16.
 * - `attributes` points to an array of input attributes that describes how vertex data is read by the HLSL shader.
 * - `attribute_count` defines input attributes count.
 * \endcond
 *
 * @section WisInputLayout_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisInputLayout_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisGraphicsPipelineDesc
 * \endcond
 */