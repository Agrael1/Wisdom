/**
 * @struct WisStdVideoAV1MatrixCoefficients WisStdVideoAV1MatrixCoefficients
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1MatrixCoefficients_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1MatrixCoefficients {
 *     WisStdVideoAV1MatrixCoefficientsIdentity = 0,
 *     WisStdVideoAV1MatrixCoefficientsBt709 = 1,
 *     WisStdVideoAV1MatrixCoefficientsUnspecified = 2,
 *     WisStdVideoAV1MatrixCoefficientsReserved3 = 3,
 *     WisStdVideoAV1MatrixCoefficientsFcc = 4,
 *     WisStdVideoAV1MatrixCoefficientsBt470BG = 5,
 *     WisStdVideoAV1MatrixCoefficientsBt601 = 6,
 *     WisStdVideoAV1MatrixCoefficientsSmpte240 = 7,
 *     WisStdVideoAV1MatrixCoefficientsSmpteYcgco = 8,
 *     WisStdVideoAV1MatrixCoefficientsBt2020Ncl = 9,
 *     WisStdVideoAV1MatrixCoefficientsBt2020Cl = 10,
 *     WisStdVideoAV1MatrixCoefficientsSmpte2085 = 11,
 *     WisStdVideoAV1MatrixCoefficientsChromatNcl = 12,
 *     WisStdVideoAV1MatrixCoefficientsChromatCl = 13,
 *     WisStdVideoAV1MatrixCoefficientsIctcp = 14,
 *     WisStdVideoAV1MatrixCoefficientsInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1MatrixCoefficients;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1MatrixCoefficients {
 *     Identity = 0,
 *     Bt709 = 1,
 *     Unspecified = 2,
 *     Reserved3 = 3,
 *     Fcc = 4,
 *     Bt470BG = 5,
 *     Bt601 = 6,
 *     Smpte240 = 7,
 *     SmpteYcgco = 8,
 *     Bt2020Ncl = 9,
 *     Bt2020Cl = 10,
 *     Smpte2085 = 11,
 *     ChromatNcl = 12,
 *     ChromatCl = 13,
 *     Ictcp = 14,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1MatrixCoefficients_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * AV1 matrix coefficients mapping to CICP.
 *
 * Values:
 * - `WisStdVideoAV1MatrixCoefficientsIdentity = 0`: Identity matrix.
 * - `WisStdVideoAV1MatrixCoefficientsBt709 = 1`: Rec. ITU-R BT.709-6.
 * - `WisStdVideoAV1MatrixCoefficientsUnspecified = 2`: Matrix characteristics are unspecified.
 * - `WisStdVideoAV1MatrixCoefficientsReserved3 = 3`:
 * - `WisStdVideoAV1MatrixCoefficientsFcc = 4`: FCC Title 47 Code of Federal Regulations.
 * - `WisStdVideoAV1MatrixCoefficientsBt470BG = 5`: Rec. ITU-R BT.470-6 System B, G (historical).
 * - `WisStdVideoAV1MatrixCoefficientsBt601 = 6`: Rec. ITU-R BT.601-7.
 * - `WisStdVideoAV1MatrixCoefficientsSmpte240 = 7`: SMPTE 240M.
 * - `WisStdVideoAV1MatrixCoefficientsSmpteYcgco = 8`: YCgCo.
 * - `WisStdVideoAV1MatrixCoefficientsBt2020Ncl = 9`: Bt2020 non-constant luminance.
 * - `WisStdVideoAV1MatrixCoefficientsBt2020Cl = 10`: Bt2020 constant luminance.
 * - `WisStdVideoAV1MatrixCoefficientsSmpte2085 = 11`: SMPTE ST 2085.
 * - `WisStdVideoAV1MatrixCoefficientsChromatNcl = 12`: Chromaticity-derived non-constant luminance.
 * - `WisStdVideoAV1MatrixCoefficientsChromatCl = 13`: Chromaticity-derived constant luminance.
 * - `WisStdVideoAV1MatrixCoefficientsIctcp = 14`: Rec. ITU-R BT.2100-0 ICtCp.
 * - `WisStdVideoAV1MatrixCoefficientsInvalid = 0x7FFFFFFF`:
 * \endcond
 *
 *
 * @section WisStdVideoAV1MatrixCoefficients_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1ColorConfig
 * \endcond
 */
