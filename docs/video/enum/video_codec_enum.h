/**
 * @struct WisVideoCodec WisVideoCodec
 * @ingroup Enumerations Video
 *
 * @section WisVideoCodec_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisVideoCodec {
 *     WisVideoCodecNone = 0,
 *     WisVideoCodecH264 = (1u << 0),
 *     WisVideoCodecH265 = (1u << 1),
 *     WisVideoCodecAV1 = (1u << 2),
 *     WisVideoCodecVP9 = (1u << 3),
 * } WisVideoCodec;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class VideoCodec : uint32_t {
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
 * @section WisVideoCodec_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Video codec flags. Used to request and check supported codecs.
 *
 * Values:
 * - `WisVideoCodecNone = 0`: No video is requested. The extension will not initialize.
 * - `WisVideoCodecH264 = (1 << 0)`: H.264 video codec.
 * - `WisVideoCodecH265 = (1 << 1)`: H.265 video codec.
 * - `WisVideoCodecAV1 = (1 << 2)`: AV1 video codec.
 * - `WisVideoCodecVP9 = (1 << 3)`: VP9 video codec.
 * \endcond
 *
 *
 * @section WisVideoCodec_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * \endcond
 */
