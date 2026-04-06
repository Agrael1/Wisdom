/**
 * @struct wisResourceAllocatorCreateTexture
 * @ingroup Functions Core
 *
 *
 * @section wisResourceAllocatorCreateTexture_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisResourceAllocatorCreateTexture(const WisResourceAllocator* self,
 *                                             const WisTextureDesc*       desc,
 *                                             WisTexture*                 texture);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKResourceAllocatorCreateTexture(const WisVKResourceAllocator* self,
 *                                               const WisTextureDesc*         desc,
 *                                               WisVKTexture*                 texture);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12ResourceAllocatorCreateTexture(const WisDX12ResourceAllocator* self,
 *                                                 const WisTextureDesc*           desc,
 *                                                 WisDX12Texture*                 texture);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::Texture ResourceAllocator::CreateTexture(const wis::TextureDesc& desc,
 *                                                             wis::Result&            out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKTexture VKResourceAllocator::CreateTexture(const wis::TextureDesc& desc,
 *                                                                 wis::Result&            out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12Texture DX12ResourceAllocator::CreateTexture(const wis::TextureDesc& desc,
 *                                                                     wis::Result&            out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisResourceAllocatorCreateTexture_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisResourceAllocator instance.
 * - `desc` indicates a pointer to WisTextureDesc, which describes the texture to create.
 * - `texture` points to WisTexture, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisResourceAllocatorCreateTexture_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisResourceAllocatorCreateTexture_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */