/**
 * @struct WisStdVideoAV1Profile WisStdVideoAV1Profile
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1Profile_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1Profile {
 *     WisStdVideoAV1ProfileMain = 0,
 *     WisStdVideoAV1ProfileHigh = 1,
 *     WisStdVideoAV1ProfileProfessional = 2,
 *     WisStdVideoAV1ProfileInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1Profile;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1Profile {
 *     Main = 0,
 *     High = 1,
 *     Professional = 2,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1Profile_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * AV1 profiles as defined in the AV1 Bitstream Specification section 6.4.1.
 *
 * Values:
 * - `WisStdVideoAV1ProfileMain = 0`: Main profile (8-bit or 10-bit color, 4:0:0 or 4:2:0).
 * - `WisStdVideoAV1ProfileHigh = 1`: High profile (adds 8-bit or 10-bit 4:4:4).
 * - `WisStdVideoAV1ProfileProfessional = 2`: Professional profile (adds 12-bit color, and 4:2:2).
 * - `WisStdVideoAV1ProfileInvalid = 0x7FFFFFFF`: Invalid profile.
 * \endcond
 *
 *
 * @section WisStdVideoAV1Profile_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1SequenceHeader
 * \endcond
 */
