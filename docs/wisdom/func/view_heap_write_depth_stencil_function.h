/**
 * @struct wisViewHeapWriteDepthStencil
 * @ingroup Functions
 *
 *
 * @section wisViewHeapWriteDepthStencil_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisViewHeapWriteDepthStencil(const WisViewHeap* self,
 *                                       const WisTexture*          texture,
 *                                       const WisRenderTargetDesc* render_target,
 *                                       uint32_t                   index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisVKViewHeapWriteDepthStencil(const WisVKViewHeap* self,
 *                                         const WisVKTexture*        texture,
 *                                         const WisRenderTargetDesc* render_target,
 *                                         uint32_t                   index);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisDX12ViewHeapWriteDepthStencil(const WisDX12ViewHeap* self,
 *                                           const WisDX12Texture*      texture,
 *                                           const WisRenderTargetDesc* render_target,
 *                                           uint32_t                   index);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t ViewHeap::WriteDepthStencil(const wis::Texture&          texture,
 *                                                         const wis::RenderTargetDesc& render_target,
 *                                                         std::uint32_t                index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t VKViewHeap::WriteDepthStencil(const wis::VKTexture&        texture,
 *                                                           const wis::RenderTargetDesc& render_target,
 *                                                           std::uint32_t                index) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t DX12ViewHeap::WriteDepthStencil(const wis::DX12Texture&      texture,
 *                                                             const wis::RenderTargetDesc& render_target,
 *                                                             std::uint32_t                index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapWriteDepthStencil_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * - `texture` describes a pointer to WisTexture to write the descriptor for.
 * - `render_target` specifies a pointer to WisRenderTargetDesc, which describes the depth stencil view to write.
 * - `index` defines the index in the view heap to write the descriptor to.
 * 
 * - **return** CPU descriptor handle for the view heap.
 * \endcond
 *
 * @section wisViewHeapWriteDepthStencil_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapWriteDepthStencil_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */