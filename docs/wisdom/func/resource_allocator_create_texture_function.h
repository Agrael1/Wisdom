/**
 * @struct wisResourceAllocatorCreateTexture
 * @ingroup Functions
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
 *                                             WisTextureState             initial_state,
 *                                             WisTexture*                 texture);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKResourceAllocatorCreateTexture(const WisVKResourceAllocator* self,
 *                                               const WisTextureDesc*         desc,
 *                                               WisTextureState               initial_state,
 *                                               WisVKTexture*                 texture);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12ResourceAllocatorCreateTexture(const WisDX12ResourceAllocator* self,
 *                                                 const WisTextureDesc*           desc,
 *                                                 WisTextureState                 initial_state,
 *                                                 WisDX12Texture*                 texture);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Texture ResourceAllocator::CreateTexture(const wis::TextureDesc& desc,
 *                                                             wis::TextureState       initial_state,
 *                                                             wis::Result&            out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKTexture VKResourceAllocator::CreateTexture(const wis::TextureDesc& desc,
 *                                                                 wis::TextureState       initial_state,
 *                                                                 wis::Result&            out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Texture DX12ResourceAllocator::CreateTexture(const wis::TextureDesc& desc,
 *                                                                     wis::TextureState       initial_state,
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
 * - `desc` points to WisTextureDesc, which describes the texture to create.
 * - `initial_state` defines the initial state of the texture. State transition @wis_must be supported, query WisDeviceMemoryProperties to get if the transition is supported. If not, @wis_must be `WisTextureStateUndefined`.
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