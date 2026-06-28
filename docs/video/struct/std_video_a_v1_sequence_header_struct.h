/**
 * @struct WisStdVideoAV1SequenceHeader
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1SequenceHeader_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1SequenceHeader {
 *     WisStdVideoAV1SequenceHeaderFlags flags;
 *     WisStdVideoAV1Profile             seq_profile;
 *     uint8_t                           frame_width_bits_minus_1;
 *     uint8_t                           frame_height_bits_minus_1;
 *     uint16_t                          max_frame_width_minus_1;
 *     uint16_t                          max_frame_height_minus_1;
 *     uint8_t                           delta_frame_id_length_minus_2;
 *     uint8_t                           additional_frame_id_length_minus_1;
 *     uint8_t                           order_hint_bits_minus_1;
 *     uint8_t                           seq_force_integer_mv;
 *     uint8_t                           seq_force_screen_content_tools;
 *     uint8_t                           reserved1[5];
 *     const WisStdVideoAV1ColorConfig*  pColorConfig;
 *     const WisStdVideoAV1TimingInfo*   pTimingInfo;
 * } WisStdVideoAV1SequenceHeader;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1SequenceHeader {
 *     wis::StdVideoAV1SequenceHeaderFlags flags;
 *     wis::StdVideoAV1Profile             seq_profile;
 *     std::uint8_t                        frame_width_bits_minus_1;
 *     std::uint8_t                        frame_height_bits_minus_1;
 *     std::uint16_t                       max_frame_width_minus_1;
 *     std::uint16_t                       max_frame_height_minus_1;
 *     std::uint8_t                        delta_frame_id_length_minus_2;
 *     std::uint8_t                        additional_frame_id_length_minus_1;
 *     std::uint8_t                        order_hint_bits_minus_1;
 *     std::uint8_t                        seq_force_integer_mv;
 *     std::uint8_t                        seq_force_screen_content_tools;
 *     std::array<std::uint8_t, 5>         reserved1;
 *     const wis::StdVideoAV1ColorConfig*  pColorConfig;
 *     const wis::StdVideoAV1TimingInfo*   pTimingInfo;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1SequenceHeader_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Sequence header flags.
 * - `seq_profile` AV1 profile.
 * - `frame_width_bits_minus_1` Number of bits used to specify the frame width minus 1.
 * - `frame_height_bits_minus_1` Number of bits used to specify the frame height minus 1.
 * - `max_frame_width_minus_1` Maximum frame width minus 1.
 * - `max_frame_height_minus_1` Maximum frame height minus 1.
 * - `delta_frame_id_length_minus_2` Specifies the number of bits used to encode delta_frame_id.
 * - `additional_frame_id_length_minus_1` Used to calculate the number of bits used to encode frame_id.
 * - `order_hint_bits_minus_1` Used to compute OrderHintBits.
 * - `seq_force_integer_mv` Equal to 1: motion vectors will always be integers.
 * - `seq_force_screen_content_tools` Screen content tools setting.
 * - `reserved1` No description.
 * - `pColorConfig` Pointer to color configuration parameters.
 * - `pTimingInfo` Pointer to timing info parameters.
 * \endcond
 *
 * @section WisStdVideoAV1SequenceHeader_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1SequenceHeader_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisVideoDecodeAV1Desc
 * \endcond
 */
