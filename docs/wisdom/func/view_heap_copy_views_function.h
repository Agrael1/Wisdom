/**
 * @struct wisViewHeapCopyViews
 * @ingroup Functions
 *
 *
 * @section wisViewHeapCopyViews_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisViewHeapCopyViews(const WisViewHeap* self,
 *                           uint32_t           dst_index,
 *                           uint64_t           src_ptr,
 *                           uint32_t           src_index,
 *                           uint32_t           count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKViewHeapCopyViews(const WisVKViewHeap* self,
 *                             uint32_t             dst_index,
 *                             uint64_t             src_ptr,
 *                             uint32_t             src_index,
 *                             uint32_t             count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12ViewHeapCopyViews(const WisDX12ViewHeap* self,
 *                               uint32_t               dst_index,
 *                               uint64_t               src_ptr,
 *                               uint32_t               src_index,
 *                               uint32_t               count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void ViewHeap::CopyViews(std::uint32_t dst_index,
 *                          std::uint64_t src_ptr,
 *                          std::uint32_t src_index,
 *                          std::uint32_t count) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKViewHeap::CopyViews(std::uint32_t dst_index,
 *                            std::uint64_t src_ptr,
 *                            std::uint32_t src_index,
 *                            std::uint32_t count) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12ViewHeap::CopyViews(std::uint32_t dst_index,
 *                              std::uint64_t src_ptr,
 *                              std::uint32_t src_index,
 *                              std::uint32_t count) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapCopyViews_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * - `dst_index` defines the index in the destination view heap to copy views to.
 * - `src_ptr` points to WisViewHeap to copy views from.
 * - `src_index` defines the index in the source view heap to copy views from.
 * - `count` defines the number of views to copy.
 * \endcond
 *
 * @section wisViewHeapCopyViews_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapCopyViews_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */