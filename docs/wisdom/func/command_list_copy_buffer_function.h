/**
 * @struct wisCommandListCopyBuffer
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListCopyBuffer_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListCopyBuffer(const WisCommandList* self,
 *                               WisBufferView              dst_buffer,
 *                               WisBufferView              src_buffer,
 *                               const WisBufferCopyRegion* regions,
 *                               size_t                     region_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListCopyBuffer(const WisVKCommandList* self,
 *                                 WisVKBufferView            dst_buffer,
 *                                 WisVKBufferView            src_buffer,
 *                                 const WisBufferCopyRegion* regions,
 *                                 size_t                     region_count);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListCopyBuffer(const WisDX12CommandList* self,
 *                                   WisDX12BufferView          dst_buffer,
 *                                   WisDX12BufferView          src_buffer,
 *                                   const WisBufferCopyRegion* regions,
 *                                   size_t                     region_count);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::CopyBuffer(wis::BufferView                        dst_buffer,
 *                              wis::BufferView                        src_buffer,
 *                              wis::span<const wis::BufferCopyRegion> regions) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::CopyBuffer(wis::VKBufferView                      dst_buffer,
 *                                wis::VKBufferView                      src_buffer,
 *                                wis::span<const wis::BufferCopyRegion> regions) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::CopyBuffer(wis::DX12BufferView                    dst_buffer,
 *                                  wis::DX12BufferView                    src_buffer,
 *                                  wis::span<const wis::BufferCopyRegion> regions) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListCopyBuffer_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `dst_buffer` defines a pointer to the destination buffer.
 * - `src_buffer` describes a pointer to the source buffer.
 * - `regions` points to an array of WisBufferCopyRegion that defines the copy regions.
 * - `region_count` defines the count of the regions.
 * \endcond
 *
 * @section wisCommandListCopyBuffer_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListCopyBuffer_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
