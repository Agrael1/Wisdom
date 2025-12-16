/**
 * @struct wisQueryAdapters
 * @ingroup Functions
 *
 *
 * @section wisQueryAdapters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisQueryAdapters(const WisInstance* self,
 *                            WisAdapterPreference preference,
 *                            WisAdapterQuery*     query);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKQueryAdapters(const WisVKInstance* self,
 *                              WisAdapterPreference preference,
 *                              WisVKAdapterQuery*   query);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12QueryAdapters(const WisDX12Instance* self,
 *                                WisAdapterPreference   preference,
 *                                WisDX12AdapterQuery*   query);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::AdapterQuery QueryAdapters(wis::AdapterPreference preference,
 *                                               wis::Result&           out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKAdapterQuery VKQueryAdapters(wis::AdapterPreference preference,
 *                                                   wis::Result&           out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12AdapterQuery DX12QueryAdapters(wis::AdapterPreference preference,
 *                                                       wis::Result&           out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisQueryAdapters_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisInstance instance.
 * - `preference` defines the order in which adapters are listed.
 * - `query` points to WisAdapterQuery, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisQueryAdapters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisQueryAdapters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */