/**
 * @struct wisViewHeapGetViewAddress
 * @ingroup Functions Core
 *
 *
 * @section wisViewHeapGetViewAddress_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * uint64_t wisViewHeapGetViewAddress(const WisViewHeap* self,
 *                                    uint32_t           index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * uint64_t wisVKViewHeapGetViewAddress(const WisVKViewHeap* self,
 *                                      uint32_t             index);
 *
 * // Provided by Wisdom 0.7.0.
 * uint64_t wisDX12ViewHeapGetViewAddress(const WisDX12ViewHeap* self,
 *                                        uint32_t               index);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::uint64_t ViewHeap::GetViewAddress(std::uint32_t index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::uint64_t VKViewHeap::GetViewAddress(std::uint32_t index) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::uint64_t DX12ViewHeap::GetViewAddress(std::uint32_t index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapGetViewAddress_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * - `index` defines the index in the view heap to get the descriptor from.
 *
 * - **return** Address of a view in heap.
 * \endcond
 *
 * @section wisViewHeapGetViewAddress_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapGetViewAddress_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */