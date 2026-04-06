/**
 * @struct wisDescriptorHeapWriteTexture
 * @ingroup Functions Core
 *
 *
 * @section wisDescriptorHeapWriteTexture_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDescriptorHeapWriteTexture(const WisDescriptorHeap* self,
 *                                         WisTextureView           texture,
 *                                         const WisTextureBinding* data,
 *                                         uint32_t                 index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDescriptorHeapWriteTexture(const WisVKDescriptorHeap* self,
 *                                           WisVKTextureView           texture,
 *                                           const WisTextureBinding*   data,
 *                                           uint32_t                   index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DescriptorHeapWriteTexture(const WisDX12DescriptorHeap* self,
 *                                             WisDX12TextureView           texture,
 *                                             const WisTextureBinding*     data,
 *                                             uint32_t                     index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DescriptorHeap::WriteTexture(wis::TextureView           texture,
 *                                          const wis::TextureBinding& data,
 *                                          std::uint32_t              index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKDescriptorHeap::WriteTexture(wis::VKTextureView         texture,
 *                                            const wis::TextureBinding& data,
 *                                            std::uint32_t              index) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12DescriptorHeap::WriteTexture(wis::DX12TextureView       texture,
 *                                              const wis::TextureBinding& data,
 *                                              std::uint32_t              index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapWriteTexture_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * - `texture` describes a pointer to WisTexture to write the descriptor for.
 * - `data` specifies a pointer to WisTextureBinding, which describes the texture view to write.
 * - `index` defines the index in the descriptor heap to write the descriptor to.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDescriptorHeapWriteTexture_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapWriteTexture_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */