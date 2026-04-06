/**
 * @struct WisSamplerFlags WisSamplerFlags
 * @ingroup Enumerations Core
 *
 * @section WisSamplerFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisSamplerFlags {
 *     WisSamplerFlagsNone = 0,
 *     WisSamplerFlagsNonNormalizedCoordinates = (1u << 0),
 * } WisSamplerFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class SamplerFlags : uint32_t {
 *     None = 0,
 *     NonNormalizedCoordinates = (1u << 0),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSamplerFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Flags for sampler creation.
 *
 * \note Translates to DirectX 12 as D3D12_SAMPLER_FLAGS.
 * Values:
 * - `WisSamplerFlagsNone = 0`: No flags set.
 * - `WisSamplerFlagsNonNormalizedCoordinates = (1 << 0)`: Use non-normalized texture coordinates.
 * \endcond
 *
 *
 * @section WisSamplerFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSamplerDesc
 * \endcond
 */