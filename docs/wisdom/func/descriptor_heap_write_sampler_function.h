/**
 * @struct wisDescriptorHeapWriteSampler
 * @ingroup Functions Core
 *
 *
 * @section wisDescriptorHeapWriteSampler_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDescriptorHeapWriteSampler(const WisDescriptorHeap* self,
 *                                         const WisSamplerDesc*    sampler,
 *                                         uint32_t                 index);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDescriptorHeapWriteSampler(const WisVKDescriptorHeap* self,
 *                                           const WisSamplerDesc*      sampler,
 *                                           uint32_t                   index);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DescriptorHeapWriteSampler(const WisDX12DescriptorHeap* self,
 *                                             const WisSamplerDesc*        sampler,
 *                                             uint32_t                     index);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result DescriptorHeap::WriteSampler(const wis::SamplerDesc& sampler,
 *                                          std::uint32_t           index) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKDescriptorHeap::WriteSampler(const wis::SamplerDesc& sampler,
 *                                            std::uint32_t           index) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12DescriptorHeap::WriteSampler(const wis::SamplerDesc& sampler,
 *                                              std::uint32_t           index) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDescriptorHeapWriteSampler_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDescriptorHeap instance.
 * - `sampler` defines a pointer to WisSamplerDesc, which describes the sampler descriptor to write.
 * - `index` defines the index in the descriptor heap to write the descriptor to.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDescriptorHeapWriteSampler_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDescriptorHeapWriteSampler_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
