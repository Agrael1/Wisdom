/**
 * @struct WisFormatProperties
 * @ingroup Structures
 *
 *
 * @section WisFormatProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisFormatProperties {
 *     WisFormatSupportFlags format_support_flags;
 *     WisSampleCount        max_sample_count;
 * } WisFormatProperties;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  FormatProperties {
 *     wis::FormatSupportFlags format_support_flags;
 *     wis::SampleCount        max_sample_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisFormatProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `format_support_flags` specifies bitmask of supported features for the format.
 * - `max_sample_count` defines maximum supported sample count for the format. If the format does not support multisampling, the value is `S1`.
 * \endcond
 *
 * @section WisFormatProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisFormatProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceGetFormatProperties
 * \endcond
 */