/**
 * @struct wisViewHeapGetCPUAddress
 * @ingroup Functions Core
 *
 *
 * @section wisViewHeapGetCPUAddress_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisViewHeapGetCPUAddress(const WisViewHeap* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisVKViewHeapGetCPUAddress(const WisVKViewHeap* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisDX12ViewHeapGetCPUAddress(const WisDX12ViewHeap* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t ViewHeap::GetCPUAddress() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t VKViewHeap::GetCPUAddress() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t DX12ViewHeap::GetCPUAddress() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapGetCPUAddress_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * 
 * - **return** CPU descriptor handle for the view heap.
 * \endcond
 *
 * @section wisViewHeapGetCPUAddress_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapGetCPUAddress_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
