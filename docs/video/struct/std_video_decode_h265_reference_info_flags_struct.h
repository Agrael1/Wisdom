/**
 * @struct WisStdVideoDecodeH265ReferenceInfoFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoDecodeH265ReferenceInfoFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoDecodeH265ReferenceInfoFlags {
 *     uint32_t used_for_long_term_reference : 1;
 *     uint32_t unused_for_reference : 1;
 *     uint32_t reserved : 30;
 * } WisStdVideoDecodeH265ReferenceInfoFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoDecodeH265ReferenceInfoFlags {
 *     std::uint32_t used_for_long_term_reference : 1;
 *     std::uint32_t unused_for_reference : 1;
 *     std::uint32_t reserved : 30;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoDecodeH265ReferenceInfoFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `used_for_long_term_reference` Specifies that the reference is used for long-term reference.
 * - `unused_for_reference` Specifies that the reference is unused for reference.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoDecodeH265ReferenceInfoFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoDecodeH265ReferenceInfoFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeH265ReferenceInfo
 * \endcond
 */
