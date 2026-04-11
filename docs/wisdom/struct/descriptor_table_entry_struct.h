/**
 * @struct WisDescriptorTableEntry
 * @ingroup Structures Core
 *
 *
 * @section WisDescriptorTableEntry_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisDescriptorTableEntry {
 *     WisDescriptorType type;
 *     uint32_t          bind_register;
 *     uint32_t          bind_space;
 *     uint32_t          count;
 *     uint32_t          descriptor_offset;
 * } WisDescriptorTableEntry;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  DescriptorTableEntry {
 *     wis::DescriptorType type;
 *     std::uint32_t       bind_register;
 *     std::uint32_t       bind_space;
 *     std::uint32_t       count;
 *     std::uint32_t       descriptor_offset;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorTableEntry_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` specifies descriptor type.
 * - `bind_register` describes bind register number in HLSL.
 * - `bind_space` defines bind space number in HLSL. `register(regN, spaceN)`
 * - `count` describes descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0 means single register, same as 1.
 * - `descriptor_offset` describes offset in descriptors from the heap start. Used for calculating descriptor indices when binding descriptor tables.
 * \endcond
 *
 * @section WisDescriptorTableEntry_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDescriptorTableEntry_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisDescriptorTable
 * \endcond
 */
