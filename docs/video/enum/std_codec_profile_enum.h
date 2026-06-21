/**
 * @struct WisStdCodecProfile WisStdCodecProfile
 * @ingroup Enumerations Video
 *
 * @section WisStdCodecProfile_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdCodecProfile {
 *     WisStdCodecProfileH264Baseline = 0,
 *     WisStdCodecProfileH264Main = 1,
 *     WisStdCodecProfileH264High = 2,
 *     WisStdCodecProfileH264HighPredictive = 3,
 *     WisStdCodecProfileH265Main = 32,
 *     WisStdCodecProfileH265Main10 = 33,
 *     WisStdCodecProfileH265Main12 = 34,
 *     WisStdCodecProfileH265Main16 = 35,
 *     WisStdCodecProfileH265FormatRangeExt = 36,
 *     WisStdCodecProfileAV1Main = 64,
 *     WisStdCodecProfileAV1High = 65,
 *     WisStdCodecProfileAV1Professional = 66,
 *     WisStdCodecProfileVP9Profile0 = 96,
 *     WisStdCodecProfileVP9Profile1 = 97,
 *     WisStdCodecProfileVP9Profile2 = 98,
 *     WisStdCodecProfileVP9Profile3 = 99,
 * } WisStdCodecProfile;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdCodecProfile {
 *     H264Baseline = 0,
 *     H264Main = 1,
 *     H264High = 2,
 *     H264HighPredictive = 3,
 *     H265Main = 32,
 *     H265Main10 = 33,
 *     H265Main12 = 34,
 *     H265Main16 = 35,
 *     H265FormatRangeExt = 36,
 *     AV1Main = 64,
 *     AV1High = 65,
 *     AV1Professional = 66,
 *     VP9Profile0 = 96,
 *     VP9Profile1 = 97,
 *     VP9Profile2 = 98,
 *     VP9Profile3 = 99,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdCodecProfile_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Standard codec profiles. Used to specify the profile of a video codec implementation.
 *
 * Values:
 * - `WisStdCodecProfileH264Baseline = 0`: H.264 Baseline profile. Suitable for low-complexity applications such as
 * video conferencing and mobile streaming.
 * - `WisStdCodecProfileH264Main = 1`: H.264 Main profile. Offers better compression efficiency than Baseline, suitable
 * for standard-definition video streaming and broadcast.
 * - `WisStdCodecProfileH264High = 2`: H.264 High profile. Provides the best compression efficiency among H.264
 * profiles, suitable for high-definition video streaming and Blu-ray discs.
 * - `WisStdCodecProfileH264HighPredictive = 3`: H.264 High Predictive profile. An extension of the High profile that
 * adds support for additional features, such as improved error resilience and better performance.
 * - `WisStdCodecProfileH265Main = 32`: H.265 Main profile. Suitable for standard-definition video streaming and
 * broadcast.
 * - `WisStdCodecProfileH265Main10 = 33`: H.265 Main 10 profile. Adds support for 10-bit video, providing better color
 * depth and improved compression efficiency compared to the Main profile.
 * - `WisStdCodecProfileH265Main12 = 34`: H.265 Main 12 profile. Adds support for 12-bit video, offering even greater
 * color depth and improved compression efficiency compared to the Main 10 profile.
 * - `WisStdCodecProfileH265Main16 = 35`: H.265 Main 16 profile. Adds support for 16-bit video, providing the highest
 * color depth and best compression efficiency among the Main profiles, suitable for professional video production and
 * post-production workflows.
 * - `WisStdCodecProfileH265FormatRangeExt = 36`: H.265 Format Range Extensions profile. Adds support for additional
 * features such as higher bit depths, wider color gamut, and improved compression efficiency, suitable for high-quality
 * video streaming and broadcast.
 * - `WisStdCodecProfileAV1Main = 64`: AV1 Main profile. Suitable for a wide range of applications, including web
 * streaming and video conferencing.
 * - `WisStdCodecProfileAV1High = 65`: AV1 High profile. Adds support for additional features such as higher bit depths
 * and improved compression efficiency, suitable for high-quality video streaming and broadcast.
 * - `WisStdCodecProfileAV1Professional = 66`: AV1 Professional profile. Designed for professional video production and
 * post-production workflows, offering support for high bit depths, wide color gamut, and advanced features.
 * - `WisStdCodecProfileVP9Profile0 = 96`: VP9 Profile 0. Supports 8-bit video with 4:2:0 chroma subsampling, suitable
 * for web streaming and general-purpose video applications.
 * - `WisStdCodecProfileVP9Profile1 = 97`: VP9 Profile 1. Adds support for 8-bit video with 4:2:2 and 4:4:4 chroma
 * subsampling, suitable for professional video production and post-production workflows that require higher color
 * fidelity.
 * - `WisStdCodecProfileVP9Profile2 = 98`: VP9 Profile 2. Adds support for 10-bit and 12-bit video with 4:2:0 chroma
 * subsampling.
 * - `WisStdCodecProfileVP9Profile3 = 99`: VP9 Profile 3. Adds support for 10-bit and 12-bit video with 4:2:2 and 4:4:4
 * chroma subsampling.
 * \endcond
 *
 *
 * @section WisStdCodecProfile_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisVideoCodecDesc, WisVideoDecoderDesc, WisVideoDecodePictureDesc
 * \endcond
 */
