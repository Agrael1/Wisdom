/**
 * @struct WisStdVideoH265ChromaFormatIdc WisStdVideoH265ChromaFormatIdc
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoH265ChromaFormatIdc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoH265ChromaFormatIdc {
 *     WisStdVideoH265ChromaFormatIdcMonochrome = 0,
 *     WisStdVideoH265ChromaFormatIdcCr420 = 1,
 *     WisStdVideoH265ChromaFormatIdcCr422 = 2,
 *     WisStdVideoH265ChromaFormatIdcCr444 = 3,
 *     WisStdVideoH265ChromaFormatIdcInvalid = 0x7FFFFFFF,
 * } WisStdVideoH265ChromaFormatIdc;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoH265ChromaFormatIdc {
 *     Monochrome = 0,
 *     Cr420 = 1,
 *     Cr422 = 2,
 *     Cr444 = 3,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265ChromaFormatIdc_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * H.265 chroma format identifiers as defined in HEVC spec Table 6-1.
 *
 * Values:
 * - `WisStdVideoH265ChromaFormatIdcMonochrome = 0`: Monochrome (4:0:0).
 * - `WisStdVideoH265ChromaFormatIdcCr420 = 1`: 4:2:0 chroma format.
 * - `WisStdVideoH265ChromaFormatIdcCr422 = 2`: 4:2:2 chroma format.
 * - `WisStdVideoH265ChromaFormatIdcCr444 = 3`: 4:4:4 chroma format.
 * - `WisStdVideoH265ChromaFormatIdcInvalid = 0x7FFFFFFF`: Invalid chroma format.
 * \endcond
 *
 *
 * @section WisStdVideoH265ChromaFormatIdc_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265SequenceParameterSet
 * \endcond
 */
