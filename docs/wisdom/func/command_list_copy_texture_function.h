/**
 * @struct wisCommandListCopyTexture
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListCopyTexture_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListCopyTexture(const WisCommandList* self,
 *                                WisTextureView              dst_texture,
 *                                WisTextureView              src_texture,
 *                                const WisTextureCopyRegion* regions,
 *                                size_t                      region_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListCopyTexture(const WisVKCommandList* self,
 *                                  WisVKTextureView            dst_texture,
 *                                  WisVKTextureView            src_texture,
 *                                  const WisTextureCopyRegion* regions,
 *                                  size_t                      region_count);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListCopyTexture(const WisDX12CommandList* self,
 *                                    WisDX12TextureView          dst_texture,
 *                                    WisDX12TextureView          src_texture,
 *                                    const WisTextureCopyRegion* regions,
 *                                    size_t                      region_count);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::CopyTexture(wis::TextureView                        dst_texture,
 *                               wis::TextureView                        src_texture,
 *                               wis::span<const wis::TextureCopyRegion> regions) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::CopyTexture(wis::VKTextureView                      dst_texture,
 *                                 wis::VKTextureView                      src_texture,
 *                                 wis::span<const wis::TextureCopyRegion> regions) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::CopyTexture(wis::DX12TextureView                    dst_texture,
 *                                   wis::DX12TextureView                    src_texture,
 *                                   wis::span<const wis::TextureCopyRegion> regions) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListCopyTexture_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `dst_texture` defines a pointer to the destination texture. Texture @wis_must be in `WisTextureStateCopyDst`.
 * - `src_texture` describes a pointer to the source texture. Texture @wis_must be in `WisTextureStateCopySrc`.
 * - `regions` points to an array of WisTextureCopyRegion that defines the copy regions.
 * - `region_count` defines the count of the regions.
 * \endcond
 *
 * @section wisCommandListCopyTexture_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListCopyTexture_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
