/**
 * @struct wisCommandListCopyBufferToTexture
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListCopyBufferToTexture_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListCopyBufferToTexture(const WisCommandList* self,
 *                                        WisTextureView                    dst_texture,
 *                                        WisBufferView                     src_buffer,
 *                                        const WisBufferTextureCopyRegion* regions,
 *                                        size_t                            region_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListCopyBufferToTexture(const WisVKCommandList* self,
 *                                          WisVKTextureView                  dst_texture,
 *                                          WisVKBufferView                   src_buffer,
 *                                          const WisBufferTextureCopyRegion* regions,
 *                                          size_t                            region_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListCopyBufferToTexture(const WisDX12CommandList* self,
 *                                            WisDX12TextureView                dst_texture,
 *                                            WisDX12BufferView                 src_buffer,
 *                                            const WisBufferTextureCopyRegion* regions,
 *                                            size_t                            region_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::CopyBufferToTexture(wis::TextureView                              dst_texture,
 *                                       wis::BufferView                               src_buffer,
 *                                       wis::span<const wis::BufferTextureCopyRegion> regions) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::CopyBufferToTexture(wis::VKTextureView                            dst_texture,
 *                                         wis::VKBufferView                             src_buffer,
 *                                         wis::span<const wis::BufferTextureCopyRegion> regions) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::CopyBufferToTexture(wis::DX12TextureView                          dst_texture,
 *                                           wis::DX12BufferView                           src_buffer,
 *                                           wis::span<const wis::BufferTextureCopyRegion> regions) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListCopyBufferToTexture_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `dst_texture` defines a pointer to the destination texture. Texture @wis_must be in `WisTextureStateCopyDst`.
 * - `src_buffer` describes a pointer to the source buffer.
 * - `regions` points to an array of WisBufferTextureCopyRegion that defines the copy regions.
 * - `region_count` defines the count of the regions.
 * \endcond
 *
 * @section wisCommandListCopyBufferToTexture_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListCopyBufferToTexture_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */