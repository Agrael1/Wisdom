/**
 * @struct wisDeviceCreateDescriptorHeap
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateDescriptorHeap_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceCreateDescriptorHeap(const WisDevice* self,
 *                                         const WisDescriptorHeapDesc* desc,
 *                                         WisDescriptorHeap*           heap);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceCreateDescriptorHeap(const WisVKDevice* self,
 *                                           const WisDescriptorHeapDesc* desc,
 *                                           WisVKDescriptorHeap*         heap);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceCreateDescriptorHeap(const WisDX12Device* self,
 *                                             const WisDescriptorHeapDesc* desc,
 *                                             WisDX12DescriptorHeap*       heap);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DescriptorHeap Device::CreateDescriptorHeap(const wis::DescriptorHeapDesc& desc,
 *                                                                wis::Result&                   out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKDescriptorHeap VKDevice::CreateDescriptorHeap(const wis::DescriptorHeapDesc& desc,
 *                                                                    wis::Result&                   out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12DescriptorHeap DX12Device::CreateDescriptorHeap(const wis::DescriptorHeapDesc& desc,
 *                                                                        wis::Result&                   out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateDescriptorHeap_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `desc` indicates a pointer to WisDescriptorHeapDesc, which describes the descriptor heap to create.
 * - `heap` points to WisDescriptorHeap, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateDescriptorHeap_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateDescriptorHeap_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */