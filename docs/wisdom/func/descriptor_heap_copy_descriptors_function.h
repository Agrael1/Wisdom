/**
 * @struct wisDescriptorHeapCopyDescriptors
 * @ingroup Functions Core
 *
 *
 * @section wisDescriptorHeapCopyDescriptors_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisDescriptorHeapCopyDescriptors(const WisDescriptorHeap* self,
 *                                       uint32_t                 dst_index,
 *                                       const void*              src_ptr,
 *                                       uint32_t                 src_index,
 *                                       uint32_t                 descriptor_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKDescriptorHeapCopyDescriptors(const WisVKDescriptorHeap* self,
 *                                         uint32_t                   dst_index,
 *                                         const void*                src_ptr,
 *                                         uint32_t                   src_index,
 *                                         uint32_t                   descriptor_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12DescriptorHeapCopyDescriptors(const WisDX12DescriptorHeap* self,
 *                                           uint32_t                     dst_index,
 *                                           const void*                  src_ptr,
 *                                           uint32_t                     src_index,
 *                                           uint32_t                     descriptor_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void DescriptorHeap::CopyDescriptors(std::uint32_t dst_index,
 *                                      const void*   src_ptr,
 *                                      std::uint32_t src_index,
 *                                      std::uint32_t descriptor_count) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKDescriptorHeap::CopyDescriptors(std::uint32_t dst_index,
 *                                        const void*   src_ptr,
 *                                        std::uint32_t src_index,
 *                                        std::uint32_t descriptor_count) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12DescriptorHeap::CopyDescriptors(std::uint32_t dst_index,
 *                                          const void*   src_ptr,
 *                                          std::uint32_t src_index,
 *                                          std::uint32_t descriptor_count) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapCopyDescriptors_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * - `dst_index` specifies the index in the destination descriptor heap to copy descriptors to.
 * - `src_ptr` describes a pointer to WisDescriptorHeap to copy descriptors from. Source heap @wis_must be CPU Only heap.
 * - `src_index` indicates the index in the source descriptor heap to copy descriptors from.
 * - `descriptor_count` indicates the number of descriptors to copy.
 * \endcond
 *
 * @section wisDescriptorHeapCopyDescriptors_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapCopyDescriptors_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
