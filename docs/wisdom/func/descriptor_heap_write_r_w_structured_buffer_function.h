/**
 * @struct wisDescriptorHeapWriteRWStructuredBuffer
 * @ingroup Functions Core
 *
 *
 * @section wisDescriptorHeapWriteRWStructuredBuffer_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDescriptorHeapWriteRWStructuredBuffer(const WisDescriptorHeap* self,
 *                                                    WisBufferView            buffer,
 *                                                    const WisBufferBinding*  data,
 *                                                    uint32_t                 index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDescriptorHeapWriteRWStructuredBuffer(const WisVKDescriptorHeap* self,
 *                                                      WisVKBufferView            buffer,
 *                                                      const WisBufferBinding*    data,
 *                                                      uint32_t                   index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DescriptorHeapWriteRWStructuredBuffer(const WisDX12DescriptorHeap* self,
 *                                                        WisDX12BufferView            buffer,
 *                                                        const WisBufferBinding*      data,
 *                                                        uint32_t                     index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DescriptorHeap::WriteRWStructuredBuffer(wis::BufferView           buffer,
 *                                                     const wis::BufferBinding& data,
 *                                                     std::uint32_t             index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKDescriptorHeap::WriteRWStructuredBuffer(wis::VKBufferView         buffer,
 *                                                       const wis::BufferBinding& data,
 *                                                       std::uint32_t             index) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12DescriptorHeap::WriteRWStructuredBuffer(wis::DX12BufferView       buffer,
 *                                                         const wis::BufferBinding& data,
 *                                                         std::uint32_t             index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapWriteRWStructuredBuffer_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * - `buffer` specifies a pointer to WisBuffer to write the descriptor for.
 * - `data` specifies a pointer to WisBufferBinding, which describes the shader resource view descriptors to write.
 * - `index` defines the index in the descriptor heap to write the descriptors to.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDescriptorHeapWriteRWStructuredBuffer_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapWriteRWStructuredBuffer_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */