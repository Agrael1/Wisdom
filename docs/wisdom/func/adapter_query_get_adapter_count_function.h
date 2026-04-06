/**
 * @struct wisAdapterQueryGetAdapterCount
 * @ingroup Functions Core
 *
 *
 * @section wisAdapterQueryGetAdapterCount_spec Specification
 * <hr>
 *
 * To get the number of adapters available from an AdapterQuery, call:
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * size_t wisAdapterQueryGetAdapterCount(const WisAdapterQuery* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * size_t wisVKAdapterQueryGetAdapterCount(const WisVKAdapterQuery* self);
 *
 * // Provided by Wisdom 0.7.0.
 * size_t wisDX12AdapterQueryGetAdapterCount(const WisDX12AdapterQuery* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::size_t AdapterQuery::GetAdapterCount() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::size_t VKAdapterQuery::GetAdapterCount() const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::size_t DX12AdapterQuery::GetAdapterCount() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisAdapterQueryGetAdapterCount_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 *
 * - **return** is a number of adapters present on the system.
 * \endcond
 *
 * @section wisAdapterQueryGetAdapterCount_descr Description
 * <hr>
 *
 * Returns the number of adapters that can be queried from the AdapterQuery instance. This number is fixed at the time of AdapterQuery creation and does not change during its lifetime.
 * If no adapters are found, the function returns zero.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisAdapterQueryGetAdapterCount_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */