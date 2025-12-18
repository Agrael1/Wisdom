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
 * Orders the adapters according to preference using builtin heuristics of underlying APIs. For DirectX 12, this translates directly to `DXGI_GPU_PREFERENCE`. For Vulkan, sorting is based on `VkPhysicalDeviceType` heuristics.
 * 
 * \note Translates to  `DXGI_GPU_PREFERENCE` for DirectX 12 implementation.
 * 
 * Values:
 * - `WisAdapterPreferenceNone = 0`: No particular preference, list adapters in system devised order.
 * - `WisAdapterPreferenceMinConsumption = 1`: List the adapters from low power consumption to high. DirectX 12: Integrated, Discrete, External, Software. Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU.
 * - `WisAdapterPreferencePerformance = 2`: List the adapters from high performance to low. DirectX 12: External, Discrete, Integrated, Software. Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
 * \endcond
 *
 * GPU order @wis_may vary between the implementations due to differing heuristics.
 *
 * @section WisAdapterPreference_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see wisInstanceQueryAdapters
 * \endcond
 */