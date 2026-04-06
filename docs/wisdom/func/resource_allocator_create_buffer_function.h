/**
 * @struct wisResourceAllocatorCreateBuffer
 * @ingroup Functions Core
 *
 *
 * @section wisResourceAllocatorCreateBuffer_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisResourceAllocatorCreateBuffer(const WisResourceAllocator* self,
 *                                            const WisBufferDesc*        desc,
 *                                            WisBuffer*                  buffer);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKResourceAllocatorCreateBuffer(const WisVKResourceAllocator* self,
 *                                              const WisBufferDesc*          desc,
 *                                              WisVKBuffer*                  buffer);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12ResourceAllocatorCreateBuffer(const WisDX12ResourceAllocator* self,
 *                                                const WisBufferDesc*            desc,
 *                                                WisDX12Buffer*                  buffer);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::Buffer ResourceAllocator::CreateBuffer(const wis::BufferDesc& desc,
 *                                                           wis::Result&           out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKBuffer VKResourceAllocator::CreateBuffer(const wis::BufferDesc& desc,
 *                                                               wis::Result&           out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12Buffer DX12ResourceAllocator::CreateBuffer(const wis::BufferDesc& desc,
 *                                                                   wis::Result&           out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisResourceAllocatorCreateBuffer_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisResourceAllocator instance.
 * - `desc` indicates a pointer to WisBufferDesc, which describes the buffer to create.
 * - `buffer` points to WisBuffer, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisResourceAllocatorCreateBuffer_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisResourceAllocatorCreateBuffer_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */