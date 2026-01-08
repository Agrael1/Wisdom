/**
 * @struct WisDescriptorTable
 * @ingroup Structures
 *
 *
 * @section WisDescriptorTable_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDescriptorTable {
 *     WisDescriptorHeapType          type;
 *     WisShaderStages                stage;
 *     const WisDescriptorTableEntry* entries;
 *     size_t                         entry_count;
 *     uint32_t                       space_overlap;
 * } WisDescriptorTable;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DescriptorTable {
 *     wis::DescriptorHeapType                    type;
 *     wis::ShaderStages                          stage;
 *     wis::span<const wis::DescriptorTableEntry> entries;
 *     std::uint32_t                              space_overlap;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorTable_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` Descriptor heap type. Either Descriptor or Sampler.
 * - `stage` Shader stage. Defines the stage where the table is used.
 * - `entries` Descriptor table entries array.
 * - `entry_count` Descriptor table entries count.
 * - `space_overlap` If this value is not zero, bindings from this table can be bound several times in different spaces. Used for descriptor indexing into unbounded arrays of similar types.
 * \endcond
 *
 * @section WisDescriptorTable_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDescriptorTable_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisPipelineLayoutDesc
 * \endcond
 */