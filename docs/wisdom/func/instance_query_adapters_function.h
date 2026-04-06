/**
 * @struct wisInstanceQueryAdapters
 * @ingroup Functions Core
 *
 *
 * @section wisInstanceQueryAdapters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisInstanceQueryAdapters(const WisInstance* self,
 *                                    WisAdapterPreference preference,
 *                                    WisAdapterQuery*     query);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKInstanceQueryAdapters(const WisVKInstance* self,
 *                                      WisAdapterPreference preference,
 *                                      WisVKAdapterQuery*   query);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12InstanceQueryAdapters(const WisDX12Instance* self,
 *                                        WisAdapterPreference   preference,
 *                                        WisDX12AdapterQuery*   query);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::AdapterQuery Instance::QueryAdapters(wis::AdapterPreference preference,
 *                                                         wis::Result&           out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKAdapterQuery VKInstance::QueryAdapters(wis::AdapterPreference preference,
 *                                                             wis::Result&           out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12AdapterQuery DX12Instance::QueryAdapters(wis::AdapterPreference preference,
 *                                                                 wis::Result&           out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisInstanceQueryAdapters_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisInstance instance.
 * - `preference` indicates the order in which adapters are listed.
 * - `query` points to WisAdapterQuery, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisInstanceQueryAdapters_descr Description
 * <hr>
 * 
 * This function creates a `WisAdapterQuery` object that can be used to enumerate the available adapters on the system.
 * `preference` influences the order in which adapters are listed, allowing applications to prioritize certain types of adapters based on their needs.
 * `preference` does not guarantee the real adapter order, as the underlying graphics APIs @wis_may have their own heuristics for adapter selection.
 * The created AdapterQuery object @wis_must be destroyed with `wisDestroyAdapterQuery` when no longer needed.
 * The order of adapters depends on `preference`: 
 * For MinConsumption 
 * - DirectX 12: Integrated, Discrete, External, Software; 
 * - Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. 
 * For Performance 
 * - DirectX 12: External, Discrete, Integrated, Software; 
 * - Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisInstanceQueryAdapters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */