/**
 * @struct WisAdapterFlags WisAdapterFlags
 * @ingroup Enumerations
 *
 * @section WisAdapterFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisAdapterFlags {
 *     WisAdapterFlagsNone = 0,
 *     WisAdapterFlagsRemote = (1 << 0),
 *     WisAdapterFlagsSoftware = (1 << 1),
 * } WisAdapterFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class AdapterFlags : uint32_t {
 *     None = 0,
 *     Remote = (1 << 0),
 *     Software = (1 << 1),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAdapterFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Flags that describe adapter.
 * 
 * Values:
 * - `WisAdapterFlagsNone = 0`: No flags set. Adapter @wis_may be descrete or embedded.
 * - `WisAdapterFlagsRemote = (1 << 0)`: Adapter is remote. Used for remote rendering.
 * - `WisAdapterFlagsSoftware = (1 << 1)`: Adapter is software. Uses CPU for software rendering.
 * \endcond
 *
 *
 * @section WisAdapterFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisAdapterDesc
 * \endcond
 */