/**
 * @struct wisCommandListRSSetScissors
 * @ingroup Functions
 *
 *
 * @section wisCommandListRSSetScissors_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListRSSetScissors(WisCommandList* self,
 *                                  const WisScissor* scissors,
 *                                  size_t            count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListRSSetScissors(WisVKCommandList* self,
 *                                    const WisScissor* scissors,
 *                                    size_t            count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListRSSetScissors(WisDX12CommandList* self,
 *                                      const WisScissor*   scissors,
 *                                      size_t              count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section wisCommandListRSSetScissors_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `scissors` The scissors to set.
 * - `count` The number of scissors to set.
 * 
 * \endcond
 *
 * @section wisCommandListRSSetScissors_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListRSSetScissors_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */