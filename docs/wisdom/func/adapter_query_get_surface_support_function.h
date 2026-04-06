/**
 * @struct wisAdapterQueryGetSurfaceSupport
 * @ingroup Functions Core
 *
 *
 * @section wisAdapterQueryGetSurfaceSupport_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * bool wisAdapterQueryGetSurfaceSupport(const WisAdapterQuery* self,
 *                                       size_t                 index,
 *                                       WisSurfaceView         surface);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * bool wisVKAdapterQueryGetSurfaceSupport(const WisVKAdapterQuery* self,
 *                                         size_t                   index,
 *                                         WisVKSurfaceView         surface);
 * 
 * // Provided by Wisdom 0.7.0. 
 * bool wisDX12AdapterQueryGetSurfaceSupport(const WisDX12AdapterQuery* self,
 *                                           size_t                     index,
 *                                           WisDX12SurfaceView         surface);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD bool AdapterQuery::GetSurfaceSupport(std::size_t      index,
 *                                                    wis::SurfaceView surface) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD bool VKAdapterQuery::GetSurfaceSupport(std::size_t        index,
 *                                                      wis::VKSurfaceView surface) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD bool DX12AdapterQuery::GetSurfaceSupport(std::size_t          index,
 *                                                        wis::DX12SurfaceView surface) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisAdapterQueryGetSurfaceSupport_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 * - `index` defines the index of the adapter to check the support for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * - `surface` describes a pointer to WisSurface to check the presentation support for.
 * 
 * - **return** `true` if the adapter supports presentation to the surface, `false` otherwise.
 * \endcond
 *
 * @section wisAdapterQueryGetSurfaceSupport_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisAdapterQueryGetSurfaceSupport_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */