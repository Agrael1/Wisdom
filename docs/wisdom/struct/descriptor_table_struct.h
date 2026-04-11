/**
 * @struct WisDescriptorTable
 * @ingroup Structures Core
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
 *     WisShaderVisibility            visibility;
 *     const WisDescriptorTableEntry* entries;
 *     size_t                         entry_count;
 * } WisDescriptorTable;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  DescriptorTable {
 *     wis::ShaderVisibility                      visibility;
 *     wis::span<const wis::DescriptorTableEntry> entries;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorTable_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `visibility` defines shader stage. Defines the stage where the table is used.
 * - `entries` points to an array of descriptor table entries.
 * - `entry_count` specifies descriptor table entries count.
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
 * @see Structs:
 * WisRootSignatureDesc
 * \endcond
 */
