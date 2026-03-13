/**
 * @struct WisInputAttributeDesc
 * @ingroup Structures
 *
 *
 * @section WisInputAttributeDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisInputAttributeDesc {
 *     uint32_t      binding_index;
 *     const char*   semantic_name;
 *     uint32_t      semantic_index;
 *     uint32_t      location;
 *     WisDataFormat format;
 *     uint32_t      offset_bytes;
 * } WisInputAttributeDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  InputAttributeDesc {
 *     std::uint32_t   binding_index;
 *     const char*     semantic_name;
 *     std::uint32_t   semantic_index;
 *     std::uint32_t   location;
 *     wis::DataFormat format;
 *     std::uint32_t   offset_bytes;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisInputAttributeDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `binding_index` Index into an array of bindings that the attribute is tied to.
 * - `semantic_name` Semantic name of the attribute in HLSL. Must be unique and null terminated.
 * - `semantic_index` Semantic index of the attribute in HLSL. Must be unique.
 * - `location` Location of the attribute in HLSL. Must be unique.
 * - `format` Data format of the attribute.
 * - `offset_bytes` Offset in bytes from the beginning of the vertex.
 * \endcond
 *
 * @section WisInputAttributeDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisInputAttributeDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisInputLayout
 * \endcond
 */