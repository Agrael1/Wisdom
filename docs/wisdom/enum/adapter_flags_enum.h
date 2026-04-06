/**
 * @struct WisAdapterFlags WisAdapterFlags
 * @ingroup Enumerations Core
 *
 * @section WisAdapterFlags_spec Specification
 * <hr>
 *
 * In order to describe adapter features check:
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisAdapterFlags {
 *     WisAdapterFlagsNone = 0,
 *     WisAdapterFlagsRemote = (1u << 0),
 *     WisAdapterFlagsSoftware = (1u << 1),
 * } WisAdapterFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class AdapterFlags : uint32_t {
 *     None = 0,
 *     Remote = (1u << 0),
 *     Software = (1u << 1),
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
 * - `WisAdapterFlagsNone = 0`: No flags set. Adapter @wis_may be discrete or embedded.
 * - `WisAdapterFlagsRemote = (1 << 0)`: Adapter is remote. Used for remote rendering.
 * - `WisAdapterFlagsSoftware = (1 << 1)`: Adapter is software. Uses CPU for software rendering.
 * \endcond
 *
 * \note on DirectX 12 implementation, `WisAdapterFlagsRemote` translates to `DXGI_ADAPTER_FLAG_REMOTE` and `WisAdapterFlagsSoftware` translates to `DXGI_ADAPTER_FLAG_SOFTWARE`.
 * The values are directly mapped, meaning it is possible to get values outside of this enum when querying adapters.
 *
 * Adapters that are neither remote nor software are considered hardware adapters, which can be either discrete or integrated GPUs.
 * The distinction is not available through flags, but can be inferred from other properties such as vendor and device IDs.
 *
 * @section WisAdapterFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisAdapterDesc
 * \endcond
 */