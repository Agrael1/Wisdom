/**
 * @struct WisStdVideoAV1FilmGrain
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1FilmGrain_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1FilmGrain {
 *     WisStdVideoAV1FilmGrainFlags flags;
 *     uint8_t                      grain_scaling_minus_8;
 *     uint8_t                      ar_coeff_lag;
 *     uint8_t                      ar_coeff_shift_minus_6;
 *     uint8_t                      grain_scale_shift;
 *     uint16_t                     grain_seed;
 *     uint8_t                      film_grain_params_ref_idx;
 *     uint8_t                      num_y_points;
 *     uint8_t                      point_y_value[14];
 *     uint8_t                      point_y_scaling[14];
 *     uint8_t                      num_cb_points;
 *     uint8_t                      point_cb_value[10];
 *     uint8_t                      point_cb_scaling[10];
 *     uint8_t                      num_cr_points;
 *     uint8_t                      point_cr_value[10];
 *     uint8_t                      point_cr_scaling[10];
 *     int8_t                       ar_coeffs_y_plus_128[24];
 *     int8_t                       ar_coeffs_cb_plus_128[25];
 *     int8_t                       ar_coeffs_cr_plus_128[25];
 *     uint8_t                      cb_mult;
 *     uint8_t                      cb_luma_mult;
 *     uint16_t                     cb_offset;
 *     uint8_t                      cr_mult;
 *     uint8_t                      cr_luma_mult;
 *     uint16_t                     cr_offset;
 * } WisStdVideoAV1FilmGrain;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1FilmGrain {
 *     wis::StdVideoAV1FilmGrainFlags flags;
 *     std::uint8_t                   grain_scaling_minus_8;
 *     std::uint8_t                   ar_coeff_lag;
 *     std::uint8_t                   ar_coeff_shift_minus_6;
 *     std::uint8_t                   grain_scale_shift;
 *     std::uint16_t                  grain_seed;
 *     std::uint8_t                   film_grain_params_ref_idx;
 *     std::uint8_t                   num_y_points;
 *     std::array<std::uint8_t, 14>   point_y_value;
 *     std::array<std::uint8_t, 14>   point_y_scaling;
 *     std::uint8_t                   num_cb_points;
 *     std::array<std::uint8_t, 10>   point_cb_value;
 *     std::array<std::uint8_t, 10>   point_cb_scaling;
 *     std::uint8_t                   num_cr_points;
 *     std::array<std::uint8_t, 10>   point_cr_value;
 *     std::array<std::uint8_t, 10>   point_cr_scaling;
 *     std::array<std::int8_t, 24>    ar_coeffs_y_plus_128;
 *     std::array<std::int8_t, 25>    ar_coeffs_cb_plus_128;
 *     std::array<std::int8_t, 25>    ar_coeffs_cr_plus_128;
 *     std::uint8_t                   cb_mult;
 *     std::uint8_t                   cb_luma_mult;
 *     std::uint16_t                  cb_offset;
 *     std::uint8_t                   cr_mult;
 *     std::uint8_t                   cr_luma_mult;
 *     std::uint16_t                  cr_offset;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1FilmGrain_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Film grain flags.
 * - `grain_scaling_minus_8` Shift value for the film grain scale calculation.
 * - `ar_coeff_lag` Number of auto-regressive coefficients.
 * - `ar_coeff_shift_minus_6` Shift value for auto-regressive coefficients.
 * - `grain_scale_shift` Specifies how much the Gaussian random numbers @wis_should be scaled down.
 * - `grain_seed` Specifies the seed for the pseudo-random number generator.
 * - `film_grain_params_ref_idx` Specifies the reference frame index to obtain the film grain parameters from.
 * - `num_y_points` Number of points for luma scaling.
 * - `point_y_value` Luma point values.
 * - `point_y_scaling` Luma point scaling.
 * - `num_cb_points` Number of points for Cb scaling.
 * - `point_cb_value` Cb point values.
 * - `point_cb_scaling` Cb point scaling.
 * - `num_cr_points` Number of points for Cr scaling.
 * - `point_cr_value` Cr point values.
 * - `point_cr_scaling` Cr point scaling.
 * - `ar_coeffs_y_plus_128` Auto-regressive coefficients for Y.
 * - `ar_coeffs_cb_plus_128` Auto-regressive coefficients for Cb.
 * - `ar_coeffs_cr_plus_128` Auto-regressive coefficients for Cr.
 * - `cb_mult` Cb multiplier for chroma scaling from luma.
 * - `cb_luma_mult` Cb luma multiplier for chroma scaling from luma.
 * - `cb_offset` Cb offset for chroma scaling from luma.
 * - `cr_mult` Cr multiplier for chroma scaling from luma.
 * - `cr_luma_mult` Cr luma multiplier for chroma scaling from luma.
 * - `cr_offset` Cr offset for chroma scaling from luma.
 * \endcond
 *
 * @section WisStdVideoAV1FilmGrain_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1FilmGrain_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
