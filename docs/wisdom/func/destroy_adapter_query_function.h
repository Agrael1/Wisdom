/**
 * @struct wisDestroyAdapterQuery
 * @ingroup Functions Core
 *
 *
 * @section wisDestroyAdapterQuery_spec Specification
 * <hr>
 *
 * To destroy an AdapterQuery, call:
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisDestroyAdapterQuery(WisAdapterQuery* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKDestroyAdapterQuery(WisVKAdapterQuery* self);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12DestroyAdapterQuery(WisDX12AdapterQuery* self);
 * ```
 * </details>
 *
 * \endcond
 *
 * @section wisDestroyAdapterQuery_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisAdapterQuery instance.
 * \endcond
 *
 * @section wisDestroyAdapterQuery_descr Description
 * <hr>
 *
 * Destruction of the AdapterQuery handle does not invalidate any `WisAdapter` instances that were created from it. They remain valid until destroyed individually.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDestroyAdapterQuery_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */