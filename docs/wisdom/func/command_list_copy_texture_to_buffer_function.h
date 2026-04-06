/**
 * @struct wisCommandListCopyTextureToBuffer
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListCopyTextureToBuffer_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListCopyTextureToBuffer(const WisCommandList* self,
 *                                        WisBufferView                     dst_buffer,
 *                                        WisTextureView                    src_texture,
 *                                        const WisBufferTextureCopyRegion* regions,
 *                                        size_t                            region_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListCopyTextureToBuffer(const WisVKCommandList* self,
 *                                          WisVKBufferView                   dst_buffer,
 *                                          WisVKTextureView                  src_texture,
 *                                          const WisBufferTextureCopyRegion* regions,
 *                                          size_t                            region_count);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListCopyTextureToBuffer(const WisDX12CommandList* self,
 *                                            WisDX12BufferView                 dst_buffer,
 *                                            WisDX12TextureView                src_texture,
 *                                            const WisBufferTextureCopyRegion* regions,
 *                                            size_t                            region_count);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::CopyTextureToBuffer(wis::BufferView                               dst_buffer,
 *                                       wis::TextureView                              src_texture,
 *                                       wis::span<const wis::BufferTextureCopyRegion> regions) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::CopyTextureToBuffer(wis::VKBufferView                             dst_buffer,
 *                                         wis::VKTextureView                            src_texture,
 *                                         wis::span<const wis::BufferTextureCopyRegion> regions) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::CopyTextureToBuffer(wis::DX12BufferView                           dst_buffer,
 *                                           wis::DX12TextureView                          src_texture,
 *                                           wis::span<const wis::BufferTextureCopyRegion> regions) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListCopyTextureToBuffer_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `dst_buffer` defines a pointer to the destination buffer.
 * - `src_texture` describes a pointer to the source texture. Texture @wis_must be in `WisTextureStateCopySrc`.
 * - `regions` points to an array of WisBufferTextureCopyRegion that defines the copy regions.
 * - `region_count` defines the count of the regions.
 * \endcond
 *
 * @section wisCommandListCopyTextureToBuffer_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListCopyTextureToBuffer_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */