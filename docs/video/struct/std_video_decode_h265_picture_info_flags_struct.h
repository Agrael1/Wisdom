/**
 * @struct WisStdVideoDecodeH265PictureInfoFlags
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoDecodeH265PictureInfoFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoDecodeH265PictureInfoFlags {
 *     uint32_t IrapPicFlag : 1;
 *     uint32_t IdrPicFlag : 1;
 *     uint32_t IsReference : 1;
 *     uint32_t short_term_ref_pic_set_sps_flag : 1;
 *     uint32_t reserved : 28;
 * } WisStdVideoDecodeH265PictureInfoFlags;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoDecodeH265PictureInfoFlags {
 *     std::uint32_t IrapPicFlag : 1;
 *     std::uint32_t IdrPicFlag : 1;
 *     std::uint32_t IsReference : 1;
 *     std::uint32_t short_term_ref_pic_set_sps_flag : 1;
 *     std::uint32_t reserved : 28;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoDecodeH265PictureInfoFlags_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `IrapPicFlag` Specifies that the picture is an IRAP picture.
 * - `IdrPicFlag` Specifies that the picture is an IDR picture.
 * - `IsReference` Specifies that the picture is a reference picture.
 * - `short_term_ref_pic_set_sps_flag` Specifies that the short-term ref pic set is from SPS.
 * - `reserved` No description.
 * \endcond
 *
 * @section WisStdVideoDecodeH265PictureInfoFlags_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoDecodeH265PictureInfoFlags_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeH265PictureInfo
 * \endcond
 */
