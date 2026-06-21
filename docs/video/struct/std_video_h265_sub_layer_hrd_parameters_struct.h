/**
 * @struct WisStdVideoH265SubLayerHrdParameters
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265SubLayerHrdParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265SubLayerHrdParameters {
 *     uint32_t bit_rate_value_minus1[32];
 *     uint32_t cpb_size_value_minus1[32];
 *     uint32_t cpb_size_du_value_minus1[32];
 *     uint32_t bit_rate_du_value_minus1[32];
 *     uint32_t cbr_flag;
 * } WisStdVideoH265SubLayerHrdParameters;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265SubLayerHrdParameters {
 *     std::array<std::uint32_t, 32> bit_rate_value_minus1;
 *     std::array<std::uint32_t, 32> cpb_size_value_minus1;
 *     std::array<std::uint32_t, 32> cpb_size_du_value_minus1;
 *     std::array<std::uint32_t, 32> bit_rate_du_value_minus1;
 *     std::uint32_t cbr_flag;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265SubLayerHrdParameters_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `bit_rate_value_minus1` Bit rate value minus 1 for each CPB.
 * - `cpb_size_value_minus1` CPB size value minus 1 for each CPB.
 * - `cpb_size_du_value_minus1` CPB size du value minus 1 for each CPB.
 * - `bit_rate_du_value_minus1` Bit rate du value minus 1 for each CPB.
 * - `cbr_flag` Specifies constant bit rate flag for each CPB.
 * \endcond
 *
 * @section WisStdVideoH265SubLayerHrdParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265SubLayerHrdParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265HrdParameters
 * \endcond
 */
