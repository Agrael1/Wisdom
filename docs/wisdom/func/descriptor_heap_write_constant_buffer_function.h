/**
 * @struct wisDescriptorHeapWriteConstantBuffer
 * @ingroup Functions
 *
 *
 * @section wisDescriptorHeapWriteConstantBuffer_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDescriptorHeapWriteConstantBuffer(const WisDescriptorHeap* self,
 *                                                const WisConstantBufferBinding* data,
 *                                                uint32_t                        index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDescriptorHeapWriteConstantBuffer(const WisVKDescriptorHeap* self,
 *                                                  const WisConstantBufferBinding* data,
 *                                                  uint32_t                        index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DescriptorHeapWriteConstantBuffer(const WisDX12DescriptorHeap* self,
 *                                                    const WisConstantBufferBinding* data,
 *                                                    uint32_t                        index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DescriptorHeap::WriteConstantBuffer(const wis::ConstantBufferBinding& data,
 *                                                 std::uint32_t                     index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKDescriptorHeap::WriteConstantBuffer(const wis::ConstantBufferBinding& data,
 *                                                   std::uint32_t                     index) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12DescriptorHeap::WriteConstantBuffer(const wis::ConstantBufferBinding& data,
 *                                                     std::uint32_t                     index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapWriteConstantBuffer_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * - `data` points to WisConstantBufferBinding, which describes the constant buffer descriptors to write.
 * - `index` defines the index in the descriptor heap to write the descriptors to.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDescriptorHeapWriteConstantBuffer_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapWriteConstantBuffer_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */