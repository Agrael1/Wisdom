/**
 * @struct wisCommandListResolveTexture
 * @ingroup Functions
 *
 *
 * @section wisCommandListResolveTexture_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListResolveTexture(const WisCommandList* self,
 *                                   WisTextureView                 dst_texture,
 *                                   WisTextureView                 src_texture,
 *                                   const WisTextureResolveRegion* regions,
 *                                   size_t                         region_count);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListResolveTexture(const WisVKCommandList* self,
 *                                     WisVKTextureView               dst_texture,
 *                                     WisVKTextureView               src_texture,
 *                                     const WisTextureResolveRegion* regions,
 *                                     size_t                         region_count);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListResolveTexture(const WisDX12CommandList* self,
 *                                       WisDX12TextureView             dst_texture,
 *                                       WisDX12TextureView             src_texture,
 *                                       const WisTextureResolveRegion* regions,
 *                                       size_t                         region_count);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section wisCommandListResolveTexture_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `dst_texture` defines a pointer to the destination texture. Texture @wis_must be in WisTextureState.
 * - `src_texture` describes a pointer to the source texture. Texture @wis_must be in WisTextureState.
 * - `regions` points to an array of WisTextureResolveRegion that defines the resolve regions.
 * - `region_count` defines the count of the regions.
 * 
 * \endcond
 *
 * @section wisCommandListResolveTexture_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListResolveTexture_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */