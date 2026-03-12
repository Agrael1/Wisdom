/**
 * @struct wisDeviceCreateComputePipeline
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreateComputePipeline_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateComputePipeline(const WisDevice* self,
 *                                          const WisComputePipelineDesc* desc,
 *                                          WisPipeline*                  pipeline);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateComputePipeline(const WisVKDevice* self,
 *                                            const WisVKComputePipelineDesc* desc,
 *                                            WisVKPipeline*                  pipeline);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateComputePipeline(const WisDX12Device* self,
 *                                              const WisDX12ComputePipelineDesc* desc,
 *                                              WisDX12Pipeline*                  pipeline);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Pipeline Device::CreateComputePipeline(const wis::ComputePipelineDesc& desc,
 *                                                           wis::Result&                    out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKPipeline VKDevice::CreateComputePipeline(const wis::VKComputePipelineDesc& desc,
 *                                                               wis::Result&                      out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Pipeline DX12Device::CreateComputePipeline(const wis::DX12ComputePipelineDesc& desc,
 *                                                                   wis::Result&                        out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateComputePipeline_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `desc` points to WisComputePipelineDesc, which describes the compute pipeline to create.
 * - `pipeline` points to WisPipeline, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateComputePipeline_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateComputePipeline_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */