/**
 * @struct wisViewHeapGetView
 * @ingroup Functions
 *
 *
 * @section wisViewHeapGetView_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisViewHeapGetView(const WisViewHeap* self,
 *                             uint32_t           index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisVKViewHeapGetView(const WisVKViewHeap* self,
 *                               uint32_t             index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisDX12ViewHeapGetView(const WisDX12ViewHeap* self,
 *                                 uint32_t               index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t ViewHeap::GetView(std::uint32_t index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t VKViewHeap::GetView(std::uint32_t index) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t DX12ViewHeap::GetView(std::uint32_t index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapGetView_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * - `index` defines the index in the view heap to get the descriptor from.
 * 
 * - **return** CPU descriptor handle for the view heap.
 * \endcond
 *
 * @section wisViewHeapGetView_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapGetView_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */