/**
 * @struct wisViewHeapGetCPUHandle
 * @ingroup Functions
 *
 *
 * @section wisViewHeapGetCPUHandle_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisViewHeapGetCPUHandle(const WisViewHeap* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisVKViewHeapGetCPUHandle(const WisVKViewHeap* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisDX12ViewHeapGetCPUHandle(const WisDX12ViewHeap* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t ViewHeap::GetCPUHandle() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t VKViewHeap::GetCPUHandle() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t DX12ViewHeap::GetCPUHandle() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapGetCPUHandle_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * 
 * - **return** CPU descriptor handle for the view heap.
 * \endcond
 *
 * @section wisViewHeapGetCPUHandle_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapGetCPUHandle_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */