/**
 * @struct WisAdapterPreference WisAdapterPreference
 * @ingroup Enumerations
 *
 * @section WisAdapterPreference_spec Specification
 * <hr>
 * 
 * To select order of adapters use:
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisAdapterPreference {
 *     WisAdapterPreferenceNone = 0,
 *     WisAdapterPreferenceMinConsumption = 1,
 *     WisAdapterPreferencePerformance = 2,
 * } WisAdapterPreference;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class AdapterPreference {
 *     None = 0,
 *     MinConsumption = 1,
 *     Performance = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAdapterPreference_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Orders the adapters according to preference using builtin heuristics of underlying APIs.
 * 
 * \note Translates to  `DXGI_GPU_PREFERENCE` for DirectX 12 implementation.
 * 
 * Values:
 * - `WisAdapterPreferenceNone = 0`: No particular preference, list adapters in system divised order.
 * - `WisAdapterPreferenceMinConsumption = 1`: List the adapters from low power consumption to high. Order is as follows: Integrated, Discrete, External, Software.
 * - `WisAdapterPreferencePerformance = 2`: List the adapters from high performance to low. Order is as follows: External, Discrete, Integrated, Software.
 * \endcond
 *
 *
 * @section WisAdapterPreference_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see wisQueryAdapters
 * \endcond
 */