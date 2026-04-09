/**
 * @struct WisAdapterQuery
 * @ingroup Handles Core
 *
 *
 * @section WisAdapterQuery_spec Specification
 * <hr>
 *
 * A purpose of this handle is to contain a snapshot of adapters that are present on the system.
 * The handle is designed to be short lived. If there are any changes to the system adapters, a new query must be
 * performed. If the underlying adapters are removed or added, the existing query becomes invalid.
 *
 * \cond WIS_GEN_CODE
 *  Vulkan Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WIS_DEFINE_HANDLE(WisVKAdapterQuery,4);
 * ```
 *  DX12 Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WIS_DEFINE_HANDLE(WisDX12AdapterQuery,4);
 * ```
 * \endcond
 *
 * @section WisAdapterQuery_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDestroyAdapterQuery, wisInstanceQueryAdapters, wisAdapterQueryGetAdapterCount, wisAdapterQueryGetAdapterDesc, wisAdapterQueryGetSurfaceSupport, wisAdapterQueryCreateDevice
 * \endcond
 */
