/**
 * @struct WisInputSlotDesc
 * @ingroup Structures Core
 *
 *
 * @section WisInputSlotDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisInputSlotDesc {
 *     uint32_t      slot;
 *     uint32_t      stride_bytes;
 *     WisInputClass input_class;
 * } WisInputSlotDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  InputSlotDesc {
 *     std::uint32_t   slot;
 *     std::uint32_t   stride_bytes;
 *     wis::InputClass input_class;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisInputSlotDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `slot` Input slot number. Must be unique.
 * - `stride_bytes` Stride in bytes. Size of one vertex in the slot.
 * - `input_class` Input class. Defines how the data is read (Per vertex or Per instance).
 * \endcond
 *
 * @section WisInputSlotDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisInputSlotDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisInputLayout
 * \endcond
 */
