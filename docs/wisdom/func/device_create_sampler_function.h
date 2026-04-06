/**
 * @struct wisDeviceCreateSampler
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateSampler_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateSampler(const WisDevice* self,
 *                                  const WisSamplerDesc* desc,
 *                                  WisSampler*           sampler);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateSampler(const WisVKDevice* self,
 *                                    const WisSamplerDesc* desc,
 *                                    WisVKSampler*         sampler);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateSampler(const WisDX12Device* self,
 *                                      const WisSamplerDesc* desc,
 *                                      WisDX12Sampler*       sampler);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Sampler Device::CreateSampler(const wis::SamplerDesc& desc,
 *                                                  wis::Result&            out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKSampler VKDevice::CreateSampler(const wis::SamplerDesc& desc,
 *                                                      wis::Result&            out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Sampler DX12Device::CreateSampler(const wis::SamplerDesc& desc,
 *                                                          wis::Result&            out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateSampler_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `desc` points to WisSamplerDesc, which describes the sampler to create.
 * - `sampler` points to WisSampler, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateSampler_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateSampler_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
