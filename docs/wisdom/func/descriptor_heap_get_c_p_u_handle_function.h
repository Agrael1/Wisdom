/**
 * @struct wisDescriptorHeapGetCPUHandle
 * @ingroup Functions Core
 *
 *
 * @section wisDescriptorHeapGetCPUHandle_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void* wisDescriptorHeapGetCPUHandle(const WisDescriptorHeap* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void* wisVKDescriptorHeapGetCPUHandle(const WisVKDescriptorHeap* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void* wisDX12DescriptorHeapGetCPUHandle(const WisDX12DescriptorHeap* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD void* DescriptorHeap::GetCPUHandle() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD void* VKDescriptorHeap::GetCPUHandle() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD void* DX12DescriptorHeap::GetCPUHandle() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapGetCPUHandle_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * 
 * - **return** CPU descriptor handle for the descriptor heap.
 * \endcond
 *
 * @section wisDescriptorHeapGetCPUHandle_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapGetCPUHandle_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */