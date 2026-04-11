/**
 * @struct wisDeviceCreateRootSignature
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateRootSignature_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceCreateRootSignature(const WisDevice* self,
 *                                        const WisRootSignatureDesc* desc,
 *                                        WisRootSignature*           layout);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceCreateRootSignature(const WisVKDevice* self,
 *                                          const WisRootSignatureDesc* desc,
 *                                          WisVKRootSignature*         layout);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceCreateRootSignature(const WisDX12Device* self,
 *                                            const WisRootSignatureDesc* desc,
 *                                            WisDX12RootSignature*       layout);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::RootSignature Device::CreateRootSignature(const wis::RootSignatureDesc& desc,
 *                                                              wis::Result&                  out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKRootSignature VKDevice::CreateRootSignature(const wis::RootSignatureDesc& desc,
 *                                                                  wis::Result&                  out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12RootSignature DX12Device::CreateRootSignature(const wis::RootSignatureDesc& desc,
 *                                                                      wis::Result&                  out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateRootSignature_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `desc` indicates a pointer to WisRootSignatureDesc, which describes the pipeline layout to create.
 * - `layout` points to WisRootSignature, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateRootSignature_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateRootSignature_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
