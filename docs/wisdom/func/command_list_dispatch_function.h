/**
 * @struct wisCommandListDispatch
 * @ingroup Functions
 *
 *
 * @section wisCommandListDispatch_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListDispatch(const WisCommandList* self,
 *                             uint32_t              group_count_x,
 *                             uint32_t              group_count_y,
 *                             uint32_t              group_count_z);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListDispatch(const WisVKCommandList* self,
 *                               uint32_t                group_count_x,
 *                               uint32_t                group_count_y,
 *                               uint32_t                group_count_z);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListDispatch(const WisDX12CommandList* self,
 *                                 uint32_t                  group_count_x,
 *                                 uint32_t                  group_count_y,
 *                                 uint32_t                  group_count_z);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::Dispatch(std::uint32_t group_count_x,
 *                            std::uint32_t group_count_y,
 *                            std::uint32_t group_count_z) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::Dispatch(std::uint32_t group_count_x,
 *                              std::uint32_t group_count_y,
 *                              std::uint32_t group_count_z) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::Dispatch(std::uint32_t group_count_x,
 *                                std::uint32_t group_count_y,
 *                                std::uint32_t group_count_z) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListDispatch_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `group_count_x` The number of groups to dispatch in X dimension.
 * - `group_count_y` The number of groups to dispatch in Y dimension. Default is 1.
 * - `group_count_z` The number of groups to dispatch in Z dimension. Default is 1.
 * \endcond
 *
 * @section wisCommandListDispatch_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListDispatch_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */