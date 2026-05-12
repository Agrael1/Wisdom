/**
 * @struct WisStdVideoAV1CDEF
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1CDEF_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1CDEF {
 *     uint8_t cdef_damping_minus_3;
 *     uint8_t cdef_bits;
 *     uint8_t cdef_y_pri_strength[8];
 *     uint8_t cdef_y_sec_strength[8];
 *     uint8_t cdef_uv_pri_strength[8];
 *     uint8_t cdef_uv_sec_strength[8];
 * } WisStdVideoAV1CDEF;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1CDEF {
 *     std::uint8_t cdef_damping_minus_3;
 *     std::uint8_t cdef_bits;
 *     std::array<std::uint8_t, 8> cdef_y_pri_strength;
 *     std::array<std::uint8_t, 8> cdef_y_sec_strength;
 *     std::array<std::uint8_t, 8> cdef_uv_pri_strength;
 *     std::array<std::uint8_t, 8> cdef_uv_sec_strength;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1CDEF_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `cdef_damping_minus_3` Controls the amount of damping in the deringing filter.
 * - `cdef_bits` Specifies the number of bits needed to specify the CDEF filter strength.
 * - `cdef_y_pri_strength` Primary filter strength for Y.
 * - `cdef_y_sec_strength` Secondary filter strength for Y.
 * - `cdef_uv_pri_strength` Primary filter strength for UV.
 * - `cdef_uv_sec_strength` Secondary filter strength for UV.
 * \endcond
 *
 * @section WisStdVideoAV1CDEF_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1CDEF_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
