/**
 * @struct wisGetAdapterDesc
 * @ingroup Functions
 *
 *
 * @section wisGetAdapterDesc_spec Specification
 * <hr>
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
 * WIS_NODISCARD wis::AdapterDesc GetAdapterDesc(std::size_t  index,
 *                                               wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::AdapterDesc VKGetAdapterDesc(std::size_t  index,
 *                                                 wis::Result& out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::AdapterDesc DX12GetAdapterDesc(std::size_t  index,
 *                                                   wis::Result& out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisGetAdapterDesc_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 * - `index` defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wisGetAdapterCount.
 * - `desc` points to WisAdapterDesc, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisGetAdapterDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisGetAdapterDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */