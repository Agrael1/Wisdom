/**
 * @struct wisCommandListSetBlendFactors
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetBlendFactors_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetBlendFactors(const WisCommandList* self,
 *                                    float                 blend_factor_r,
 *                                    float                 blend_factor_g,
 *                                    float                 blend_factor_b,
 *                                    float                 blend_factor_a);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetBlendFactors(const WisVKCommandList* self,
 *                                      float                   blend_factor_r,
 *                                      float                   blend_factor_g,
 *                                      float                   blend_factor_b,
 *                                      float                   blend_factor_a);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetBlendFactors(const WisDX12CommandList* self,
 *                                        float                     blend_factor_r,
 *                                        float                     blend_factor_g,
 *                                        float                     blend_factor_b,
 *                                        float                     blend_factor_a);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetBlendFactors(float blend_factor_r,
 *                                   float blend_factor_g,
 *                                   float blend_factor_b,
 *                                   float blend_factor_a) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetBlendFactors(float blend_factor_r,
 *                                     float blend_factor_g,
 *                                     float blend_factor_b,
 *                                     float blend_factor_a) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetBlendFactors(float blend_factor_r,
 *                                       float blend_factor_g,
 *                                       float blend_factor_b,
 *                                       float blend_factor_a) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetBlendFactors_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `blend_factor_r` specifies blend factor for red color to set.
 * - `blend_factor_g` specifies blend factor for green color to set.
 * - `blend_factor_b` specifies blend factor for blue color to set.
 * - `blend_factor_a` specifies blend factor for alpha channel to set.
 * \endcond
 *
 * @section wisCommandListSetBlendFactors_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetBlendFactors_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
