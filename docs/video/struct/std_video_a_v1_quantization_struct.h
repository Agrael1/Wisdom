/**
 * @struct WisStdVideoAV1Quantization
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1Quantization_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1Quantization {
 *     WisStdVideoAV1QuantizationFlags flags;
 *     uint8_t                         base_q_idx;
 *     int8_t                          DeltaQYDc;
 *     int8_t                          DeltaQUDc;
 *     int8_t                          DeltaQUAc;
 *     int8_t                          DeltaQVDc;
 *     int8_t                          DeltaQVAc;
 *     uint8_t                         qm_y;
 *     uint8_t                         qm_u;
 *     uint8_t                         qm_v;
 * } WisStdVideoAV1Quantization;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1Quantization {
 *     wis::StdVideoAV1QuantizationFlags flags;
 *     std::uint8_t                      base_q_idx;
 *     std::int8_t                       DeltaQYDc;
 *     std::int8_t                       DeltaQUDc;
 *     std::int8_t                       DeltaQUAc;
 *     std::int8_t                       DeltaQVDc;
 *     std::int8_t                       DeltaQVAc;
 *     std::uint8_t                      qm_y;
 *     std::uint8_t                      qm_u;
 *     std::uint8_t                      qm_v;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1Quantization_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Quantization flags.
 * - `base_q_idx` Indicates the base frame qindex.
 * - `DeltaQYDc` Y DC quantizer relative to base_q_idx.
 * - `DeltaQUDc` U DC quantizer relative to base_q_idx.
 * - `DeltaQUAc` U AC quantizer relative to base_q_idx.
 * - `DeltaQVDc` V DC quantizer relative to base_q_idx.
 * - `DeltaQVAc` V AC quantizer relative to base_q_idx.
 * - `qm_y` Specifies the level in the quantizer matrix that @wis_should be used for luma plane decoding.
 * - `qm_u` Specifies the level in the quantizer matrix that @wis_should be used for chroma U plane decoding.
 * - `qm_v` Specifies the level in the quantizer matrix that @wis_should be used for chroma V plane decoding.
 * \endcond
 *
 * @section WisStdVideoAV1Quantization_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1Quantization_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
