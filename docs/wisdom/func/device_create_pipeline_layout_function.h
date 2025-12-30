/**
 * @struct wisDeviceCreatePipelineLayout
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreatePipelineLayout_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreatePipelineLayout(const WisDevice* self,
 *                                         const WisPipelineLayoutDesc* desc,
 *                                         WisPipelineLayout*           layout);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreatePipelineLayout(const WisVKDevice* self,
 *                                           const WisPipelineLayoutDesc* desc,
 *                                           WisVKPipelineLayout*         layout);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreatePipelineLayout(const WisDX12Device* self,
 *                                             const WisPipelineLayoutDesc* desc,
 *                                             WisDX12PipelineLayout*       layout);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::PipelineLayout Device::CreatePipelineLayout(const wis::PipelineLayoutDesc& desc,
 *                                                                wis::Result&                   out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKPipelineLayout VKDevice::CreatePipelineLayout(const wis::PipelineLayoutDesc& desc,
 *                                                                    wis::Result&                   out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12PipelineLayout DX12Device::CreatePipelineLayout(const wis::PipelineLayoutDesc& desc,
 *                                                                        wis::Result&                   out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreatePipelineLayout_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `desc` points to WisPipelineLayoutDesc, which describes the pipeline layout to create.
 * - `layout` points to WisPipelineLayout, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreatePipelineLayout_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreatePipelineLayout_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */