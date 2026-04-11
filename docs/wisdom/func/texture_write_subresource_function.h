/**
 * @struct wisTextureWriteSubresource
 * @ingroup Functions Core
 *
 *
 * @section wisTextureWriteSubresource_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisTextureWriteSubresource(const WisTexture* self,
 *                                      const void*             source_data,
 *                                      const WisTextureRegion* target_region);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKTextureWriteSubresource(const WisVKTexture* self,
 *                                        const void*             source_data,
 *                                        const WisTextureRegion* target_region);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12TextureWriteSubresource(const WisDX12Texture* self,
 *                                          const void*             source_data,
 *                                          const WisTextureRegion* target_region);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result Texture::WriteSubresource(const void*               source_data,
 *                                       const wis::TextureRegion& target_region) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKTexture::WriteSubresource(const void*               source_data,
 *                                         const wis::TextureRegion& target_region) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12Texture::WriteSubresource(const void*               source_data,
 *                                           const wis::TextureRegion& target_region) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisTextureWriteSubresource_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisTexture instance.
 * - `source_data` specifies a pointer to the data to write to the texture.
 * - `target_region` specifies a pointer to WisTextureRegion, which describes the region of the texture to write to. The `flags` parameter of the region is ignored, as depth/stencil/planar is not supported.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisTextureWriteSubresource_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisTextureWriteSubresource_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
