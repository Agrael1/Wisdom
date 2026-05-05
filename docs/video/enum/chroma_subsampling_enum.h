/**
 * @struct WisChromaSubsampling WisChromaSubsampling
 * @ingroup Enumerations Video
 *
 * @section WisChromaSubsampling_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisChromaSubsampling {
 *     WisChromaSubsamplingNone = 0,
 *     WisChromaSubsamplingC420 = (1u << 1),
 *     WisChromaSubsamplingC422 = (1u << 2),
 *     WisChromaSubsamplingC444 = (1u << 3),
 * } WisChromaSubsampling;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class ChromaSubsampling : uint32_t {
 *     None = 0,
 *     C420 = (1u << 1),
 *     C422 = (1u << 2),
 *     C444 = (1u << 3),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisChromaSubsampling_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Chroma subsampling flags for video components. Used to specify supported chroma subsampling formats for video
 * decoding.
 *
 * \note Translates to Vulkan as VkVideoChromaSubsamplingFlagsKHR .
 *
 * Values:
 * - `WisChromaSubsamplingNone = 0`: No chroma subsampling is specified.
 * - `WisChromaSubsamplingC420 = (1 << 1)`: 4:2:0 chroma subsampling format.
 * - `WisChromaSubsamplingC422 = (1 << 2)`: 4:2:2 chroma subsampling format.
 * - `WisChromaSubsamplingC444 = (1 << 3)`: 4:4:4 chroma subsampling format.
 * \endcond
 *
 *
 * @section WisChromaSubsampling_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * \endcond
 */
