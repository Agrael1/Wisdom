/**
 * @struct wisDescriptorHeapWriteAccelerationStructure
 * @ingroup Functions
 *
 *
 * @section wisDescriptorHeapWriteAccelerationStructure_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDescriptorHeapWriteAccelerationStructure(const WisDescriptorHeap* self,
 *                                                       uint64_t                 address,
 *                                                       uint32_t                 index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDescriptorHeapWriteAccelerationStructure(const WisVKDescriptorHeap* self,
 *                                                         uint64_t                   address,
 *                                                         uint32_t                   index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DescriptorHeapWriteAccelerationStructure(const WisDX12DescriptorHeap* self,
 *                                                           uint64_t                     address,
 *                                                           uint32_t                     index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DescriptorHeap::WriteAccelerationStructure(std::uint64_t address,
 *                                                        std::uint32_t index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKDescriptorHeap::WriteAccelerationStructure(std::uint64_t address,
 *                                                          std::uint32_t index) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12DescriptorHeap::WriteAccelerationStructure(std::uint64_t address,
 *                                                            std::uint32_t index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapWriteAccelerationStructure_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * - `address` GPU address of a raytracing acceleration structure.
 * - `index` defines the index in the descriptor heap to write the descriptor to.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDescriptorHeapWriteAccelerationStructure_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapWriteAccelerationStructure_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */