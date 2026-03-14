/**
 * @struct wisCommandListSetDepthBias
 * @ingroup Functions
 *
 *
 * @section wisCommandListSetDepthBias_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListSetDepthBias(WisCommandList* self,
 *                                 float           depth_bias,
 *                                 float           depth_bias_clamp,
 *                                 float           slope_scaled_depth_bias);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListSetDepthBias(WisVKCommandList* self,
 *                                   float             depth_bias,
 *                                   float             depth_bias_clamp,
 *                                   float             slope_scaled_depth_bias);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListSetDepthBias(WisDX12CommandList* self,
 *                                     float               depth_bias,
 *                                     float               depth_bias_clamp,
 *                                     float               slope_scaled_depth_bias);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::SetDepthBias(float depth_bias,
 *                                float depth_bias_clamp,
 *                                float slope_scaled_depth_bias) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::SetDepthBias(float depth_bias,
 *                                  float depth_bias_clamp,
 *                                  float slope_scaled_depth_bias) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::SetDepthBias(float depth_bias,
 *                                    float depth_bias_clamp,
 *                                    float slope_scaled_depth_bias) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetDepthBias_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `depth_bias` The depth bias to set.
 * - `depth_bias_clamp` The depth bias clamp to set.
 * - `slope_scaled_depth_bias` The slope scaled depth bias to set.
 * \endcond
 *
 * @section wisCommandListSetDepthBias_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetDepthBias_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */