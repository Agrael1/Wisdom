/**
 * @struct WisStdVideoH265ScalingLists
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265ScalingLists_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265ScalingLists {
 *     uint8_t ScalingList4x4[6*16];
 *     uint8_t ScalingList8x8[6*64];
 *     uint8_t ScalingList16x16[6*64];
 *     uint8_t ScalingList32x32[2*64];
 *     uint8_t ScalingListDCCoef16x16[6];
 *     uint8_t ScalingListDCCoef32x32[2];
 * } WisStdVideoH265ScalingLists;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265ScalingLists {
 *     std::array<std::uint8_t, 6*16> ScalingList4x4;
 *     std::array<std::uint8_t, 6*64> ScalingList8x8;
 *     std::array<std::uint8_t, 6*64> ScalingList16x16;
 *     std::array<std::uint8_t, 2*64> ScalingList32x32;
 *     std::array<std::uint8_t, 6> ScalingListDCCoef16x16;
 *     std::array<std::uint8_t, 2> ScalingListDCCoef32x32;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265ScalingLists_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `ScalingList4x4` 4x4 scaling list data (6 lists x 16 entries).
 * - `ScalingList8x8` 8x8 scaling list data (6 lists x 64 entries).
 * - `ScalingList16x16` 16x16 scaling list data (6 lists x 64 entries).
 * - `ScalingList32x32` 32x32 scaling list data (2 lists x 64 entries).
 * - `ScalingListDCCoef16x16` DC coefficients for 16x16 scaling lists.
 * - `ScalingListDCCoef32x32` DC coefficients for 32x32 scaling lists.
 * \endcond
 *
 * @section WisStdVideoH265ScalingLists_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265ScalingLists_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265SequenceParameterSet, WisStdVideoH265PictureParameterSet
 * \endcond
 */
