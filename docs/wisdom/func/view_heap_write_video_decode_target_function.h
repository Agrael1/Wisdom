/**
 * @struct wisViewHeapWriteVideoDecodeTarget
 * @ingroup Functions Core
 *
 *
 * @section wisViewHeapWriteVideoDecodeTarget_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * uint64_t wisViewHeapWriteVideoDecodeTarget(const WisViewHeap* self,
 *                                            const WisTexture*          texture,
 *                                            const WisRenderTargetDesc* render_target,
 *                                            uint32_t                   index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * uint64_t wisVKViewHeapWriteVideoDecodeTarget(const WisVKViewHeap* self,
 *                                              const WisVKTexture*        texture,
 *                                              const WisRenderTargetDesc* render_target,
 *                                              uint32_t                   index);
 *
 * // Provided by Wisdom 0.7.1.
 * uint64_t wisDX12ViewHeapWriteVideoDecodeTarget(const WisDX12ViewHeap* self,
 *                                                const WisDX12Texture*      texture,
 *                                                const WisRenderTargetDesc* render_target,
 *                                                uint32_t                   index);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD std::uint64_t ViewHeap::WriteVideoDecodeTarget(const wis::Texture&          texture,
 *                                                              const wis::RenderTargetDesc& render_target,
 *                                                              std::uint32_t                index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD std::uint64_t VKViewHeap::WriteVideoDecodeTarget(const wis::VKTexture&        texture,
 *                                                                const wis::RenderTargetDesc& render_target,
 *                                                                std::uint32_t                index) const noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD std::uint64_t DX12ViewHeap::WriteVideoDecodeTarget(const wis::DX12Texture&      texture,
 *                                                                  const wis::RenderTargetDesc& render_target,
 *                                                                  std::uint32_t                index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisViewHeapWriteVideoDecodeTarget_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisViewHeap instance.
 * - `texture` describes a pointer to WisTexture to write the view for.
 * - `render_target` specifies a pointer to WisRenderTargetDesc, which describes the texture view to write.
 * - `index` defines the index in the view heap to write the view to.
 *
 * - **return** CPU descriptor handle for the view heap.
 * \endcond
 *
 * @section wisViewHeapWriteVideoDecodeTarget_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisViewHeapWriteVideoDecodeTarget_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
