/**
 * @struct WisVideoCodecFlags WisVideoCodecFlags
 * @ingroup Enumerations Video
 *
 * @section WisVideoCodecFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisVideoCodecFlags {
 *     WisVideoCodecFlagsNone = 0,
 *     WisVideoCodecFlagsH264 = (1u << 0),
 *     WisVideoCodecFlagsH265 = (1u << 1),
 *     WisVideoCodecFlagsAV1 = (1u << 2),
 *     WisVideoCodecFlagsVP9 = (1u << 3),
 * } WisVideoCodecFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class VideoCodecFlags : uint32_t {
 *     None = 0,
 *     H264 = (1u << 0),
 *     H265 = (1u << 1),
 *     AV1 = (1u << 2),
 *     VP9 = (1u << 3),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVideoCodecFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Video codec flags. Used to request and check supported codecs.
 *
 * Values:
 * - `WisVideoCodecFlagsNone = 0`: No video is requested. The extension will not initialize.
 * - `WisVideoCodecFlagsH264 = (1 << 0)`: H.264 video codec.
 * - `WisVideoCodecFlagsH265 = (1 << 1)`: H.265 video codec.
 * - `WisVideoCodecFlagsAV1 = (1 << 2)`: AV1 video codec.
 * - `WisVideoCodecFlagsVP9 = (1 << 3)`: VP9 video codec.
 * \endcond
 *
 *
 * @section WisVideoCodecFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisInitVideoDecodingExtension
 * \endcond
 */
