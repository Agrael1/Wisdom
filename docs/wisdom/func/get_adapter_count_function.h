/**
 * @struct wisGetAdapterCount
 * @ingroup Functions
 *
 *
 * @section wisGetAdapterCount_spec Specification
 * <hr>
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
 * WIS_NODISCARD std::size_t GetAdapterCount() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::size_t VKGetAdapterCount() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::size_t DX12GetAdapterCount() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisGetAdapterCount_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 * 
 * - **return** is a number of adapters present on the system.
 * \endcond
 *
 * @section wisGetAdapterCount_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisGetAdapterCount_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */