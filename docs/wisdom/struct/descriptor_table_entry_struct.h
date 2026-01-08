/**
 * @struct WisDescriptorTableEntry
 * @ingroup Structures
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
 *     uint32_t          count;
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
 *     std::uint32_t       count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorTableEntry_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` Descriptor type.
 * - `bind_register` Bind register number in HLSL.
 * - `count` Descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0 means single register, same as 1.
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
 * @see WisDescriptorTable
 * \endcond
 */