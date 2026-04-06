/**
 * @struct wisAdapterQueryGetAdapterDesc
 * @ingroup Functions Core
 *
 *
 * @section wisAdapterQueryGetAdapterDesc_spec Specification
 * <hr>
 *
 * To get the description of an adapter from an AdapterQuery, call:
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisAdapterQueryGetAdapterDesc(const WisAdapterQuery* self,
 *                                         size_t                 index,
 *                                         WisAdapterDesc*        desc);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKAdapterQueryGetAdapterDesc(const WisVKAdapterQuery* self,
 *                                           size_t                   index,
 *                                           WisAdapterDesc*          desc);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12AdapterQueryGetAdapterDesc(const WisDX12AdapterQuery* self,
 *                                             size_t                     index,
 *                                             WisAdapterDesc*            desc);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::AdapterDesc AdapterQuery::GetAdapterDesc(std::size_t  index,
 *                                                             wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::AdapterDesc VKAdapterQuery::GetAdapterDesc(std::size_t  index,
 *                                                               wis::Result& out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::AdapterDesc DX12AdapterQuery::GetAdapterDesc(std::size_t  index,
 *                                                                 wis::Result& out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisAdapterQueryGetAdapterDesc_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 * - `index` defines the index of the adapter to get the description for. It @wis_must be less than the value returned
 * by wisAdapterQueryGetAdapterCount.
 * - `desc` points to WisAdapterDesc, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisAdapterQueryGetAdapterDesc_descr Description
 * <hr>
 *
 * `index` selects which adapter's description to retrieve from the AdapterQuery. The index is zero-based.
 * `desc` @wis_must be a valid pointer and it is populated with the description of the specified adapter, including
 * details such as its name, type, and capabilities. The information about memory resources @wis_maynot be identical
 * across different implementations, because underlying graphics APIs expose varying levels of detail about adapter
 * memory.
 * `
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisAdapterQueryGetAdapterDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
