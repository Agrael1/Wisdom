/**
 * @struct WisStdVideoAV1ColorConfig
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1ColorConfig_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1ColorConfig {
 *     WisStdVideoAV1ColorConfigFlags        flags;
 *     uint8_t                               BitDepth;
 *     uint8_t                               subsampling_x;
 *     uint8_t                               subsampling_y;
 *     uint8_t                               reserved1;
 *     WisStdVideoAV1ColorPrimaries          color_primaries;
 *     WisStdVideoAV1TransferCharacteristics transfer_characteristics;
 *     WisStdVideoAV1MatrixCoefficients      matrix_coefficients;
 *     WisStdVideoAV1ChromaSamplePosition    chroma_sample_position;
 * } WisStdVideoAV1ColorConfig;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1ColorConfig {
 *     wis::StdVideoAV1ColorConfigFlags        flags;
 *     std::uint8_t                            BitDepth;
 *     std::uint8_t                            subsampling_x;
 *     std::uint8_t                            subsampling_y;
 *     std::uint8_t                            reserved1;
 *     wis::StdVideoAV1ColorPrimaries          color_primaries;
 *     wis::StdVideoAV1TransferCharacteristics transfer_characteristics;
 *     wis::StdVideoAV1MatrixCoefficients      matrix_coefficients;
 *     wis::StdVideoAV1ChromaSamplePosition    chroma_sample_position;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1ColorConfig_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Color configuration flags.
 * - `BitDepth` Bit depth of the color samples (8, 10, or 12).
 * - `subsampling_x` Chroma subsampling x.
 * - `subsampling_y` Chroma subsampling y.
 * - `reserved1` No description.
 * - `color_primaries` Color primaries.
 * - `transfer_characteristics` Transfer characteristics.
 * - `matrix_coefficients` Matrix coefficients.
 * - `chroma_sample_position` Chroma sample position.
 * \endcond
 *
 * @section WisStdVideoAV1ColorConfig_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1ColorConfig_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1SequenceHeader
 * \endcond
 */
