/**
 * @struct WisStdVideoH265ProfileTierLevelFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoH265ProfileTierLevelFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoH265ProfileTierLevelFlags {
 *     uint32_t general_tier_flag : 1;
 *     uint32_t general_progressive_source_flag : 1;
 *     uint32_t general_interlaced_source_flag : 1;
 *     uint32_t general_non_packed_constraint_flag : 1;
 *     uint32_t general_frame_only_constraint_flag : 1;
 *     uint32_t reserved : 27;
 * } WisStdVideoH265ProfileTierLevelFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoH265ProfileTierLevelFlags {
 *     std::uint32_t general_tier_flag : 1;
 *     std::uint32_t general_progressive_source_flag : 1;
 *     std::uint32_t general_interlaced_source_flag : 1;
 *     std::uint32_t general_non_packed_constraint_flag : 1;
 *     std::uint32_t general_frame_only_constraint_flag : 1;
 *     std::uint32_t reserved : 27;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoH265ProfileTierLevelFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `general_tier_flag` Specifies the tier for the profile (0 = Main, 1 = High).
 * - `general_progressive_source_flag` Specifies that the source is progressive.
 * - `general_interlaced_source_flag` Specifies that the source is interlaced.
 * - `general_non_packed_constraint_flag` Specifies that the source is non-packed.
 * - `general_frame_only_constraint_flag` Specifies that the source is frame-only.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoH265ProfileTierLevelFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoH265ProfileTierLevelFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoH265ProfileTierLevel
 * \endcond
 */
