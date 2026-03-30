/**
 * @struct WisSampleDesc
 * @ingroup Structures
 *
 *
 * @section WisSampleDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisSampleDesc {
 *     WisSampleCount rate;
 *     uint32_t       sample_mask;
 *     bool           alpha_to_coverage_enable;
 * } WisSampleDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  SampleDesc {
 *     wis::SampleCount rate;
 *     std::uint32_t    sample_mask;
 *     bool             alpha_to_coverage_enable;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSampleDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `rate` defines sample rate. Default is `WisSampleCountS1`.
 * - `sample_mask` describes sample mask. Default is 0xffffffff.
 * - `alpha_to_coverage_enable` describes alpha to coverage enable. Default is false.
 * \endcond
 *
 * @section WisSampleDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSampleDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisGraphicsPipelineDesc
 * \endcond
 */