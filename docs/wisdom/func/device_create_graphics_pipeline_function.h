/**
 * @struct wisDeviceCreateGraphicsPipeline
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateGraphicsPipeline_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceCreateGraphicsPipeline(const WisDevice* self,
 *                                           const WisGraphicsPipelineDesc* desc,
 *                                           WisPipeline*                   pipeline);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceCreateGraphicsPipeline(const WisVKDevice* self,
 *                                             const WisVKGraphicsPipelineDesc* desc,
 *                                             WisVKPipeline*                   pipeline);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceCreateGraphicsPipeline(const WisDX12Device* self,
 *                                               const WisDX12GraphicsPipelineDesc* desc,
 *                                               WisDX12Pipeline*                   pipeline);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::Pipeline Device::CreateGraphicsPipeline(const wis::GraphicsPipelineDesc& desc,
 *                                                            wis::Result&                     out_result) const
 * noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKPipeline VKDevice::CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc& desc,
 *                                                                wis::Result&                       out_result) const
 * noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12Pipeline DX12Device::CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc& desc,
 *                                                                    wis::Result&                         out_result)
 * const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateGraphicsPipeline_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `desc` indicates a pointer to WisGraphicsPipelineDesc, which describes the graphics pipeline to create.
 * - `pipeline` points to WisPipeline, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateGraphicsPipeline_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateGraphicsPipeline_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
