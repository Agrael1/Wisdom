/**
 * @struct WisInputBindingDesc
 * @ingroup Structures Core
 *
 *
 * @section WisInputBindingDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisInputBindingDesc {
 *     uint32_t      slot;
 *     uint32_t      stride_bytes;
 *     WisInputClass input_class;
 * } WisInputBindingDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  InputBindingDesc {
 *     std::uint32_t   slot;
 *     std::uint32_t   stride_bytes;
 *     wis::InputClass input_class;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisInputBindingDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `slot` specifies input slot number; it @wis_must be unique.
 * - `stride_bytes` describes stride in bytes. Size of one vertex in the slot.
 * - `input_class` describes input class. Defines how the data is read (Per vertex or Per instance).
 * \endcond
 *
 * @section WisInputBindingDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisInputBindingDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisInputLayout
 * \endcond
 */
