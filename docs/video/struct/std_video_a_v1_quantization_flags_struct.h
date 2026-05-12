/**
 * @struct WisStdVideoAV1QuantizationFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1QuantizationFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1QuantizationFlags {
 *     uint32_t using_qmatrix : 1;
 *     uint32_t diff_uv_delta : 1;
 *     uint32_t reserved : 30;
 * } WisStdVideoAV1QuantizationFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1QuantizationFlags {
 *     std::uint32_t using_qmatrix : 1;
 *     std::uint32_t diff_uv_delta : 1;
 *     std::uint32_t reserved : 30;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1QuantizationFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `using_qmatrix` Specifies whether the quantizer matrix @wis_should be used.
 * - `diff_uv_delta` Specifies whether the U and V delta quantizer values are transmitted separately.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoAV1QuantizationFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1QuantizationFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1Quantization
 * \endcond
 */
